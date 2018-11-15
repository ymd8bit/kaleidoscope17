#ifndef KALEIDOSCOPE17_TOKEN_H
#define KALEIDOSCOPE17_TOKEN_H

#include <cstdint>
#include <string>
#include <variant>

namespace kaleidoscope17 {

enum class TokenType
{
  Eof,
  Def,
  Extern,
  Id,
  Num,
  GenericAscii,

  ParenL,
  ParenR,
  Comma,
  Semicolon,
};

using num_t = double;

struct Token
{
  TokenType type;
  std::variant<num_t, std::string, char> val;

  const std::string name();
  // template <typename T>
  // const T val();
};

/*
template char Token::val()
{
  switch (type) {
    case TokenType::GenericAscii:
      return get_token_value<char>(*this);
      break;
    default:
      assert(false && "Given token value is not char...");
  }
}

template const std::string Token::val()
{
  switch (type) {
    case TokenType::Id:
      return get_token_value<std::string>(*this);
      break;
    default:
      assert(false && "Given token value is not string...");
  }
}
const num_t Token::val() { return get_token_value<num_t>(*this); }
*/

template <class T>
inline T get_token_value(Token &token)
{
  return std::get<T>(token.val);
}

} // namespace kaleidoscope17

#endif