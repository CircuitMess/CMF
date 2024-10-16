#ifndef TEMPLATETYPES_H
#define TEMPLATETYPES_H

#include <source_location>
#include <string>
#include <vector>

/**
 * @brief A static class used by objects to determine the type information of their templated and generate the corresponding class ID based on the information of the templates.
 * This is used to distinguish same object types with different templates when casting.
 * @tparam Args The type of which the information is needed, can return the names of the arguments as well as their hash which is used for the class ID.
 */
template<typename... Args>
class TemplateTypesInfo {
public:
    /**
     * @return The names of the template types separated by ',' character.
     */
    static constexpr std::string TypeNames() {
        std::string name = std::source_location::current().function_name();
        const size_t first = name.find_first_of('{') + 1;
        const size_t last = name.find_last_of('}');
        name = name.substr(first, last - first);

        return name;
    }

    /**
     * @return The hash of the types. There is no limit on the number of arguments, however due to the limited nature of the DJB hashing, hash collisions may occur on edge cases.
     */
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

private:
    /**
     * @brief Default constructor is deleted to prevent creating instances of this purely static class.
     */
    TemplateTypesInfo() = delete;
};

#endif //TEMPLATETYPES_H
