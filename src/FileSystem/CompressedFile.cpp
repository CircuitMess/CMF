#include "CompressedFile.h"

#include "Log/Log.h"
#include <cstring>
#include <esp_heap_caps.h>

DEFINE_LOG(CompressedFile)

CompressedFile::CompressedFile(File file, bool use32bAligned) : filePath(file.name()){
	if(!file){
		CMF_LOG(CompressedFile, LogLevel::Error, "Couldn't open file: %s", file.name());
		return;
	}

	file.seek(0);
	fileSize = file.size();

	if(fileSize == 0){
		CMF_LOG(CompressedFile, LogLevel::Error, "File is empty: %s", file.name());
		file.close();
		return;
	}
	size_t allocSize = fileSize;
	if(use32bAligned){
		auto rest = fileSize % 4;
		if(rest != 0){
			allocSize += (4 - rest);
		}
	}

	data = (uint8_t*) heap_caps_malloc(allocSize, MALLOC_CAP_INTERNAL | (use32bAligned ? MALLOC_CAP_32BIT : MALLOC_CAP_8BIT));
	if(data == nullptr){
		fileSize = 0;
		CMF_LOG(CompressedFile, LogLevel::Error, "Couldn't allocate memory for %s. Need %zu B, largest block: %zu B", file.name(), allocSize,
				 heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | (use32bAligned ? MALLOC_CAP_32BIT : MALLOC_CAP_8BIT)));
		file.close();

		return;
	}

	file.read(data, fileSize);
}

CompressedFile::CompressedFile(uint8_t* data, size_t size, const char* name) : data(data), filePath(name), fileSize(size), borrowed(true){

}

CompressedFile::~CompressedFile(){
	if(!borrowed){
		free(data);
	}
}

CompressedFile::operator bool(){
	return data != nullptr;
}

File CompressedFile::open(const File& file, bool use32bAligned){
	auto f = std::make_shared<CompressedFile>(file, use32bAligned);
	return { f };
}

void CompressedFile::close(){
	delete data;

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
	}else if(whence == SEEK_CUR){
		cursor += pos;
	}else if(whence == SEEK_END){
		cursor = fileSize - pos;
	}
	return true;
}

size_t CompressedFile::pos() const{
	return cursor;
}
