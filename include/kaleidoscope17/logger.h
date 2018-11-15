#ifndef KALEIDOSCOPE17_LOGGER_H
#define KALEIDOSCOPE17_LOGGER_H

#include <iostream>
#include <string>

using std::endl;
using std::ostream;
using std::string;

namespace kaleidoscope17 {

struct Logger
{
  static ostream &Debug(const string &msg, ostream &os = std::cout)
  {
    return Message("DEBUG", msg, os);
  }

  static ostream &Info(const string &msg, ostream &os = std::cout)
  {
    return Message("INFO", msg, os);
  };

  static ostream &Fatal(const string &msg, ostream &os = std::cout)
  {
    return Message("FATAL", msg, os);
  }

  static ostream &Message(const string &tag, const string &msg, ostream &os)
  {
    os << "[" + tag + "] " + msg << std::endl;
    return os;
  }

  static ostream &DebugHeader(const string &tag, const string &file_name,
                              const string &func_name, const unsigned line_num)
  {
    return Message(tag, "", std::cout) << "file: " << file_name << "\n"
                                       << "func: " << func_name << "\n"
                                       << "line: " << line_num << endl;
  }
};

#define DEBUG_HEADER(TAG, FILE, FUNC, LINE) \
  ::kaleidoscope17::Logger::DebugHeader(#TAG, FILE, FUNC, LINE)

#define ASSERT(COND, MSG)                                                  \
  if (!(COND)) {                                                           \
    DEBUG_HEADER(Assert, __FILE__, __FUNCTION__, __LINE__) << MSG << "\n"; \
  }                                                                        \
  assert(COND);

#define EXCEPTION(MSG)                                                     \
  DEBUG_HEADER(Exeption, __FILE__, __FUNCTION__, __LINE__) << MSG << "\n"; \
  assert(false);

} // namespace kaleidoscope17

#endif