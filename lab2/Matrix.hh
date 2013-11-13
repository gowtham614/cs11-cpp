#include <cassert>

class Matrix {

private:
  int  _rows;
  int  _cols;
  int *_elems;

public:
  // Constructors
  Matrix();   
  Matrix(Matrix &m);                   
  Matrix(int r, int c);   

  // Destructor
  ~Matrix();

  // Mutator methods
  void setelem(int r, int c, int val);

  // Accessor methods
  int getrows();
  int getcols();
  int getelem(int r, int c);

  // Arithmetic methods
  void add(Matrix &m);
  void subtract(Matrix &m);

  bool equals(Matrix &m);
};


