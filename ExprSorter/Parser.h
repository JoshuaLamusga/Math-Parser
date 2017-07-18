#include <string>
#include <list>
#include "ParseToken.h"

#pragma once
namespace ExprSorter_Parsing
{
  //Returns the validity of a string formatted as parentheses.
  //A string is considered valid if:
  //all characters are parentheses or the whitespace character,
  //evaluated left to right, number of ( symbols is >= ) symbols, and
  //there is an equal number of open and close parentheses.
  bool AreParenthesesValid(std::string expr);

  //Evaluates and returns a value for an rpn (reverse polish notation) string.
  //Throws: BadSyntaxException for invalid input.
  long double EvalPostfix(std::list<std::string>);

  //Parses an infix mathematical expression to rpn format.
  //Throws: BadSyntaxException for invalid input.
  std::list<std::string> InfixToPostfix(std::string expr);
}