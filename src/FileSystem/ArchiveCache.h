#ifndef CMF_ARCHIVECACHE_H
#define CMF_ARCHIVECACHE_H

#include "FileCache.h"
#include <unordered_map>
#include <string>
#include "FileSystem/File.h"

class ArchiveCache : public FileCache {
public:
	ArchiveCache(File file);
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

	bool externalData = false;
	uint8_t* data = nullptr;
};

#endif //CMF_ARCHIVECACHE_H
