#include "CompressedFile.h"

#include "Log/Log.h"
#include <cstring>
#include <vector>
#include <esp_heap_caps.h>

extern "C" {
#include <heatshrink_decoder.h>
}

DEFINE_LOG(CompressedFile)

static constexpr uint8_t HEATSHRINK_WINDOW_SZ2 = 14;
static constexpr uint8_t HEATSHRINK_LOOKAHEAD_SZ2 = 7;
static constexpr uint16_t HEATSHRINK_INPUT_BUF_SIZE = 256;
static constexpr size_t HEATSHRINK_POLL_BUF_SIZE = 1024;

uint8_t* CompressedFile::decompress(const uint8_t* compressedData, size_t compressedSize, size_t& outSize, bool use32bAligned){
	heatshrink_decoder* hsd = heatshrink_decoder_alloc(HEATSHRINK_INPUT_BUF_SIZE, HEATSHRINK_WINDOW_SZ2, HEATSHRINK_LOOKAHEAD_SZ2);
	if(!hsd){
		CMF_LOG(CompressedFile, LogLevel::Error, "Failed to allocate heatshrink decoder");
		outSize = 0;
		return nullptr;
	}

	std::vector<uint8_t> result;
	uint8_t pollBuf[HEATSHRINK_POLL_BUF_SIZE];

	auto pollAll = [&]() {
		HSD_poll_res pres;
		do{
			size_t polled = 0;
			pres = heatshrink_decoder_poll(hsd, pollBuf, HEATSHRINK_POLL_BUF_SIZE, &polled);
			result.insert(result.end(), pollBuf, pollBuf + polled);
		} while(pres == HSDR_POLL_MORE);
	};

	size_t inOffset = 0;
	while(inOffset < compressedSize){
		size_t sunk = 0;
		heatshrink_decoder_sink(hsd, const_cast<uint8_t*>(compressedData + inOffset), compressedSize - inOffset, &sunk);
		inOffset += sunk;
		pollAll();
	}

	HSD_finish_res fres = heatshrink_decoder_finish(hsd);
	while(fres == HSDR_FINISH_MORE){
		pollAll();
		fres = heatshrink_decoder_finish(hsd);
	}

	heatshrink_decoder_free(hsd);

	outSize = result.size();
	if(outSize == 0){
		CMF_LOG(CompressedFile, LogLevel::Error, "Decompression produced no output");
		return nullptr;
	}

	size_t allocSize = outSize;
	if(use32bAligned){
		const auto rest = outSize % 4;
		if(rest != 0) allocSize += (4 - rest);
	}

	auto* buf = static_cast<uint8_t*>(heap_caps_malloc(allocSize,MALLOC_CAP_SPIRAM | (use32bAligned ? MALLOC_CAP_32BIT : MALLOC_CAP_8BIT)));
	if(!buf){
		CMF_LOG(CompressedFile, LogLevel::Error, "Couldn't allocate %zu B for decompressed data", allocSize);
		outSize = 0;
		return nullptr;
	}

	memcpy(buf, result.data(), outSize);
	return buf;
}

CompressedFile::CompressedFile(File file, bool use32bAligned) : filePath(file.name()){
	if(!file){
		CMF_LOG(CompressedFile, LogLevel::Error, "Couldn't open file: %s", file.name());
		return;
	}

	file.seek(0);
	const size_t compressedSize = file.size();

	if(compressedSize == 0){
		CMF_LOG(CompressedFile, LogLevel::Error, "File is empty: %s", file.name());
		file.close();
		return;
	}

	auto* compressedData = static_cast<uint8_t*>(malloc(compressedSize));
	if(!compressedData){
		CMF_LOG(CompressedFile, LogLevel::Error, "Couldn't allocate %zu B to read compressed file: %s", compressedSize, file.name());
		file.close();
		return;
	}

	file.read(compressedData, compressedSize);
	data = decompress(compressedData, compressedSize, fileSize, use32bAligned);
	free(compressedData);

	if(!data){
		CMF_LOG(CompressedFile, LogLevel::Error, "Decompression failed for: %s", file.name());
	}
}

CompressedFile::CompressedFile(const uint8_t* compressedData, size_t compressedSize, const char* name, bool use32bAligned) : filePath(name){
	data = decompress(compressedData, compressedSize, fileSize, use32bAligned);
	if(!data){
		CMF_LOG(CompressedFile, LogLevel::Error, "Decompression failed for: %s", name);
	}
}

CompressedFile::~CompressedFile(){
	free(data);
}

CompressedFile::operator bool(){
	return data != nullptr;
}

File CompressedFile::open(const File& file, bool use32bAligned){
	auto f = std::make_shared<CompressedFile>(file, use32bAligned);
	return { f };
}

void CompressedFile::close(){
	free(data);

	data = nullptr;
	fileSize = 0;
	filePath = "";
}

size_t CompressedFile::size() const{
	return fileSize;
}

const char* CompressedFile::name() const{
	return filePath.c_str();
}

size_t IRAM_ATTR CompressedFile::read(uint8_t* dest, size_t len){
	if(cursor >= fileSize) return 0;
	len = std::min(len, fileSize - cursor);
	if(len <= 0) return 0;

	memcpy(dest, data + cursor, len);
	cursor += len;

	return len;
}

size_t CompressedFile::write(const uint8_t* buf, size_t size){
	return 0;
}

void CompressedFile::flush(){
}

bool CompressedFile::seek(int pos, int whence){
	if(whence == SEEK_SET){
		cursor = pos;
	} else if(whence == SEEK_CUR){
		cursor += pos;
	} else if(whence == SEEK_END){
		cursor = fileSize - pos;
	}
	return true;
}

size_t CompressedFile::pos() const{
	return cursor;
}
