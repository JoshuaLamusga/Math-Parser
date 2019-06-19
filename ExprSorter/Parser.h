#pragma once
#include <string>
#include <list>
#include "ParseToken.h"

namespace ExprSorter_Parsing
{
  //Evaluates and returns a value for an rpn (reverse polish notation) string.
  //Throws: BadSyntaxException for invalid input.
  long double EvalPostfix(std::list<std::string>);

  //Parses an infix mathematical expression to rpn format.
  //Throws: BadSyntaxException for invalid input.
  std::list<std::string> InfixToPostfix(std::string expr);
}