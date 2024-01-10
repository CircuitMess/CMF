#ifndef CMF_OBJECTMANAGER_H
#define CMF_OBJECTMANAGER_H

#include <functional>
#include "Containers/BinaryTree.h"

class Object;

class ObjectManager {
public:
	static ObjectManager* get() noexcept;

public:
	virtual ~ObjectManager() noexcept = default;

	uint32_t getReferenceCount(Object* object) const noexcept;
	bool isValid(Object* object) const noexcept;

	void registerReference(Object** object, bool keepAlive = false) noexcept;
	void unregisterReference(Object** object, bool keepAlive = false) noexcept;

	void forEachObject(const std::function<bool(Object*)>& fn) const noexcept;

private:
	struct ObjectRefInfo {
		uint32_t count = 0;
		std::set<Object**> objectPointers;
	};

private:
	BinaryTree<Object*, ObjectRefInfo> objectReferenceTree;
};

#endif //CMF_OBJECTMANAGER_H