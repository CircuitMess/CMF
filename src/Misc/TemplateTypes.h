#ifndef TEMPLATETYPES_H
#define TEMPLATETYPES_H

#include <source_location>
#include <string>
#include <vector>

template<typename... Args>
class TemplateTypesInfo {
public:
    static constexpr std::string TypeNames() {
        std::string name = std::source_location::current().function_name();
        const size_t first = name.find_first_of('{') + 1;
        const size_t last = name.find_last_of('}');
        name = name.substr(first, last - first);

        return name;
    }

    static constexpr uint32_t TypesHash() {
        std::vector<std::string> substrings;
        std::string names = TypeNames();

        while(names.size() > 32) {
            substrings.emplace_back(names.substr(0, 32));
            names.erase(0, 32);
        }

        substrings.emplace_back(names);

        std::vector<uint32_t> hashes;
        for(const std::string& substring : substrings) {
            hashes.emplace_back(STD_STRING_HASH(substring));
        }

        while(hashes.size() > 1) {
            std::string newHashStr;

            for(size_t i = 0; i < std::min(hashes.size(), static_cast<size_t>(3)); ++i) {
                newHashStr += std::to_string(hashes[i]);
            }

            hashes.erase(hashes.begin(), hashes.begin() + std::min(hashes.size(), static_cast<size_t>(3)) + 1);
            hashes.emplace_back(STD_STRING_HASH(newHashStr));
        }

        return hashes.front();
    }
};

#endif //TEMPLATETYPES_H
