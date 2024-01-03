#ifndef CMF_CLASS_H
#define CMF_CLASS_H

#include <cinttypes>

class Class {
	friend class Object;

public:
	uint32_t GetID() const noexcept;

	template<typename Type>
	static inline constexpr bool Derives() noexcept {
		return false;
	}

protected:
	explicit Class(uint32_t ID) noexcept;

private:
	uint32_t classID;
};

#endif //CMF_CLASS_H