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

// 

void SparseVector::arithmetic(Operation type,const SparseVector &sv)
{
	assert(_size == sv._size);

	int sign = 0;
	switch(type)
	{
		case ADD:
			sign =  1;
			break;
		case SUB:
			sign = -1;
			break;
		default:
			assert(false);
	}

	int val;

	node* prev = 0;
	node* curr = _head;
	node* currSv = sv._head;

	if((curr == 0 && currSv == 0) || (curr != 0 && currSv == 0)){
		return;
	}
	else if(curr == 0 && currSv != 0){
		_head = new node(currSv->index, ((sign) * currSv->value));
		curr = _head;
		currSv = currSv->next;
		
		while(currSv != 0)
		{
			curr->next = new node(currSv->index,((sign) * currSv->value));
			curr = curr->next;
			currSv = currSv->next;
		}

		return;	
	}

	while(curr != 0 && currSv != 0)
	{
		if(currSv->index == curr->index)
		{
			val = ((sign) * currSv->value) + curr->value;
			if(val == 0){
				if(prev == 0){
					node* tmp = curr;
					_head = curr->next;
					delete tmp;
				
					curr = _head;
					currSv = currSv->next;
				}
				else{
					node* tmp = curr;
					prev->next = curr->next;
					delete tmp;
				
					curr = prev->next;
					currSv = currSv->next;
				}
			}
			else{
				curr->value = val;
				prev = curr;
				curr = curr->next;
				currSv = currSv->next;
			}
		}

		while(currSv != 0 && curr != 0 && currSv->index < curr->index )
		{
			if(prev == 0){
				_head = new node(currSv->index, ((sign) * currSv->value), curr);
				prev = _head;
			}
			else{
				prev->next = new node(currSv->index, ((sign) * currSv->value), curr);
				prev = prev->next;
			}

			currSv = currSv->next;
		}

		while(curr != 0 && currSv != 0 && currSv->index > curr->index )
		{
			prev = curr;
			curr = curr->next;
		}
	}

	if(currSv != 0)
	{
		curr = curr != 0 ? curr : prev;
		while(currSv != 0)
		{
			curr->next = new node(currSv->index, ((sign) * currSv->value));
			
			curr = curr->next;
			currSv = currSv->next;
		}
	}
}

void SparseVector::add(const SparseVector &sv)
{
	this->arithmetic(ADD, sv);
}

void SparseVector::subtract(const SparseVector &sv)
{
	this->arithmetic(SUB, sv);
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

SparseVector & SparseVector::operator+=(const SparseVector &sv)
{
	this->add(sv);
	return *this;
}

SparseVector & SparseVector::operator-=(const SparseVector &sv)
{
	this->subtract(sv);
	return *this;
}

const SparseVector SparseVector::operator+(const SparseVector &sv) const
{
	SparseVector result = *this;
   	result += sv; 
    	return result; 
}

const SparseVector SparseVector::operator-(const SparseVector &sv) const
{
	SparseVector result = *this;
   	result -= sv; 
    	return result; 
}

bool SparseVector::operator==(const SparseVector &sv) const
{
	if( _size != sv._size)
	{
		return false;
	}

	bool idx;
	bool val;

	node* curr = _head;
	node* currSv = sv._head;

	while(curr != 0 && currSv != 0)
	{
		idx = (curr->index == currSv->index);
		val = (curr->value == currSv->value);

		if(!idx || !val){
			return false;
		}

		curr = curr->next;
		currSv = currSv->next;
	}

	return (curr == 0 && currSv == 0);
}

bool SparseVector::operator!=(const SparseVector &sv) const
{
	return !(*this == sv);
}







