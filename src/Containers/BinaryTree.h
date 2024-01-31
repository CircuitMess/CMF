#ifndef CMF_BINARYTREE_H
#define CMF_BINARYTREE_H

#include <set>
#include <queue>
#include <cstddef>
#include <mutex>
#include "Util/stdafx.h"

// TODO: iterators (const, non const) ?maybe?

template<typename K, typename V>
class BinaryTree {
public:
	inline BinaryTree() noexcept = default;

	inline BinaryTree(const BinaryTree& other) noexcept {
		std::lock_guard guard(accessMutex);
		*this = other;
	}

	inline BinaryTree(BinaryTree&& other) noexcept {
		std::lock_guard guard(accessMutex);
		std::lock_guard guardOther(other.accessMutex);
		*this = std::move(other);
	}

	inline virtual ~BinaryTree() noexcept {
		destruct();
	}

	inline BinaryTree& operator = (const BinaryTree& other) noexcept {
		std::lock_guard guard(accessMutex);

		if(&other == this){
			return *this;
		}

		std::set<K> keys = other.keys();

		for(const K& key : keys){
			set(key, other[key]);
		}

		return *this;
	}

	inline BinaryTree& operator = (BinaryTree&& other) noexcept {
		if(&other == this){
			return *this;
		}

		destruct();

		std::lock_guard guard(accessMutex);
		std::lock_guard guardOther(other.accessMutex);

		head = other.head;
		start = other.start;
		end = other.end;

		other.head = nullptr;
		other.start = nullptr;
		other.end = nullptr;

		return *this;
	}

	inline bool operator == (const BinaryTree& other) const noexcept {
		return other.head == head;
	}

	inline V& operator [] (const K& key) noexcept {
		V* val = get(key);

		if(val == nullptr){
			set(key);
			val = get(key);
		}

		return *val;
	}

	inline void set(const K& key, const V& value = V()) noexcept {
		std::lock_guard guard(accessMutex);

		if(head == nullptr){
			head = new Node();
			head->key = key;
			head->value = value;

			start = end = head;
			return;
		}

		bool updateStart = true; // Changes to false as soon as we go right through the tree
		bool updateEnd = true; // Changes to false as soon as we go left through the tree

		Node* newNode = nullptr;

		for(Node* current = head; current != nullptr; ){
			Node** next = nullptr;

			if(key == current->key){
				current->value = value;
				return;
			}else if(key > current->key){
				next = &current->rightNode;
				updateStart = false;
			}else if(key < current->key){
				next = &current->leftNode;
				updateEnd = false;
			}

			if(next == nullptr){
				// TODO error, this should not be possible!?
				return;
			}

			if(*next == nullptr){
				*next = newNode = new Node();
				newNode->key = key;
				newNode->value = value;
				newNode->parent = current;
				break;
			}

			current = *next;
		}

		if(newNode == nullptr){
			return;
		}

		if(updateEnd){
			end = newNode;
		}else if(updateStart){
			start = newNode;
		}
	}

	inline void remove(const K& key) noexcept {
		std::lock_guard guard(accessMutex);

		Node** foundNode = nullptr;
		Node** replacementNode = nullptr;

		for(Node** current = &head; *current != nullptr; ){
			if(key == (*current)->key){
				foundNode = current;
				break;
			}else if(key > (*current)->key){
				current = &(*current)->rightNode;
			}else if(key < (*current)->key){
				current = &(*current)->leftNode;
			}
		}

		if(foundNode == nullptr){
			return;
		}

		if(*foundNode == nullptr){
			return;
		}

		if((*foundNode)->rightNode != nullptr){
			for(replacementNode = &(*foundNode)->rightNode; (*replacementNode)->leftNode != nullptr; replacementNode = &(*replacementNode)->leftNode);
		}else if((*foundNode)->leftNode != nullptr){
			for(replacementNode = &(*foundNode)->leftNode; (*replacementNode)->rightNode != nullptr; replacementNode = &(*replacementNode)->rightNode);
		}else{
			if(start == *foundNode){
				start = (*foundNode)->parent;
			}

			if(end == *foundNode){
				end = (*foundNode)->parent;
			}

			if(head == *foundNode){
				head = nullptr;
			}

			delete *foundNode;
			*foundNode = nullptr;
			return;
		}

		if(replacementNode == nullptr){
			return;
		}

		if(*replacementNode == nullptr){
			return;
		}

		std::swap((*foundNode)->key, (*replacementNode)->key);
		std::swap((*foundNode)->value, (*replacementNode)->value);

		delete *replacementNode;
		*replacementNode = nullptr;
	}

	inline bool contains(const K& key) const noexcept {
		std::set<K> treeKeys = keys();
		if(treeKeys.empty()){
			return false;
		}

		return treeKeys.contains(key);
	}

	inline size_t size() const noexcept {
		return keys().size();
	}

	inline std::set<K> keys() noexcept {
		std::lock_guard guard(accessMutex);

		std::set<K> keys;

		if(head == nullptr){
			return keys;
		}

		std::queue<Node*> nodeQueue;
		nodeQueue.push(head);

		for(const Node* current = nodeQueue.front(); true; ){
			nodeQueue.pop();

			if(current == nullptr){
				continue;
			}

			if(current->leftNode != nullptr){
				nodeQueue.push(current->leftNode);
			}

			if(current->rightNode != nullptr){
				nodeQueue.push(current->rightNode);
			}

			keys.insert(current->key);

			if(nodeQueue.empty()){
				break;
			}

			current = nodeQueue.front();
		}

		return keys;
	}

	inline std::set<K> keys() const noexcept {
		std::set<K> keys;

		if(head == nullptr){
			return keys;
		}

		std::queue<Node*> nodeQueue;
		nodeQueue.push(head);

		for(const Node* current = nodeQueue.front(); true; ){
			nodeQueue.pop();

			if(current == nullptr){
				continue;
			}

			if(current->leftNode != nullptr){
				nodeQueue.push(current->leftNode);
			}

			if(current->rightNode != nullptr){
				nodeQueue.push(current->rightNode);
			}

			keys.insert(current->key);

			if(nodeQueue.empty()){
				break;
			}

			current = nodeQueue.front();
		}

		return keys;
	}

private:
	struct Node {
		K key = K();
		V value = V();
		Node* parent = nullptr;
		Node* leftNode = nullptr;
		Node* rightNode = nullptr;
	};

private:
	Node* head = nullptr;

	// These two will serve well for iterating through the tree, it will always be sorted while iterating, and provides faster start of iteration in either direction
	Node* start = nullptr;
	Node* end = nullptr;

	std::mutex accessMutex;

private:
	inline V* get(const K& key) noexcept {
		std::lock_guard guard(accessMutex);

		if(head == nullptr){
			return nullptr;
		}

		for(Node* current = head; current != nullptr; ){
			if(key == current->key){
				return &current->value;
			}else if(key > current->key){
				current = current->rightNode;
			}else if(key < current->key){
				current = current->leftNode;
			}
		}

		return nullptr;
	}

	inline void destruct() noexcept {
		std::lock_guard guard(accessMutex);

		std::queue<Node*> nodeQueue;
		nodeQueue.push(head);

		for(const Node* current = nodeQueue.front(); true; ){
			nodeQueue.pop();

			if(current == nullptr){
				continue;
			}

			if(current->leftNode != nullptr){
				nodeQueue.push(current->leftNode);
			}

			if(current->rightNode != nullptr){
				nodeQueue.push(current->rightNode);
			}

			delete current;

			if(nodeQueue.empty()){
				break;
			}

			current = nodeQueue.front();
		}

		head = nullptr;
		start = nullptr;
		end = nullptr;
	}
};

#endif //CMF_BINARYTREE_H