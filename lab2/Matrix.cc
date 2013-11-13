#include "Matrix.hh"

Matrix::Matrix() 
{
	_rows = 0;
	_cols = 0;
	_elems = new int[0];
}

Matrix::Matrix(Matrix &m)
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

// Destructor
Matrix::~Matrix() 
{
	delete[] _elems;
}

// Mutators:

void Matrix::setelem(int r, int c, int val)
{
	assert(r < _rows);
	assert(c < _cols);

	_elems[(r * _cols) + c] = val;
}

// Accessors:

int Matrix::getrows()
{
	return _rows;
}

int Matrix::getcols()
{
	return _cols;
}

int Matrix::getelem(int r, int c)
{
	assert(r < _rows);
	assert(c < _cols);

	return _elems[(r * _cols) + c];
}

// Arithmetic methods

void Matrix::add(Matrix &m)
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

void Matrix::subtract(Matrix &m)
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

bool Matrix::equals(Matrix &m)
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





