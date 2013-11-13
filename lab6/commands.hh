#ifndef _COMMANDS_HH_
#define _COMMANDS_HH_

#include "environment.hh"
#include "expressions.hh"

#include <iostream>

class Command {

public:
  // Constructors
  Command() {}

  // Destructor
  virtual ~Command() {}

  virtual void run(Environment &env) = 0;
};


/*===============================*/
/*    Command :: PrintCommand    */
/*===============================*/

class PrintCommand : public Command {

private:
  Expression *_pEXP;

public:
  // Constructors
  PrintCommand(Expression *pEXP) : _pEXP(pEXP) {
	assert(_pEXP != 0);
  }

  // Destructor
  ~PrintCommand() { 
	delete _pEXP;
  }

  void run(Environment &env) {
	std::cout << " = " << _pEXP->evaluate(env) << std::endl;
  }
};

/*================================*/
/*    Command :: AssignCommand    */
/*================================*/

class AssignCommand : public Command {

private:
  Expression *_pEXP;
  Symbol *_pSYM;

public:
  // Constructors
  AssignCommand(Symbol *pSYM, Expression *pEXP) : _pEXP(pEXP) , _pSYM(pSYM) {
	assert(_pEXP != 0);
	assert(_pSYM != 0);
  }

  // Destructor
  ~AssignCommand() { 
	delete _pEXP;
	delete _pSYM;
  }

  void run(Environment &env) {
	string name = _pSYM->getName();
	double val  = _pEXP->evaluate(env);
	env.setSymbolValue(name, val);
	std::cout << name << " = " << val << std::endl;
  }
};

#endif // _COMMANDS_HH_
