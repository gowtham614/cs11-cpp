#include <cassert>

class Matrix {

private:
  int  _rows;
  int  _cols;
  int *_elems;

  // helpers
  void cleanup();
  void copy(const Matrix &m);

public:
  // Constructors
  Matrix();   
  Matrix(const Matrix &m);                   
  Matrix(int r, int c);   

  // Destructor
  ~Matrix();

  // Mutator methods
  void setelem(int r, int c, int val);

  // Accessor methods
  int getrows() const;
  int getcols() const;
  int getelem(int r, int c) const;

  // Arithmetic methods
  void add(const Matrix &m);
  void subtract(const Matrix &m);

  bool equals(const Matrix &m) const;

  // Operator Overloading
  Matrix & operator=(const Matrix &m);
  Matrix & operator+=(const Matrix &m);
  Matrix & operator-=(const Matrix &m);
  Matrix & operator*=(const Matrix &m);
  const Matrix operator+(const Matrix &m) const;
  const Matrix operator-(const Matrix &m) const;
  const Matrix operator*(const Matrix &m) const;
  bool operator==(const Matrix &m) const;
  bool operator!=(const Matrix &m) const;
};


