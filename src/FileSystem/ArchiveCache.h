#ifndef CMF_ARCHIVECACHE_H
#define CMF_ARCHIVECACHE_H

#include "FileCache.h"
#include <unordered_map>
#include <string>
#include "FileSystem/File.h"

class ArchiveCache : public FileCache {
public:
	ArchiveCache(const File& file);
	~ArchiveCache() override;

	void load() override;
	void unload() override;

	File open(const char* path) override;

private:
	struct Entry {
		const std::string name;
		size_t size;
		size_t offset;
	};

	std::unordered_map<std::string, const Entry> entries;

	bool loaded = false;
	uint8_t* data = nullptr;
	File archiveFile;
};

#endif //CMF_ARCHIVECACHE_H
