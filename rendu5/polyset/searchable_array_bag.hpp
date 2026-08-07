#ifndef _SEARCHABLE_ARRAY_BAG_HPP_
#define _SEARCHABLE_ARRAY_BAG_HPP_

#include "array_bag.hpp"
#include "searchable_bag.hpp"

class searchable_array_bag : public array_bag, public searchable_bag
{
	private:
		
	public:
		searchable_array_bag( void );
		searchable_array_bag(const searchable_array_bag& other);
		searchable_array_bag& operator=(const searchable_array_bag& other);
		~searchable_array_bag();

		bool has(int) const;
};
	
#endif