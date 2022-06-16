/*
 * /File Blocklist.h 
 * /Brief File storing the blocklist class to store the tape 
 * /Author Shane Keane 17329836
 * /Date 03-06-2022
 * /Version 1.0
 * */

#include <array>
#include <list>
#include <iterator>
#include <cstring>

using namespace std;

template <class T, size_t block_size>
class blocklist {
	/* Container is a list of arrays */
	list<array<T, block_size>> data;

	/* Defining types for iterators */
	using list_iter = decltype(data.begin());
	using block_iter = decltype(data.back().begin());

	/* Current and last block */
	list_iter cur_block;
	list_iter last_block;
	/* Next and last free space in current block */
	block_iter next_space;
	block_iter last_space;
	/* Making marker iterators which can be used to rewind tape */
	list_iter marked_block;
	block_iter marked_space;
private: 
	/* @Brief Function to create a new array */
	void newblock() { 
		/* Create array */
		data.emplace_back(); /* Creates new slot and object at end of data */
		/* Reassigning iterators */
		cur_block = last_block = prev(data.end());
		next_space = cur_block->begin();
		last_space = cur_block->end();
	}
	/* @Brief Function to move to next array */
	void nextblock() {
		/* If at last block create a new block */
		if(cur_block == last_block){
			newblock();
		} else {
			/* Iterate if not at last block */
			++cur_block;
			next_space = cur_block->begin();
			last_space = cur_block->end();
		}
	}
public:
	/* @Brief Constructor */
	blocklist() {
		newblock();
	}
	/* @Brief Function to clear and reset */
	void clear() {
		data.clear();
		newblock();
	}

	/* @Brief Function to rewind the tape to start */
	void rewind() {
		cur_block = data.begin();
		next_space = cur_block->begin();
                last_space = cur_block->end();
	}
	/* @Brief Function to set marker at current space and block */
	void setmark() { 
		marked_block = cur_block;
		marked_space = next_space;
	}
	/* @Brief Function to rewind to mark previously set */
	void rewind_to_mark() {
		cur_block = marked_block;
                last_space = cur_block->end();
		next_space = marked_space;
	}
	/* @Brief Overloading emplace_back() to datatype T constructed */
	T* emplace_back() {
		/* Creating new block if at end of block */
		if(next_space = last_space) {
			nextblock();
		}	
		/* Storing next space */
		auto old_next = next_space;
		/* Iterating next space */
		++next_space;
		/* Returning old next space */
		return &*old_next;
	}

	/* @Brief Function to store multiple elements at the end of arraay */
	template <size_t n>
	T* emplace_back_multi() {
		/* Check if values will fit in array */
		if(distance(next_space, last_space) < n){
			nextblock();
		}
		/* Current space */
		auto old_next = next_space;
		/* Update next space */
		next_space += n;
		/* Return old current space */
		return &* old_next;
	}

	T* emplace_back_multi(const size_t n){
                /* Check if values will fit in array */
                if(distance(next_space, last_space) < n){
                        nextblock();
                }
                /* Current space */
                auto old_next = next_space;
                /* Update next space */
                next_space += n;
                /* Return old current space */
                return &* old_next;
	}

	/* @Brief Function to reset bytes in array to 0 */
	void memset(unsigned char value = 0){
		for(auto& arr: data){
			std::memset(&arr[0], value, block_size*sizeof(T));
		}
	}

	/* @Brief Overloaded emplace_back function to take arguments it can't construct */
	template<typename ...Args>
	T* emplace_back(Args&& ...args) {
		/* Check if at end of block */
		if(next_space == last_space) {
			nextblock();
		}
		/* Construct in memory pointed to by next T* */
		T* emplaced = new (&*next_space) T(forward<Args> (args)...);
		/* Iterate next space */
		++next_space;
		/* Return new memory */
		return emplaced; 
	}

	/* @Brief Blocklist iterator for easier to work with objects */
	class iterator {
		/* Initialising some points of Blocklist */
		list_iter cur_block;
		block_iter cur_space;
		block_iter first_space;
		block_iter last_space;

		public: 
			using differece_type = ptrdiff_t;
			using reference = T&;
			using pointer = T*;
			using value_type = T;
			using iterator_category = bidirectional_iterator_tag;

			/* Constructors */
			iterator() {}  /* Default */
			iterator(list_iter cb, block_iter cs, block_iter fs, block_iter ls) : 
				cur_block(cb), cur_space(cs), first_space(fs), last_space(ls) {}

			/* @Brief overloading ++ operator */
			iterator& operator++() {
				/* If at end of block */
				if(cur_space == last_space) {
					/* Jump to next block */
					++cur_block;
					first_space = cur_block->begin();
					last_space = cur_block->end();
					cur_space = first_space;
				} else {
					++cur_space;
				}
				return *this;
			}
			/* @Brief overloading -- operator */
			iterator& operator--() {
				if(cur_space == first_space){
					--cur_block;
					first_space = cur_block->begin();
					last_space = cur_block->end();
					cur_space = last_space;
				} else {
					--cur_space;
				}
				return *this;
			}
			/* @Brief Dereferencing operators */
			T& operator*() {
				return *cur_space;
			}
			
			T* operator->() {
				return &*cur_space;
			}
			/* @Brief Equality operators */
			bool operator ==(const iterator& rhs) const{
				return (cur_block == rhs.cur_block && cur_space == rhs.cur_space);
			}

			bool operator !=(const iterator& rhs) const{
				return (cur_block != rhs.cur_block && cur_space != rhs.cur_space);
			}
	};

	    //  Access to iterators

    iterator begin()
    {
        return iterator(data.begin(), data.begin()->begin(),
            data.begin()->begin(), data.begin()->end());
    }

    iterator end()
    {
        return iterator(cur_block, next_space,
            cur_block->begin(), cur_block->end());
    }

    //  Iterator on mark
    iterator mark()
    {
        return iterator(marked_block, marked_space,
            marked_block->begin(), marked_block->end());
    }

    //  Find element, by pointer, searching sequentially from the end
    iterator find(const T* const element)
    {
        //	Search from the end
        iterator it = end();
        iterator b = begin();

        while (it != b)
        {
            --it;
            if (&*it == element) return it;
        }

        if (&*it == element) return it;

        return end();
    }
};


