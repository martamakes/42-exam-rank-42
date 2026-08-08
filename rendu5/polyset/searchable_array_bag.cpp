#include "searchable_array_bag.hpp"
#include <iostream>

searchable_array_bag::searchable_array_bag() : array_bag() {}

searchable_array_bag::searchable_array_bag(const searchable_array_bag& other) : array_bag(other) {}

searchable_array_bag &searchable_array_bag::operator=(const searchable_array_bag &other) {
	if (this != &other) {
		array_bag::operator=(other);
	}
	return *this;
}

searchable_array_bag::~searchable_array_bag() {}

/*! PSEUDOCODE — has (linear search, array_bag has no ordering guarantee):
 *  1. walk this->data[0 .. this->size) — both inherited from array_bag
 *  2. return true as soon as data[i] == val
 *  3. loop finishes without a match -> return false
 *  -> O(n): unlike the tree version, there's no order to exploit here
 */
bool searchable_array_bag::has(int val) const {

	for (int i = 0; i < this->size; i++) {
		if (this->data[i] == val)
			return true;
	}
	return false;
	
}
