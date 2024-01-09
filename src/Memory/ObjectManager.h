#ifndef CMF_OBJECTMANAGER_H
#define CMF_OBJECTMANAGER_H

#include "Containers/BinaryTree.h"
#include "Object/Object.h"

class ObjectManager {
public:
	virtual ~ObjectManager() noexcept = default;

	uint32_t getReferenceCount(Object* object) const noexcept;
	bool isValid(Object* object) const noexcept;
	void registerReference(Object* object) noexcept;
	void unregisterReference(Object* object) noexcept;

private:
	struct ObjectRefInfo {
		uint32_t count = 0;
		// std::set<ObjectPtr*> ptrList TODO: when pointers get created
	};

private:
	// TODO: instead of having register counters, have pointer list in a way that only strong pointers increase object ref counter, while all can be invalidated when destroyed
	BinaryTree<Object*, ObjectRefInfo> objectReferenceTree;
};

#endif //CMF_OBJECTMANAGER_H