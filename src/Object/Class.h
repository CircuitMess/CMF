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

public:
	uint32_t getID() const noexcept;

	template<typename Type>
	static inline constexpr bool implements() noexcept {
		return false;
	}

protected:
	static inline ClassRegistry* registry = nullptr;

protected:
	explicit Class(uint32_t ID) noexcept;

private:
	uint32_t classID;
};

#endif //CMF_CLASS_H