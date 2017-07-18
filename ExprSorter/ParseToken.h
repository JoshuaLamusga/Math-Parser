#pragma once
#include <string>

namespace ExprSorter_Parsing
{
  //Represents an operator or function token.
  struct ParseToken
  {
    public:
      ParseToken();

      //Initializes as a function token.
      ParseToken(
        std::string strForm,
        int numArgs);

      //Initializes as an operator token.
      ParseToken(
        char cForm,
        bool isLeftAssociative,
        int precedence,
        int numArgs);

      //Returns true if the token is a function token.
      bool IsFunction();

      //Overwrites all values to be empty again.
      void Empty();

      //Returns true if the object is initialized to default values.
      bool IsEmpty();

      //The character representation of the operator.
      char cForm;

      //The string representation of the operator.
      std::string strForm;

      //Affects output order for associativity.
      bool isLeftAssociative;

      //The number of arguments needed.
      int numArgs;

      //Determines precedence.
      int precedence;

    private:

      //True if the token is a function token.
      bool isFunction;
  };
}