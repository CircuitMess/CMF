#ifndef BUTTERBOTCTRL_FIRMWARE_COMPRESSEDFILE_H
#define BUTTERBOTCTRL_FIRMWARE_COMPRESSEDFILE_H

#include <cstdint>
#include <vector>
#include <string>
#include "FileImpl.h"
#include "File.h"

class CompressedFile : public FileImpl {
public:
	/**
	 * @param file input compressed File object
	 * @param reserveSize size to reserve for internal buffer
	 * @param use32bAligned if memory is organized in 32bit alignment
	 */
	CompressedFile(File file, size_t reserveSize = 0, bool use32bAligned = false);
	CompressedFile(const uint8_t* compressedData, size_t compressedSize, const char* name, size_t reserveSize = 0, bool use32bAligned = false);
	~CompressedFile() override;

	operator bool() override;

	static File open(const File& file, size_t reserveSize = 0, bool use32bAligned = false);
	void close() override;

	size_t size() const override;
	const char* name() const override;

	size_t read(uint8_t* dest, size_t len) override;
	size_t write(const uint8_t* buf, size_t size) override;
	void flush() override;

	bool seek(int pos, int whence) override;
	size_t pos() const override;

private:
	void decompress(const uint8_t* compressedData, size_t compressedSize, bool use32bAligned);

	std::vector<uint8_t> result;
	size_t cursor = 0;
	std::string filePath;
};

#endif //BUTTERBOTCTRL_FIRMWARE_COMPRESSEDFILE_H
