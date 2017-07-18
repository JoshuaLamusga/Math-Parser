#include <iostream>
#include "Parser.h"
#include "BadSyntaxException.h"
using std::cout;
using std::cin;
using std::endl;

int main()
{
  std::string userInput;

  while (true)
  {
    cout << "Please enter an expression: ";
    std::getline(cin, userInput);

    try
    {
      cout << ExprSorter_Parsing::EvalPostfix(
        ExprSorter_Parsing::InfixToPostfix(userInput)) << endl;
    }
    catch (ExprSorter_Parsing::BadSyntaxException e)
    {
      cout << e.GetMessage() << endl;
      continue;
    }
  }

  return 0;
}