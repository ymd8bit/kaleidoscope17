#include "kaleidoscope17/token.h"

#include <string>

using namespace kaleidoscope17;

const std::string Token::name()
{
  switch (type)
  {
  default:
    assert(false && "Given token's type is not supported... ");
    return "";
    break;
  case TokenType::Eof:
    return "TokenType::EOF";
    break;
  case TokenType::Semicolon:
    return "TokenType::Semicolon";
    break;
  case TokenType::Def:
    return "TokenType::Def";
    break;
  case TokenType::Extern:
    return "TokenType::Extern";
    break;
  case TokenType::Id:
    return "ID(" + std::get<std::string>(val) + ")";
    break;
  case TokenType::ParenL:
    return "TokenType::ParenL";
    break;
  case TokenType::ParenR:
    return "TokenType::ParenR";
    break;
  case TokenType::Comma:
    return "TokenType::Comma";
    break;
  case TokenType::Num:
    return "Number,(" + std::to_string(std::get<num_t>(val)) + ")";
    break;
  case TokenType::GenericAscii:
    return "TokenType::GenericAscii(" + std::string{std::get<char>(val)} +
           ")";
    break;
  }
}