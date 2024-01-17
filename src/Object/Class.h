#ifndef CMF_CLASS_H
#define CMF_CLASS_H

#include <cinttypes>

class Object;

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
	explicit Class(uint32_t ID) noexcept;

private:
	uint32_t classID;
};

#endif //CMF_CLASS_H