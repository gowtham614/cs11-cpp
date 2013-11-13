#include <cassert>

class SparseVector {

private:

  struct node
  {
      int   index;   // Element number, in the range [0, size)
      int   value;   // The value of this element.
      node *next;    // A pointer to the next node in the linked-list.

      // Node constructor - simply initializes the data-members.
      node(int index, int value, node *next = 0) : index(index), value(value), next(next){}
  };

  int  _size;
  node *_head;

  // helpers
  void cleanup();
  void copy(const SparseVector &sv);
  void setElemToZero(int col);
  void setElemToNoneZero(int col, int value);

public:
  // Constructors
  SparseVector();                   
  SparseVector(int size); 
  SparseVector(const SparseVector &sv);  

  // Destructor
  ~SparseVector();

  // Mutator methods
  void setElem(int col, int value);

  // Accessor methods
  int getSize() const;
  int getElem(int col) const;

  // Arithmetic methods
  //void add(const SparseVector &sv);
  //void subtract(const SparseVector &sv);

  //bool equals(const SparseVector &sv) const;

  // Operator Overloading
  SparseVector & operator=( const SparseVector &sv);
  //SparseVector & operator+=(const SparseVector &sv);
  //SparseVector & operator-=(const SparseVector &sv);
  //const SparseVector operator+(const SparseVector &sv) const;
  //const SparseVector operator-(const SparseVector &sv) const;
  //bool operator==(const SparseVector &sv) const;
  //bool operator!=(const SparseVector &sv) const;
};
