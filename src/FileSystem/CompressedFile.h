#ifndef BUTTERBOTCTRL_FIRMWARE_COMPRESSEDFILE_H
#define BUTTERBOTCTRL_FIRMWARE_COMPRESSEDFILE_H

#include <cstdint>
#include <string>
#include "FileImpl.h"
#include "File.h"

class CompressedFile : public FileImpl {
public:
	CompressedFile(File file, bool use32bAligned = false);
	CompressedFile(uint8_t* data, size_t size, const char* name);
	~CompressedFile() override;

	operator bool() override;

	static File open(const File& file, bool use32bAligned = false);
	void close() override;

	size_t size() const override;
	const char* name() const override;

	size_t read(uint8_t* dest, size_t len) override;
	size_t write(const uint8_t* buf, size_t size) override;
	void flush() override;

	bool seek(int pos, int whence) override;
	size_t pos() const override;

private:
	uint8_t* data = nullptr;
	size_t cursor = 0;
	std::string filePath;
	size_t fileSize = 0;
	bool borrowed = false;
};

#endif //BUTTERBOTCTRL_FIRMWARE_COMPRESSEDFILE_H
