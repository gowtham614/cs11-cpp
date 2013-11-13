#include "SparseVector.hh"

SparseVector::SparseVector() 
{
	_size = 0;
}

SparseVector::SparseVector(int size)
{
	_size = size;
	_head = 0;
}
                   
SparseVector::SparseVector(const SparseVector &sv)
{
	this->copy(sv);
}

// helpers

void SparseVector::cleanup()
{
	_size = 0;

	node* curr = _head;

	while(curr != 0)
	{
		node* tmp = curr;
		curr = curr->next;
		delete tmp;
	}
}

void SparseVector::copy(const SparseVector &sv)
{
	_size = sv.getSize();
	_head = 0;

	node* curr = 0;
	node* curr_sv = sv._head;

	while(curr_sv != 0)
	{
		if(curr == 0){
			_head = new node(curr_sv->index, curr_sv->value);
			curr = _head;
		}
		else{
			curr->next = new node(curr_sv->index, curr_sv->value);
			curr = curr->next;
		}

		curr_sv = curr_sv->next;
	}
}

// Destructor

SparseVector::~SparseVector() 
{
	this->cleanup();
}

// Mutator methods

void SparseVector::setElemToZero(int col)
{
	assert(col < _size);

	if(_head == 0 || _head->index > col){
		return;
	}

	node* curr = _head;
	node* next = _head->next;

	int idx = curr->index;
	if(idx == col){
		_head = next;
		delete curr;
		return;
	}	

	while(next != 0)
	{
		idx = next->index;

		if(idx > col){ 
			break;
		}
		else if (idx == col){
			curr->next = next->next;
			delete next;
			break;
		}
		
		curr = next;
		next = curr->next;
	}
}

void SparseVector::setElemToNoneZero(int col, int value)
{
	assert(col < _size);
	
	if(_head == 0){
		_head = new node(col, value);
		return;
	}
	else if(_head->index > col){
		_head = new node(col, value, _head);
		assert(_head != _head->next);
		return;
	}

	node* curr = _head;
	node* next = _head->next;

	int idx = 0;	

	while(next != 0)
	{
		idx = next->index;

		if(idx > col){ 
			curr->next = new node(col, value, next);
			return;
		}
		else if (idx == col){
			next->value = value;
			return;
		}
		
		curr = next;
		next = curr->next;
	}

	curr->next = new node(col, value);
}

void SparseVector::setElem(int col, int value)
{
	if(value == 0){
		this->setElemToZero(col);
	}
	else{
		this->setElemToNoneZero(col, value);
	}
}

// Accessor methods

int SparseVector::getSize() const
{
	return _size;
}

int SparseVector::getElem(int col) const
{
	assert(col < _size);
	
	if(_head == 0 || _head->index > col){
		return 0;
	}

	node* curr = _head;
	int idx = 0;	

	while(curr != 0)
	{
		idx = curr->index;

		if(idx > col){ 
			return 0;
		}
		else if (idx == col){
			return curr->value;
		}
		
		curr = curr->next;
	}

	return 0;
}

// Operator Overloading

SparseVector & SparseVector::operator=( const SparseVector &sv)
{
	if (this == &sv){
      		return *this;
	}

	this->cleanup();
	this->copy(sv);
	
	return *this;
}





