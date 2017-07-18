#pragma once
#include <string>

namespace ExprSorter_Parsing
{
  //Represents a syntactic error in processed user input.
  class BadSyntaxException
  {
    public:
      //Creates a syntax exception object with a message.
      BadSyntaxException(std::string message);

      //Returns the message associated with the exception.
      std::string GetMessage();

    private:
      //Stores a message associated with the error.
      std::string message;
  };
}