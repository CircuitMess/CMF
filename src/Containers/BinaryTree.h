#ifndef CMF_BINARYTREE_H
#define CMF_BINARYTREE_H

#include <concepts>

template<typename K, typename V>
class BinaryTree {
public:
	BinaryTree() noexcept {

	}

	BinaryTree(const BinaryTree& other) noexcept {

	}

	BinaryTree(BinaryTree&& other) noexcept {

	}

	virtual ~BinaryTree() noexcept {

	}

	void add(const K& key, const V& value = V()) noexcept {

	}

	void remove(const K& key) noexcept {

	}

	V* get(const K& key) const noexcept {

	}

private:
	struct Node {
		K key;
		V value;
	};

private:
	Node* head = nullptr;

	// These two will serve well for iterating through the tree, it will always be sorted while iterating, and provides faster start of iteration in either direction
	Node* start = nullptr;
	Node* end = nullptr;
};

#endif //CMF_BINARYTREE_H