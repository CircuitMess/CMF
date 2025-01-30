#ifndef CMF_RAWCACHE_H
#define CMF_RAWCACHE_H

#include <unordered_map>
#include <vector>
#include "FileCache.h"

class RawCache : public FileCache {
public:
	explicit RawCache();
	explicit RawCache(const std::vector<std::string>& paths);

	void setPaths(const std::vector<std::string>& paths);

	void load() override;
	void unload() override;

	File open(const char* path) override;

private:
	std::vector<std::string> paths;

	bool loaded = false;
	std::unordered_map<std::string, File> files;
};

#endif //CMF_RAWCACHE_H
