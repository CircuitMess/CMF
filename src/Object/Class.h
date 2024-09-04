#ifndef CMF_CLASS_H
#define CMF_CLASS_H

#include <cinttypes>
#include <map>
#include <string>

class Object;

class ClassRegistry {
public:
	virtual ~ClassRegistry() noexcept = default;

	const class Class* getClass(uint64_t ID) const noexcept;
	void registerClass(const Class* cls) noexcept;

private:
	std::map<uint64_t, const Class*> classes;
};

class Class {
	friend class Object;

public:
	virtual ~Class() = default;

	virtual Object* createDefaultObject() const noexcept;

	uint64_t getID() const noexcept;

	template<typename Type>
	static inline constexpr bool implements() noexcept {
		return false;
	}

	static inline const Class* getClasByID(uint64_t ID) noexcept {
		if(registry == nullptr){
			return nullptr;
		}

		return registry->getClass(ID);
	}

	inline virtual constexpr std::string getName() const noexcept{
		return "Object";
	}

protected:
	static inline ClassRegistry* registry = nullptr;

protected:
	explicit Class(uint64_t ID) noexcept;

private:
	uint64_t classID;
};

#endif //CMF_CLASS_H