#ifndef SET_HPP
#define SET_HPP

#include "searchable_bag.hpp"


/*! PSEUDOCODE — set (wrapper, not a container of its own):
 *  1. `set` does NOT store values itself — it holds a `searchable_bag *`
 *     (either an array_bag or a tree_bag, chosen by whoever builds it)
 *  2. every "set" behavior is just "check with has() first, then
 *     delegate to the wrapped bag":
 *       - insert(val): if (!sb->has(val)) sb->insert(val)  <- the ONLY
 *         place the "no duplicates" rule actually lives
 *       - insert(array, size): loop and call insert(val) one by one —
 *         reuse the rule above instead of duplicating it
 *       - has/print/clear: just forward to sb->has/print/clear
 *  3. no default constructor on purpose: a set with no wrapped bag has
 *     no valid state, so the only constructor takes a searchable_bag&
 *  4. copy ctor / operator= copy the POINTER (sb), not the bag itself —
 *     a set is a VIEW over someone else's bag, so the destructor must
 *     NOT delete or clear it (whoever created the bag owns its lifetime)
 */

/*! @brief A wrapper class that turns a searchable_bag into a set (no duplicates). */
class set 
{
	private:
		searchable_bag *sb; /*!< Underlying storage bag. */
	public:
		/*! @note No default constructor on purpose: a set with no wrapped
		    bag has no valid state to be in, so we never let it exist.
		    Declaring the constructor below is enough to stop the
		    compiler from generating an implicit default one. */
		/*! @brief Constructor with an existing bag.
		    @param sb Reference to a searchable_bag to wrap. */
		set(searchable_bag &sb);
		
		/*! @note in main GIVEN: a set abject take in it costructor a searchable_bag & */
		/*! @brief Copy constructor. */
		set(const set &other);
		
		/*! @brief Assignment operator. */
		set &operator=(const set &other);
		
		/*! @brief Destructor. */
		~set();

	/* The Wrapper Functions: 
    These functions mimic the interface of the underlying bag (array or tree).
    The 'Set' handles the filtering logic (checking for duplicates), 
    and then delegates the actual storing task to the internal 'Bag'.
*/
		/*! @brief Inserts a value only if it doesn't already exist. */
		void insert(int val);
		
		/*! @brief Inserts an array of values, filtering duplicates. */
        void insert(int *array, int size);
        
		/*! @brief Prints the contents of the underlying bag. */
		void print() const;
		
		/*! @brief Clears the underlying bag. */
        void clear();
        
		/*! @brief Checks if a value exists in the set. */
		bool has(int val) const;

		/*! @note in main GIVEN: we call a getter so we need to impliment it */
		/*! @brief Returns a reference to the underlying bag. Always valid:
		    a set always wraps a bag (no default constructor exists). */
		searchable_bag &get_bag() const;
};
	
#endif
