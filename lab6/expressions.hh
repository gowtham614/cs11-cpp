#ifndef _EXPRESSIONS_HH_
#define _EXPRESSIONS_HH_

#include <cassert>
#include <stdexcept>
#include <cmath>

#include "environment.hh"

class Expression {

private:

public:
  // Constructors
  Expression() {}

  // Destructor
  virtual ~Expression() {}


  virtual double evaluate(const Environment &env) const = 0;
};


/*===========================*/
/*    Expression :: Value    */
/*===========================*/


class Value : public Expression {

private:
  double _val;

public:
  // Constructors
  Value(double val) : _val(val) {}

  // Destructor
  ~Value() {}

  double evaluate(const Environment &env) const { return _val; }
};

/*===========================*/
/*    Expression :: Symbol   */
/*===========================*/

class Symbol : public Expression {

private:
  string _name;

public:
  // Constructors
  Symbol(const string &name) : _name(name) {}

  string getName() const { return _name; }

  // Destructor
  ~Symbol() {}

  double evaluate(const Environment &env) const { return env.getSymbolValue(_name); }
};

/*===================================*/
/*    Expression :: BinaryOperator   */
/*===================================*/

class BinaryOperator : public Expression {

protected:
  Expression *_pRHS;
  Expression *_pLHS;

public:
  // Constructors
  BinaryOperator(Expression *pLHS, Expression *pRHS) : _pRHS(pRHS) , _pLHS(pLHS) {
	assert(_pRHS != 0);
	assert(_pLHS != 0);
  }

  // Destructor
  virtual ~BinaryOperator() {
	delete _pRHS; 
	delete _pLHS;
  }

  // Accessor methods
  const Expression& getRHS() const {return *_pRHS;}
  const Expression& getLHS() const {return *_pLHS;}

  virtual double evaluate(const Environment &env) const = 0;
};


/*================================*/
/*    BinaryOperator :: AddOper   */
/*================================*/


class AddOper : public BinaryOperator {

public:
  // Constructors
  AddOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS) {}

  // Destructor
  ~AddOper() {}

  double evaluate(const Environment &env) const 
  {
	double lhs = _pLHS->evaluate(env);
	double rhs = _pRHS->evaluate(env);
	
	return lhs + rhs;
  }
};

/*================================*/
/*    BinaryOperator :: SubOper   */
/*================================*/

class SubOper : public BinaryOperator {

public:
  // Constructors
  SubOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS) {}

  // Destructor
  ~SubOper() {}

  double evaluate(const Environment &env) const 
  {
	double lhs = _pLHS->evaluate(env);
	double rhs = _pRHS->evaluate(env);
	
	return lhs - rhs;
  }
};

/*================================*/
/*    BinaryOperator :: MulOper   */
/*================================*/

class MulOper : public BinaryOperator {

public:
  // Constructors
  MulOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS) {}

  // Destructor
  ~MulOper() {}

  double evaluate(const Environment &env) const 
  {
	double lhs = _pLHS->evaluate(env);
	double rhs = _pRHS->evaluate(env);
	
	return lhs * rhs;
  }
};

/*================================*/
/*    BinaryOperator :: DivOper   */
/*================================*/

class DivOper : public BinaryOperator {

public:
  // Constructors
  DivOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS) {}

  // Destructor
  ~DivOper() {}

  double evaluate(const Environment &env) const 
  {
	double rhs = _pRHS->evaluate(env);
	if(rhs == 0) 
		throw std::runtime_error ("divide-by-zero");

	double lhs = _pLHS->evaluate(env);
	
	return lhs / rhs;
  }
};

/*================================*/
/*    BinaryOperator :: PowOper   */
/*================================*/

class PowOper : public BinaryOperator {

public:
  // Constructors
  PowOper(Expression *pLHS, Expression *pRHS) : BinaryOperator(pLHS, pRHS) {}

  // Destructor
  ~PowOper() {}

  double evaluate(const Environment &env) const 
  {
	double lhs = _pLHS->evaluate(env);
	double rhs = _pRHS->evaluate(env);

	return pow (lhs, rhs);
  }
};

/*===================================*/
/*    Expression :: UnaryOperator    */
/*===================================*/

class UnaryOperator : public Expression {

protected:
  Expression *_pEXP;

public:
  // Constructors
  UnaryOperator(Expression *pEXP) : _pEXP(pEXP) {
	assert(_pEXP != 0);
  }

  // Destructor
  virtual ~UnaryOperator() {
	delete _pEXP; 
  }

  // Accessor methods
  const Expression& getEXP() const {return *_pEXP;}

  virtual double evaluate(const Environment &env) const = 0;
};


/*===============================*/
/*    UnaryOperator :: NegOper   */
/*===============================*/


class NegOper : public UnaryOperator {

public:
  // Constructors
  NegOper(Expression *pEXP) : UnaryOperator(pEXP) {}

  // Destructor
  ~NegOper() {}

  double evaluate(const Environment &env) const 
  {
	double exp = _pEXP->evaluate(env);
	
	return -exp;
  }
};


/*===============================*/
/*    UnaryOperator :: FacOper   */
/*===============================*/


class FacOper : public UnaryOperator {

private:
  int factorial(int n) const{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
  } 

public:
  // Constructors
  FacOper(Expression *pEXP) : UnaryOperator(pEXP) {}

  // Destructor
  ~FacOper() {}

  double evaluate(const Environment &env) const 
  {
	double exp = _pEXP->evaluate(env);
	if (exp < 0)
		throw  std::runtime_error ("factorial operator is applied to a negative value");

	double exp2  = floor(exp);
	if (exp2 != exp)
		throw  std::runtime_error ("factorial operator is applied to a non-integer value");
	
	return factorial(exp);
  }
};



#endif // _EXPRESSIONS_HH_
