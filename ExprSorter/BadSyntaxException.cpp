#include <string>
#include "BadSyntaxException.h"

namespace ExprSorter_Parsing
{
    //Creates a syntax exception object with a message.
    BadSyntaxException::BadSyntaxException(std::string message)
    {
      this->message = message;
    }

    //Returns the message associated with the exception.
    std::string BadSyntaxException::GetMessage()
    {
      return message;
    }
}