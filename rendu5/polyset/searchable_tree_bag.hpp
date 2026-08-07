#ifndef _SEARCHABLE_TREE_BAG_HPP_
#define _SEARCHABLE_TREE_BAG_HPP_

#include "tree_bag.hpp"
#include "searchable_bag.hpp"

	class searchable_tree_bag : public tree_bag, public searchable_bag
	{
		private:
			
		public:
			searchable_tree_bag( void );
			searchable_tree_bag(const searchable_tree_bag& other);
			searchable_tree_bag& operator=(const searchable_tree_bag& other);
			~searchable_tree_bag();

			bool has(int) const;	
	};
	
#endif