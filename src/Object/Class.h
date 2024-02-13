#ifndef CMF_CLASS_H
#define CMF_CLASS_H

#include <cinttypes>
#include <map>

class Object;

class ClassRegistry {
public:
	virtual ~ClassRegistry() noexcept = default;

	const class Class* getClass(uint32_t ID) const noexcept;
	void registerClass(const Class* cls) noexcept;

private:
	std::map<uint32_t, const Class*> classes;
};

class Class {
	friend class Object;

public:
	virtual ~Class() = default;

	virtual Object* createDefaultObject() const noexcept;

	uint32_t getID() const noexcept;

	template<typename Type>
	static inline constexpr bool implements() noexcept {
		return false;
	}

	static inline const Class* getClasByID(uint32_t ID) noexcept {
		if(registry == nullptr){
			return nullptr;
		}

		return registry->getClass(ID);
	}

protected:
	static inline ClassRegistry* registry = nullptr;

protected:
	explicit Class(uint32_t ID) noexcept;

	inline virtual constexpr const char* getName() const noexcept{
		return "Object";
	}

private:
	uint32_t classID;
};

#endif //CMF_CLASS_H