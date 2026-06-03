#include "ArchiveCache.h"
#include <vector>
#include "Log/Log.h"
#include "FileSystem/RamFile.h"

extern "C" {
#include <heatshrink_decoder.h>
}

DEFINE_LOG(ArchiveCache)

ArchiveCache::ArchiveCache(const File& file) : archiveFile(file){
	if(!file){
		CMF_LOG(ArchiveCache, LogLevel::Error, "File not opened: %s", file.name());
	}
	ArchiveCache::load();
}

ArchiveCache::~ArchiveCache(){
	free(data);
}

void ArchiveCache::setArchiveFile(const File& file){
	if(!file){
		CMF_LOG(ArchiveCache, LogLevel::Error, "New file not opened: %s", file.name());
	}
	archiveFile = file;
}

void ArchiveCache::load(){
	if(loaded) return;
	loaded = true;

	archiveFile.seek(0);
	uint32_t count = 0;
	archiveFile.read((uint8_t*)&count, 4);

	std::vector<Entry> tmp;
	tmp.reserve(count);

	size_t totalSize = 0;

	while(archiveFile.available()){
		std::string name;
		name.reserve(32);
		while(archiveFile.available()){
			if(name.length() >= 32){
				CMF_LOG(ArchiveCache, LogLevel::Error, "Too big filename; %s...", name.c_str());
				abort();
			}

			char c = 0;
			archiveFile.read((uint8_t*)&c, 1);
			if(c == 0) break;

			name.append(1, c);
		}

		if(name.empty()) break;

		size_t size = 0;
		archiveFile.read((uint8_t*)&size, 4);
		totalSize += size;

		tmp.emplace_back(Entry{ name, size, 0 });
	}

	data = (uint8_t*)malloc(totalSize);

	if(data == nullptr){
		CMF_LOG(ArchiveCache, LogLevel::Warning, "Failed allocating data buffer of %zu B", totalSize);
		return;
	}

	size_t offset = 0;

	for(auto& entry : tmp){
		archiveFile.read(data + offset, entry.size);
		entry.offset = offset;

		offset += entry.size;

		entries.insert(std::make_pair(entry.name, std::move(entry)));
	}
}

void ArchiveCache::unload(){
	if(!loaded) return;
	loaded = false;

	entries.clear();
}

File ArchiveCache::open(const char* path){
	const auto entry = entries.find(path);
	if(entry == entries.end()) return File();

	const auto ramFile = std::make_shared<RamFile>(data + entry->second.offset, entry->second.size, path);
	return File(ramFile);
}
