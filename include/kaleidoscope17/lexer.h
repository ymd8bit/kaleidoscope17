#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include "kaleidoscope17/token.h"

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <istream>
#include <map>

namespace kaleidoscope17 {

class Lexer
{
  using kwTokenMap = std::map<std::string, TokenType>;

  std::istream strm_;
  char cur_char_ = ' ';
  Token cur_token_;
  kwTokenMap kw_token_map_;

public:
  Lexer(std::istream &strm) : strm_{strm.rdbuf()}
  { // should be intialized with a space.
    register_kw_token_map();
  }

  // const Token get_token(char last_chr)
  Token get_token()
  {
    // skip any spaces
    while (isspace(cur_char_) && !is_eof()) { get_next_char(); };

    // check wheather current token is the end of file or not.
    if (is_eof()) {
      return {TokenType::Eof, static_cast<char>(EOF)};
    }

    // check wheather current token is an identifier or not.
    // this hits only when the first charactor is alphabet not a number.
    if (std::isalpha(cur_char_)) {
      std::string identifier_str{cur_char_};

      // get next char that could be a number or an alphabet.
      get_next_char();
      while (std::isalnum(cur_char_)) {
        identifier_str += cur_char_;
        get_next_char();
      }

      /// check wheather current token is one of the keyword tokens.
      if (kw_token_map_.count(identifier_str)) {
        auto token_type = kw_token_map_[identifier_str];
        return {token_type, std::move(identifier_str)};
      }
      return {TokenType::Id, std::move(identifier_str)};
    }

    // check wheather current token is number of not.
    // This hits the string is a sequence of numbers or decimal point.
    if (std::isdigit(cur_char_) || cur_char_ == '.') {
      std::string num_str{cur_char_};
      cur_char_ = get_next_char();
      while (std::isdigit(cur_char_) || cur_char_ == '.') {
        num_str += cur_char_;
        get_next_char();
      }

      // num_t is double.
      num_t num = std::strtod(num_str.c_str(), nullptr);
      return {TokenType::Num, num};
    }

    // check wheather current token is a number of not.
    // if this hits, this pass throgh the charactors until the newline.
    if (cur_char_ == '#') {
      do {
        get_next_char();
      } while (cur_char_ != EOF && cur_char_ != '\n' && cur_char_ != '\r');

      // if the last charactor doesn't reach to the end of the file,
      // lexer continues to get token recursively.
      if (cur_char_ != EOF)
        return get_token();
    }

    /// current char should be an ascii.
    assert(isascii(cur_char_));

    /// before return an ascii, lexer should consume the next charactor.
    char last_char = cur_char_;
    get_next_char();

    /// check wheather current tokey is one of the following items.
    if (last_char == '(') {
      return {TokenType::ParenL, last_char};
    } else if (last_char == ')') {
      return {TokenType::ParenR, last_char};
    } else if (last_char == ',') {
      return {TokenType::Comma, last_char};
    } else if (last_char == ';') {
      return {TokenType::Semicolon, last_char};
    }

    // return the charactors as just sequence of ASCII.
    return {TokenType::GenericAscii, last_char};
  }

private:
  const char get_next_char()
  {
    strm_.get(cur_char_);
    return cur_char_;
  }

  const bool is_eof() { return strm_.eof(); }

  void register_kw_token_map()
  {
    kw_token_map_["def"] = TokenType::Def;
    kw_token_map_["extern"] = TokenType::Extern;
  }
};

} // namespace kaleidoscope17

#endif