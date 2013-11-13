#include <cassert>
#include <stdexcept>
#include <iostream>

template <typename T, int size = 10> 
class Heap {

private:

  const int _max_elems;
  int _count;

  T _values [size];

  int LEFT_CHILD(int index) const { return (2 * (index) + 1); }
  int RIGHT_CHILD(int index) const { return (2 * (index) + 2); } 
  int PARENT(int index) const { return (((index) - 1) / 2); } 

  void sift_down(int index);
  void sift_up(int index);
  void swap_values(int i, int j);

  void sift_down_catch(int index);
  void sift_up_catch(int index);
  void swap_values_catch(int i, int j);

public: 

  // Constructors
  Heap() : _max_elems(size), _count(0)
  {
	for(int i=0 ; i < _max_elems ; ++i){
		_values[i] = T();
	}
  }

  // Destructor
  ~Heap() {}

  // Accessor methods
  T get_first_value();

  // Mutator methods
  void add_value(const T& newval);
};

template <typename T, int size>
T Heap<T, size>::get_first_value()
{
	if(_count <= 0)
		throw std::out_of_range ("heap is empty");

	T result = _values[0];
	_count--;

	if(_count > 0)
	{
		_values[0] = _values[_count];
		sift_down_catch(0);
	}
	
  	return result;
}

template <typename T, int size>
void Heap<T, size>::add_value(const T& newval)
{
	/* There needs to be room for one more element in the heap... */
	if(_count >= _max_elems)
		throw std::out_of_range ("heap is full");

	/* Add the new value to the end of the heap, then sift up. */

	int index = _count;
	_values[index] = newval;
	_count++;

	/* If the new value isn't at the root, sift up. */
	if (index > 0)
		sift_up_catch(index);
}


/*==================*/
/* HELPER FUNCTIONS */
/*==================*/


/*
 * Helper function to handle the onerous task of swapping two
 * values in the value array of a heap.
 */
template <typename T, int size>
void Heap<T, size>::swap_values_catch(int i, int j)
{
	try
	{
		swap_values(i, j);
	}
	catch (std::out_of_range& e)
	{
		std::cout << "Error : out_of_range [ " << e.what() << " ]" << std::endl;
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Error : invalid_argument [ " << e.what() << " ]" << std::endl;
	}	
}

template <typename T, int size>
void Heap<T, size>::swap_values(int i, int j)
{
	if(i < 0 || i >= _count)
		throw std::out_of_range ("i < 0 || i >= _count");
	if(j < 0 || j >= _count)
		throw std::out_of_range ("j < 0 || j >= _count");
	if(i == j)
		throw std::invalid_argument ("i == j");

	T tmp = _values[i];
	_values[i] = _values[j];
	_values[j] = tmp;
}

/*
 * Given a heap and an index, sift_up checks to see if the value
 * at that index needs to be "sifted upward" in the heap, to
 * preserve the heap properties.  Specifically, a value needs to
 * be moved up in the heap if it is less than its parent value.
 * (This is just the "order" property; the "shape" property is
 * not affected by sifting a value up.)
 */
template <typename T, int size>
void Heap<T, size>::sift_up_catch(int index)
{
	try
	{
		sift_up(index);
	}
	catch (std::out_of_range& e)
	{
		std::cout << "Error : out_of_range [ " << e.what() << " ]" << std::endl;
	}
	catch (std::logic_error& e)
	{
		std::cout << "Error : logic_error [ " << e.what() << " ]" << std::endl;
	}
}

template <typename T, int size>
void Heap<T, size>::sift_up(int index)
{
	int parent_index = PARENT(index);

	/* If the index to sift up is the root, we are done. */
	if (index == 0)
		return;
  
	if(parent_index < 0)
		throw std::out_of_range ("parent_index < 0");
	if(parent_index == index) /* Parent of index 0 = 0... that's bad. */
		throw std::logic_error ("parent_index == index");

  	/* If the specified value is smaller than its parent value then
   	* we have to swap the value and its parent.
   	*/
  	if (_values[index] < _values[parent_index])
  	{
    		/* Swap the value with its parent value. */
		swap_values_catch(index, parent_index);

    		/* If we haven't gotten to the root, we might have to
     		* sift up again.
     		*/
    		if (parent_index != 0)
			sift_up_catch(parent_index);	
  	}
}

/*
 * Given a heap and an index, sift_down checks to see if the value
 * at that index needs to be "sifted downward" in the heap, to
 * preserve the heap properties.  Specifically, a value needs to
 * be moved down in the heap if it is greater than either of its
 * children's values.  (This is the "order" property.)  In order
 * to preserve the "shape" property of heaps, the value is swapped
 * with the *smaller* of its two child values.
 *
 * If a value only has a left child, then only the left child is
 * examined for the swap.
 *
 * If a value has both left and right children, it is possible
 * that one child may be larger than the value, while the other is
 * smaller than the value.  Since we swap with the smallest child
 * value, we preserve the heap properties even in that situation.
 */
template <typename T, int size>
void Heap<T, size>::sift_down_catch(int index)
{
	try
	{
		sift_down(index);
	}
	catch (std::out_of_range& e)
	{
		std::cout << "Error : out_of_range [ " << e.what() << " ]" << std::endl;
	}
}

template <typename T, int size>
void Heap<T, size>::sift_down(int index)
{
	if(index >= _count)
		throw std::out_of_range ("index >= _count");
	if(index < 0)
		throw std::out_of_range ("index < 0");

  	int left_child = LEFT_CHILD(index);
  	int right_child = RIGHT_CHILD(index);

  	if (left_child >= _count)
  	{
    		/* If the left child's index is past the end of the heap
    		 * then this value has no children.  We're done.
     		 */
    		return;
  	}

  	if (right_child >= _count)
  	{
    		/* Only have a left child. */

    		if (_values[left_child] < _values[index])
    		{
      			/* Left child value is smaller.  Swap this value and the
      			 * left child value.
      			 */
      			swap_values_catch(index, left_child);

      			/* Don't need to call sift_down again, because if this
       			 * node only has a left child, we are at the bottom of
       			 * the heap.
       			 */
    		}
  	}
  	else
  	{
    		/* This value has a left and right child. */

    		T left_val  = _values[left_child];
    		T right_val = _values[right_child];
    		int swap_child;

    		if (left_val < _values[index] || right_val < _values[index])
    		{
		      /* Need to swap this node with one of its children.  Pick
		       * the smaller of the two children, since this is a min-heap
		       * and that will preserve the heap properties.
		       */
      			if (left_val < right_val)
        			swap_child = left_child;
      			else
        			swap_child = right_child;

		      /* Do the swap, then call sift_down again, in case we aren't
		       * at the bottom of the heap yet.
		       */
		     	swap_values_catch(index, swap_child);
		      	sift_down_catch(swap_child);
		}
  	}
}



