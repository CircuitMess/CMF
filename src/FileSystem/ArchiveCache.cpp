#include "ArchiveCache.h"
#include "SPIFFS.h"
#include "Log/Log.h"
#include <cstring>

DEFINE_LOG(ArchiveCache)

ArchiveCache::ArchiveCache(const std::vector<std::string>& paths) : paths(paths){
	archives.reserve(paths.size());
}

void ArchiveCache::load(){
	if(loaded) return;
	loaded = true;

	for(const auto& path : paths){
		const auto filename = path + ".sz";

		auto file = SPIFFS::open(filename.c_str());
		if(!file){
			CMF_LOG(ArchiveCache, LogLevel::Warning, "Can't open archive %s", path.c_str());
			continue;
		}

		archives.emplace(path, FileArchive(file));
	}
}

void ArchiveCache::unload(){
	if(!loaded) return;
	loaded = false;

	archives.clear();
}

File ArchiveCache::open(const char* path){
	const char* slash = strchr(path+1, '/');
	if(slash == nullptr) return { };

	const auto archive = archives.find(std::string(path, slash));
	if(archive == archives.end()) return { };

	return archive->second.get(slash+1, path);
}
