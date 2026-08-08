#include "set.hpp"

set::set(searchable_bag &sb) : sb(&sb) {}

set::set(const set &other) : sb(other.sb) {}

set &set::operator=(const set &other) {
	if (this != &other){
		this->sb = other.sb;
	}
	return *this;
}

set::~set() {
    // A wrapper/view should not delete or clear the wrapped object.
    // The life of the bag is managed by whoever created it (in this case main).
}

/*! PSEUDOCODE — insert(val): the ONE place the "no duplicates" rule lives
 *  1. ask the wrapped bag if val is already there: sb->has(val)
 *  2. only if it's NOT there, sb->insert(val)
 *  -> this single check is what turns a plain bag (allows duplicates)
 *     into a set (doesn't)
 */
void set::insert(int val) {
	if (!this->sb->has(val))
		this->sb->insert(val);
}

/*! PSEUDOCODE — insert(array, size): bulk insert, reuse the rule above
 *  1. guard against a NULL array or a non-positive size -> just return
 *  2. loop i in [0, size) and call this->insert(array[i]) for each one
 *     (delegate to the single-value version instead of duplicating the
 *     has()-then-insert() check)
 */
void set::insert(int *array, int size) {
	if (!array || size <= 0)
		return;
	for (int i = 0; i < size; i++) {
		this->insert(array[i]);
	}
}

void set::print() const {
	this->sb->print();
}

void set::clear() {
	this->sb->clear();
}

bool set::has(int val) const {
	return this->sb->has(val);
}

searchable_bag &set::get_bag() const {
	return *(this->sb);
}
