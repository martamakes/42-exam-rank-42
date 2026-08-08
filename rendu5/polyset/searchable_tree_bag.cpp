#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() : tree_bag() {}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &other) : tree_bag(other) {}

searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag &other) {
	if (this != &other) {
		tree_bag::operator=(other);
	}
	return *this;
}

searchable_tree_bag::~searchable_tree_bag(){}

/*! PSEUDOCODE — has (BST search, relies on tree_bag::insert keeping it sorted):
	I-THINKING:
		we know that if the values smaller that the val we at we will find it on left
		and if val is bigger than the one we at we will find it on right
		we keep rept this intill we find value -> return bool
		if not find stop, risk for infinite loop ⚠️
	II- CODE:
		1. current = this->tree (root), NOT a double pointer — this
		   function is const, so we're only ever walking/reading, never
		   rewiring the tree
		2. while current is not NULL:
			 - if val == current->value  -> return true
			 - else if val >= current->value -> go right (current = current->r)
			 - else                          -> go left  (current = current->l)
		3. loop ends only when current becomes NULL (fell off a leaf) ->
		   return false — this is exactly what stops the "infinite loop"
		   risk from I-THINKING: every step strictly descends one level,
		   so the tree's finite height bounds the loop
*/

bool searchable_tree_bag::has(int val) const {
	node *current = this->tree; /*! @note we can't use double pointer (node ** n) like in @function: tree_bag::insert()
	↪ because this function is const (➤ I promise I will not modify any member variables of this class.)
	and &this->tree  would be able to change in tree.
	*/
	while (current)
	{
		if (val == current->value)
			return true;
		if (val >= current->value)
			current = current->r;
		else
			current = current->l;
	}
	return false;
}

