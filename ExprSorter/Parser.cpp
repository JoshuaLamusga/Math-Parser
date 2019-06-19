#include <vector>
#include <stack>
#include "Parser.h"
#include "BadSyntaxException.h"

namespace ExprSorter_Parsing
{
  std::list<std::string> ExprSorter_Parsing::InfixToPostfix(std::string expr)
  {
    //This is an implementation of the Shunting-Yard algorithm.

    const ParseToken DIV('/', true, 3, 2); //Division
    const ParseToken EXP('^', false, 4, 2); //Exponentiation
    const ParseToken FAC('!', true, 5, 1); //Factorial
    const ParseToken LBR('(', true, 6, 0); //Left brace
    const ParseToken MIN('-', true, 2, 2); //Binary minus
    const ParseToken MOD('%', true, 3, 2); //Modulus
    const ParseToken MULT('*', true, 3, 2); //Multiplication
    const ParseToken NEG('_', false, 4, 1); //Unary minus (not in list)
    const ParseToken PLS('+', true, 2, 2); //Addition
    const ParseToken RBR(')', true, 6, 0); //Right brace

    //Function tokens.
    const ParseToken F_RND("round", 1); //Round(a)

    //All operator tokens.
    std::vector<ParseToken> validOps =
      {DIV, EXP, FAC, LBR, MIN, MOD, MULT, PLS, RBR, NEG};

    //All function tokens.
    std::vector<ParseToken> validFuncs =
      {F_RND};

    std::list<std::string> output; //Stores final output.
    std::stack<ParseToken> opStack; //Stores operator tokens.
    std::string idStr = ""; //Stores identifier tokens.

    //Reads each token.
    for (char c : expr)
    {
      //If the character is whitespace, ignore it.
      if (isspace(c))
      {
        continue;
      }

      //If the character is part of an identifer or number token, add it.
      if (isalnum(c) || c == '.')
      {
        //Rejects numbers with multiple decimals.
        if (c == '.' && idStr.find('.') != idStr.npos)
        {
          throw BadSyntaxException("Numbers may contain only one decimal.");
        }

        idStr += c;

        //If it is a number token, rejects identifier characters.
        if ((isdigit(idStr.at(0)) || idStr.at(0) == '.') && isalpha(c))
        {
          throw BadSyntaxException("Expected a number. Found '" +
            idStr + "' instead.");
        }
      }

      //If the token is not an identifier or number.
      else
      {
        //The identifier or number string represents a completed token.
        if (idStr.size() != 0)
        {
          //Pushes number tokens to output.
          if (isdigit(idStr.at(0)) || idStr.at(0) == '.')
          {
            output.push_back(idStr);
          }

          //Functions are pushed to the stack.
          else if (idStr == "round")
          {
            opStack.push(F_RND);
          }

          //Unrecognized identifier.
          else
          {
            throw BadSyntaxException("Identifier '" +
              idStr + "' unrecognized.");
          }

          idStr.clear();
        }

        //If the token is a function argument delimiter.
        if (c == ',')
        {
          //Moves from stack to output until LBR token is found.
          while (opStack.size() != 0)
          {
            if (opStack.top().cForm != LBR.cForm)
            {
              output.push_back(opStack.top().strForm);
            }
            else
            {
              break;
            }

            opStack.pop();
          }

          //Delimeter or parentheses were out of order.
          if (opStack.size() == 0)
          {
            throw BadSyntaxException("Bad function arguments given.");
          }
        }

        //If the token is an operator.
        else
        {
          //Converts the new character to an operator.
          ParseToken opToken;

          for (ParseToken op : validOps)
          {
            if (c == op.cForm)
            {
              opToken = op;
              break;
            }
          }

          //If the token is an LBR, pushes it to operator stack.
          if (opToken.cForm == LBR.cForm)
          {
            opStack.push(opToken);
          }

          //If the token is an RBR.
          else if (opToken.cForm == RBR.cForm)
          {
            //Until the token at top of stack is LBR.
            do
            {
              if (opStack.size() != 0)
              {
                if (opStack.top().cForm == LBR.cForm)
                {
                  break;
                }
                else
                {
                  output.push_back(opStack.top().strForm);
                  opStack.pop();
                }
              }

              //If the stack runs out without finding an LBR token.
              else
              {
                throw BadSyntaxException("There are mismatched parentheses.");
              }
            } while (opStack.size() != 0 && opStack.top().cForm != LBR.cForm);

            //Pop the LBR from the stack, but not to output.
            opStack.pop();

            //Pops a function token (if it exists) to output.
            if (opStack.size() != 0 && opStack.top().IsFunction())
            {
              output.push_back(opStack.top().strForm);
              opStack.pop();
            }
          }

          //If the token is unrecognized.
          else if (opToken.IsEmpty())
          {
            throw BadSyntaxException("invalid operator: " + std::string(1, c));
          }

          //If the token is a different operator.
          else
          {
            //If there's an operator on the stack and one being read,
            //and the first is left-associative with prec <= second,
            //or it's right-associative with prec < second.
            while (!opToken.IsEmpty() && (opStack.size() != 0) &&
              opStack.top().cForm != LBR.cForm &&
              opStack.top().cForm != RBR.cForm &&
              ((opToken.isLeftAssociative &&
                opToken.precedence <= opStack.top().precedence) ||
                (!opToken.isLeftAssociative &&
                  opToken.precedence < opStack.top().precedence)))
            {
              //Pop it to the output.
              output.push_back(opStack.top().strForm);
              opStack.pop();

              if (opStack.size() == 0)
              {
                break;
              }
            }

            //Push the newly-read operator on afterwards.
            opStack.push(opToken);
          }
        }
      }
    }

    //If the input ends with an identifier, finalizes it.
    if (idStr.size() != 0)
    {
      //Pushes number tokens to output.
      if (isdigit(idStr.at(0)) || idStr.at(0) == '.')
      {
        output.push_back(idStr);
      }

      //Functions are pushed to the stack.
      else if (idStr == "round")
      {
        opStack.push(F_RND);
      }

      //Unrecognized identifier.
      else
      {
        throw BadSyntaxException("Identifier '" +
          idStr + "' unrecognized.");
      }

      idStr.clear();
    }

    //When there are no more tokens to read and operators left over.
    while (opStack.size() != 0)
    {
      //If the operator token on top is an LBR or RBR, it's mismatched.
      if (opStack.top().cForm == LBR.cForm ||
        opStack.top().cForm == RBR.cForm)
      {
        throw BadSyntaxException("There are mismatched parentheses.");
      }

      //Pop the operator to the output.
      output.push_back(opStack.top().strForm);
      opStack.pop();
    }

    return output;
  }

  long double ExprSorter_Parsing::EvalPostfix(std::list<std::string> tokens)
  {
    //TODO: Refactor to define token types once, outside any functions.

    //Operator tokens.
    const ParseToken DIV('/', true, 3, 2); //Division
    const ParseToken EXP('^', false, 4, 2); //Exponentiation
    const ParseToken FAC('!', true, 5, 1); //Factorial
    const ParseToken LBR('(', true, 6, 0); //Left brace
    const ParseToken MIN('-', true, 2, 2); //Binary minus
    const ParseToken MOD('%', true, 3, 2); //Modulus
    const ParseToken MULT('*', true, 3, 2); //Multiplication
    const ParseToken NEG('_', false, 4, 1); //Unary minus
    const ParseToken PLS('+', true, 2, 2); //Addition
    const ParseToken RBR(')', true, 6, 0); //Right brace

    //Function tokens.
    const ParseToken F_RND("round", 1); //Round(a)

    //All operator tokens.
    std::vector<ParseToken> validOps =
    { DIV, EXP, FAC, LBR, MIN, MOD, MULT, NEG, PLS, RBR };

    //All function tokens.
    std::vector<ParseToken> validFuncs =
    { F_RND };

    std::list<std::string> output; //Stores final output.
    std::stack<long double> valStack; //Stores values.

    //Handles empty input.
    if (tokens.size() == 0)
    {
      throw BadSyntaxException("No expression to perform was found.");
    }

    //Reads each token.
    for (std::string tokenStr : tokens)
    {
      //Reads through all tokens to identify the token string.
      ParseToken token;

      //Tries to identify the string as an operator token.
      for (ParseToken otherToken : validOps)
      {
        if (tokenStr == otherToken.strForm)
        {
          token = otherToken;
          break;
        }
      }

      //Tries to identify the string as a function token.
      if (token.IsEmpty())
      {
        for (ParseToken otherToken : validFuncs)
        {
          if (tokenStr == otherToken.strForm)
          {
            token = otherToken;
            break;
          }
        }
      }

      //Assumes the string is a value.
      if (token.IsEmpty())
      {
        //Converts the string to a number, rethrowing exceptions to suit
        //the parser.
        long double val = 0;
        try
        {
          val = std::stold(tokenStr);
        }
        catch (std::invalid_argument e)
        {
          throw BadSyntaxException("Number was invalid: " + tokenStr);
        }
        catch (std::out_of_range)
        {
          throw BadSyntaxException("Number '" +
            tokenStr + "' is too large or precise.");
        }
        if (val == NAN || val == INFINITY)
        {
          throw BadSyntaxException("Expected a number; found '" +
            std::to_string(val) + "' instead.");
        }

        valStack.push(val);
      }

      //The token is an operator or function.
      else
      {
        //There must be as many values as arguments, or input is bad.
        if (valStack.size() < token.numArgs)
        {
          throw BadSyntaxException("There aren't enough arguments for "
            "the function.");
        }

        //Evaluates the operation with its needed number of values.
        else
        {
          //Gets all arguments in reverse (the correct) order.
          std::vector<long double> args;
          for (int i = 0; i < token.numArgs; ++i)
          {
            args.push_back(valStack.top());
            valStack.pop();
          }
          std::reverse(args.begin(), args.end());

          //Performs each operation.
          if (token.cForm == DIV.cForm)
          {
            if (args[1] == 0)
            {
              throw BadSyntaxException("Can't divide by zero.");
            }
            
            valStack.push(args[0] / args[1]);
          }
          else if (token.cForm == EXP.cForm)
          {
            valStack.push(std::pow(args[0], args[1]));
          }
          else if (token.cForm == FAC.cForm)
          {
            valStack.push(std::tgammal(args[0] + 1));
          }
          else if (token.cForm == MIN.cForm)
          {
            valStack.push(args[0] - args[1]);
          }
          else if (token.cForm == MOD.cForm)
          {
            if (args[1] == 0)
            {
              throw BadSyntaxException("Can't divide by zero.");
            }

            valStack.push((int)args[0] % (int)args[1]);
          }
          else if (token.cForm == MULT.cForm)
          {
            valStack.push(args[0] * args[1]);
          }
          else if (token.cForm == NEG.cForm)
          {
            valStack.push(args[0] * -1);
          }
          else if (token.cForm == PLS.cForm)
          {
            valStack.push(args[0] + args[1]);
          }
          else if (token.cForm == F_RND.cForm)
          {
            valStack.push(std::roundl(args[0]));
          }
        }
      }
    }

    if (valStack.size() == 1)
    {
      return valStack.top();
    }
    else
    {
      throw BadSyntaxException("Input has too many values at once.");
    }
  }
}