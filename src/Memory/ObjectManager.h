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

	uint32_t getReferenceCount(const Object* object) noexcept;
	bool isValid(const Object* object) noexcept;

	void registerReference(Object** object, bool keepAlive = false) noexcept;
	void unregisterReference(Object** object, bool keepAlive = false) noexcept;

	void forEachObject(const std::function<bool(Object*)>& fn) const noexcept;

	void onObjectDeleted(Object* object) noexcept;

private:
	struct ObjectRefInfo {
		uint32_t count = 0;
		std::set<Object**> objectPointers;
	};

private:
	BinaryTree<Object*, ObjectRefInfo> objectReferenceTree;
};

#endif //CMF_OBJECTMANAGER_H