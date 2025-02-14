#include "AACSource.h"

DEFINE_LOG(AACSource)

void AACSource::open(const std::string& path){
	file = std::ifstream(path);
	if(!file.is_open()){
		CMF_LOG(AACSource, LogLevel::Error, "Failed to open file %s", path.c_str());
		return;
	}

	if(decoder){
		AACFreeDecoder(decoder);
	}

	decoder = AACInitDecoder();

	if(decoder == nullptr){
		CMF_LOG(AACSource, LogLevel::Error, "Libhelix AAC decoder failed to initialize.");
		return;
	}
}

void AACSource::close(){
	file.close();
	AACFreeDecoder(decoder);
	decoder = nullptr;

	fillBuffer.clear();
	dataBuffer.clear();
	bytesRemaining = 0;
}

size_t AACSource::getData(uint8_t* buffer, size_t bytes){
	if(decoder == nullptr){
		CMF_LOG(AACSource, LogLevel::Debug, "early return, decoder == nullptr");
		return 0;
	}

	if(buffer == nullptr){
		CMF_LOG(AACSource, LogLevel::Debug, "early return, buffer == nullptr");
		return 0;
	}

	if(bytes == 0){
		CMF_LOG(AACSource, LogLevel::Debug, "early return, bytes == 0");
		return 0;
	}


	size_t bytesTransferred = 0;

	if(!dataBuffer.empty()){
		memcpy(buffer, dataBuffer.data(), std::min(dataBuffer.size(), bytes));
		bytesTransferred += dataBuffer.size();
		dataBuffer.clear();
	}

	while(bytesTransferred < bytes){
		CMF_LOG(AACSource, LogLevel::Debug, "bytesRemaining: %d", bytesRemaining);

		if(fillBuffer.size() < FileReadThreshold && file){
			fillBuffer.resize(fillBuffer.size() + FileReadChunkSize);
			file.read(fillBuffer.data() + fillBuffer.size() - FileReadChunkSize, FileReadChunkSize);
			bytesRemaining += file.gcount();
			CMF_LOG(AACSource, LogLevel::Debug, "fillBuffer resized, bytesRemaining: %d", bytesRemaining);
		}

		if(bytesRemaining <= 0){
			break;
		}

		unsigned char* inBuffer = reinterpret_cast<unsigned char *>(fillBuffer.data());

		const int bytesRemainingBefore = bytesRemaining;

		dataBuffer.resize(dataBuffer.size() + DecodeOutBufferSize, 0);

		if(const int ret = AACDecode(decoder, &inBuffer, &bytesRemaining, reinterpret_cast<SampleType*>(dataBuffer.data() + dataBuffer.size() - DecodeOutBufferSize))){
			CMF_LOG(AACSource, LogLevel::Error, "AAC decoding error %d", ret);
			return 0;
		}

		const int bytesDecoded = bytesRemainingBefore - bytesRemaining;
		CMF_LOG(AACSource, LogLevel::Debug, "decoded: %d", bytesDecoded);

		fillBuffer.erase(fillBuffer.begin(), fillBuffer.begin() + bytesDecoded);

		const int bytesToTransfer = std::min(dataBuffer.size(), bytes - bytesTransferred);
		memcpy((buffer + bytesTransferred / SampleSize), dataBuffer.data(), bytesToTransfer);
		dataBuffer.erase(dataBuffer.begin(), dataBuffer.begin() + bytesToTransfer);

		bytesTransferred += bytesToTransfer;
		CMF_LOG(AACSource, LogLevel::Debug, "bytesTransferred: %d", bytesTransferred);
	}

	return bytesTransferred;
}
