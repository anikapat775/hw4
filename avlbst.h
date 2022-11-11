#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* insert (const std::pair<const Key, Value> &new_item, AVLNode<Key, Value>* n, AVLNode<Key, Value>* parent);
		int getHeight (AVLNode<Key, Value>* n);
		int getDifference (AVLNode<Key, Value>* n);
		AVLNode<Key, Value>* rightRotate(AVLNode<Key, Value>* n);
		AVLNode<Key, Value>* leftRotate(AVLNode<Key, Value>* n);
		AVLNode<Key, Value>* balance(AVLNode<Key, Value>* n);
		void updateBalance(AVLNode<Key, Value> * n);

		AVLNode<Key, Value> * remove(const Key& key, AVLNode<Key, Value>* n); 
		AVLNode<Key, Value> * pred(AVLNode<Key, Value>* current);
};
template<class Key, class Value>
int AVLTree<Key, Value>::getDifference (AVLNode<Key, Value>* n){
	//calculate left height minus right height
	int difference = getHeight(n->getLeft())-getHeight(n->getRight());
	return difference;
}
template<class Key, class Value>
int AVLTree<Key, Value>::getHeight (AVLNode<Key, Value>* n){
	if(n==NULL){
		return -1;
	}
	else{
		return n->getBalance();
	}
}
template<class Key, class Value>
void AVLTree<Key, Value>::updateBalance(AVLNode<Key, Value> * n){
	if(n!=NULL){
		//get height of subtrees & add one to the larger height
		int height = getHeight(n->getLeft());
		int rightHeight = getHeight(n->getRight());
		if(rightHeight>height){
			height = rightHeight;
		}
		n->setBalance(1+height);
	}
}
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value>* n){
	//store left subtree 
	AVLNode<Key, Value> * temp = n->getLeft();
	//set left subtree to right subtree
	n->setLeft(temp->getRight());
	if(temp->getRight() != NULL){
		//update parent
		temp->getRight()->setParent(n);
	}
	//update new parent node
	temp->setRight(n);
	temp->setParent(n->getParent());
	n->setParent(temp);
	//update parents left or right, depending on key
	if(temp->getParent()!= NULL && n->getKey()<temp->getParent()->getKey()){
		temp->getParent()->setLeft(temp);
	}
	else if(temp->getParent()!=NULL){
		temp->getParent()->setRight(temp);
	}
	n = temp;
	updateBalance(n->getLeft());
	updateBalance(n->getRight());
	updateBalance(n);
	updateBalance(n->getParent());
	return n; 
}
template<class Key, class Value>
	AVLNode<Key, Value>* AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value>* n){
		//store right subtree 
	AVLNode<Key, Value> * temp = n->getRight();
	//set right subtree to left subtree
	n->setRight(temp->getLeft());
	if(temp->getLeft() != NULL){
		//update parent
		temp->getLeft()->setParent(n);
	}
	//update new parent node
	temp->setLeft(n);
	temp->setParent(n->getParent());
	n->setParent(temp);
	//update parents left or right, depending on key
	if(temp->getParent()!= NULL && n->getKey()<temp->getParent()->getKey()){
		temp->getParent()->setLeft(temp);
	}
	else if(temp->getParent()!=NULL){
		temp->getParent()->setRight(temp);
	}
	n = temp;
	updateBalance(n->getLeft());
	updateBalance(n->getRight());
	updateBalance(n);
	updateBalance(n->getParent());
	return n; 
}
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::balance(AVLNode<Key, Value>* n){
	int difference = getDifference(n);
	if(difference>1){
		if(getDifference(n->getLeft())>=0){
			//right rotate
			n = rightRotate(n);
		}
		else{
			//left-right rotate
			n->setLeft(leftRotate(n->getLeft()));
			n = rightRotate(n);
		}
	}
	else if(difference<-1){
		if(getDifference(n->getRight())>0){
			//right-left rotate
			n->setRight(rightRotate(n->getRight()));
			n = leftRotate(n);
		}
		else{
			//left-rotate
			n = leftRotate(n);
		}
	}
	return n;
}
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */ 
template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item, AVLNode<Key, Value>* n, AVLNode<Key, Value>* parent) {
    if(n ==NULL){
        n = new AVLNode<Key,Value>(new_item.first, new_item.second, parent);
        return n; 
    }
    else if(n->getKey() > new_item.first){
			//insert into left subtree & update balance
			n->setLeft(insert(new_item, n->getLeft(), n));
			n = balance(n);
			updateBalance(n);
		}
		else if(n->getKey()< new_item.first){
			//insert into right subtree & update balance
			n->setRight(insert(new_item, n->getRight(), n));
			n = balance(n);
			updateBalance(n);
		}
		else{
			//override key with value
			n->setValue(new_item.second);
		}
		return n;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
		this->root_ = insert(new_item, (AVLNode<Key,Value>*)this->root_, NULL);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
AVLNode<Key, Value> * AVLTree<Key, Value>::remove(const Key& key, AVLNode<Key, Value>* n){
		if(n == NULL){
			return NULL;
		}
		else if(key>n->getKey()){
			//remove from right subtree & update balance
			n->setRight(remove(key, n->getRight()));
			updateBalance(n);
			updateBalance(n->getParent());
			n = balance(n);
			updateBalance(n);
			updateBalance(n->getParent());
		}
		else if(key<n->getKey()){
			//remove from left subtree & update balance
			n->setLeft(remove(key, n->getLeft()));
			updateBalance(n);
			updateBalance(n->getParent());
			n = balance(n);
			updateBalance(n);
			updateBalance(n->getParent());
		}
		else{
			//if no children, just delete
			if(n->getLeft()== NULL && n->getRight()==NULL){
				delete n;
				n = NULL;
				return n;
			}
			//if just right child, set node to right tree and balance
			else if(n->getRight()!= NULL && n->getLeft()==NULL){
				AVLNode<Key,Value>* temp = n;
				n = n->getRight();
				n->setParent(temp->getParent());
				updateBalance(n);
				updateBalance(n->getParent());
				delete temp;
				return n;
			}
			//if just left child, set node to left tree and balance
			else if(n->getRight()== NULL && n->getLeft()!=NULL){
				AVLNode<Key,Value>* temp = n;
				n = n->getLeft();
				n->setParent(temp->getParent());
				updateBalance(n);
				updateBalance(n->getParent());
				delete temp;
				return n;
			}
			//if two children, swap with predecessor and call remove on left subtree
			else{
				AVLNode<Key, Value> * temp = pred(n);
				AVLTree<Key,Value>::nodeSwap(n, temp);
				temp->setLeft(remove(key, temp->getLeft()));
				temp = balance(temp);
				updateBalance(temp);
				updateBalance(temp->getParent());
				return temp;
			}
		}
		updateBalance(n);
		updateBalance(n->getParent());
		return n;
}

template<class Key, class Value>
AVLNode<Key, Value>*
AVLTree<Key, Value>::pred(AVLNode<Key, Value>* current)
{
    // TODO
    if(current ==NULL){
        return NULL;
    }
    current = current->getLeft();
    while(current->getRight()!=NULL){
        current = current->getRight();
    }
    return current; 
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
		this->root_ = remove(key, (AVLNode<Key,Value>*)this->root_);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
