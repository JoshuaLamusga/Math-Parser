#include <string>
#include "ParseToken.h"

namespace ExprSorter_Parsing
{
  ParseToken::ParseToken()
  {
    Empty();
  }

  ParseToken::ParseToken(
    std::string strForm,
    int numArgs)
  {
    cForm = ' ';
    this->strForm = strForm;
    isFunction = true;
    isLeftAssociative = true;
    this->numArgs = numArgs;
    precedence = 0;
  }

  ParseToken::ParseToken(
    char cForm,
    bool isLeftAssociative,
    int precedence,
    int numArgs)
  {
    this->cForm = cForm;
    strForm = std::string(1, cForm);
    isFunction = false;
    this->isLeftAssociative = isLeftAssociative;
    this->numArgs = numArgs;
    this->precedence = precedence;
  }

  bool ParseToken::IsEmpty()
  {
    return (cForm == ' ' &&
    strForm == "" &&
    !isFunction &&
    isLeftAssociative &&
    numArgs == 0 &&
    precedence == 0);
  }

  bool ParseToken::IsFunction()
  {
    return isFunction;
  }

  void ParseToken::Empty()
  {
    cForm = ' ';
    strForm = "";
    isFunction = false;
    isLeftAssociative = true;
    numArgs = 0;
    precedence = 0;
  }
};