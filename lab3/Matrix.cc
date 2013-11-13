#include "Matrix.hh"

Matrix::Matrix() 
{
	_rows = 0;
	_cols = 0;
	_elems = new int[0];
}

Matrix::Matrix(const Matrix &m)
{
	this->copy(m);
}
                   
Matrix::Matrix(int r, int c)
{
	_rows = r;
  	_cols = c;

	int len = r*c;
 	_elems = new int[len];

	for(int i=0 ; i < len ; ++i){
		_elems[i] = 0;
	}
}

// helpers

void Matrix::cleanup()
{
	_rows = 0;
	_cols = 0;

	delete[] _elems;
}

void Matrix::copy(const Matrix &m)
{
	_rows = m.getrows();
  	_cols = m.getcols();
 	_elems = new int[_rows * _cols];

	int idx = 0;

	for(int i=0 ; i < _rows ; ++i){
		for(int j=0 ; j < _cols ; ++j){
			_elems[idx + j] = m.getelem(i,j);
		}
		idx += _cols; // idx = i * _cols
	}
}

// Destructor

Matrix::~Matrix() 
{
	this->cleanup();
}

// Mutators:

void Matrix::setelem(int r, int c, int val)
{
	assert(r < _rows);
	assert(c < _cols);

	_elems[(r * _cols) + c] = val;
}

// Accessors:

int Matrix::getrows() const
{
	return _rows;
}

int Matrix::getcols() const
{
	return _cols;
}

int Matrix::getelem(int r, int c) const
{
	assert(r < _rows);
	assert(c < _cols);

	return _elems[(r * _cols) + c];
}

// Arithmetic methods

void Matrix::add(const Matrix &m)
{
	assert(_rows == m.getrows());
	assert(_cols == m.getcols());

	int idx = 0;

	for(int i=0 ; i < _rows ; ++i){
		for(int j=0 ; j < _cols ; ++j){
			_elems[idx + j] += m.getelem(i,j);
		}
		idx += _cols; // idx = i * _cols
	}
}

void Matrix::subtract(const Matrix &m)
{
	assert(_rows == m.getrows());
	assert(_cols == m.getcols());

	int idx = 0;

	for(int i=0 ; i < _rows ; ++i){
		for(int j=0 ; j < _cols ; ++j){
			_elems[idx + j] -= m.getelem(i,j);
		}
		idx += _cols; // idx = i * _cols
	}
}

bool Matrix::equals(const Matrix &m) const
{
	if(( _rows != m.getrows() ) || ( _cols != m.getcols() ))
	{
		return false;
	}

	int idx = 0;

	for(int i=0 ; i < _rows ; ++i){
		for(int j=0 ; j < _cols ; ++j){
			if(_elems[idx + j] != m.getelem(i,j)){
				return false;
			}
		}
		idx += _cols; // idx = i * _cols
	}

	return true;
}

// Operator Overloading

Matrix & Matrix::operator=(const Matrix &m)
{
	if (this == &m){
      		return *this;
	}

	this->cleanup();
	this->copy(m);
	
	return *this;
}

Matrix & Matrix::operator+=(const Matrix &m)
{
	this->add(m);
	return *this;
}

Matrix & Matrix::operator-=(const Matrix &m)
{
	this->subtract(m);
	return *this;
}

Matrix & Matrix::operator*=(const Matrix &m)
{
	assert(_cols == m.getrows());

	Matrix result = Matrix(_rows, m.getcols());

	int val;

	for(int i=0 ; i < _rows ; ++i){
		for(int j=0 ; j < m.getcols() ; ++j){
			val = 0;
			for(int k=0 ; k < _cols ; ++k){
				val += ( this->getelem(i, k) * m.getelem(k, j) );
			}
			result.setelem(i, j, val);
		}
	}

	return ( *this = result );
}

const Matrix Matrix::operator+(const Matrix &m) const
{
	Matrix result = *this;
   	result += m; 
    	return result; 
}

const Matrix Matrix::operator-(const Matrix &m) const
{
	Matrix result = *this;
   	result -= m; 
    	return result; 
}

const Matrix Matrix::operator*(const Matrix &m) const
{
	Matrix result = *this;
   	result *= m; 
    	return result; 
}

bool Matrix::operator==(const Matrix &m) const
{
	return this->equals(m);
}

bool Matrix::operator!=(const Matrix &m) const
{
	return !(*this == m);
}





