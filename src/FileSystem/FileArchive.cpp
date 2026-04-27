#include "FileArchive.h"
#include <vector>
#include "Log/Log.h"
#include "FileSystem/RamFile.h"

DEFINE_LOG(FileArchive)

FileArchive::FileArchive(File file, const std::unordered_set<std::string>& excluded){
	file.seek(0);
	uint32_t count = 0;
	file.read((uint8_t*) &count, 4);

	std::vector<Entry> tmp;
	tmp.reserve(count);

	size_t totalSize = 0;

	while(file.available()){
		std::string name;
		name.reserve(32);
		while(file.available()){
			if(name.length() >= 32){
				CMF_LOG(FileArchive, LogLevel::Error, "Too big filename; %s...", name.c_str());
				abort();
			}

			char c = 0;
			file.read((uint8_t*) &c, 1);
			if(c == 0) break;

			name.append(1, c);
		}

		if(name.empty()) break;

		size_t size = 0;
		file.read((uint8_t*) &size, 4);

		if(!excluded.contains(name)){
			totalSize += size;
		}

		tmp.emplace_back(Entry { name, size, 0 });
	}

	data = (uint8_t*) malloc(totalSize);
	externalData = false;

	if(data == nullptr){
		CMF_LOG(FileArchive, LogLevel::Warning, "Failed allocating data buffer of %zu B", totalSize);
		return;
	}

	size_t offset = 0;

	for(auto& entry : tmp){
		if(excluded.contains(entry.name)){
			file.seek(entry.size, SeekMode::SeekCur);
			continue;
		}

		file.read(data + offset, entry.size);
		entry.offset = offset;

		offset += entry.size;

		entries.insert(std::make_pair(entry.name, std::move(entry)));
	}
}

FileArchive::~FileArchive(){
	if(!externalData){
		free(data);
	}
}

File FileArchive::get(const char* file, const char* name){
	auto it = entries.find(file);
	if(it == entries.end()) return File();

	if(!name){
		name = file;
	}

	auto f = std::make_shared<RamFile>(data + it->second.offset, it->second.size, name);
	return File(f);
}
