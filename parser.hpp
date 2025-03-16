#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "expression.hpp"

Expression<double> parseExpression(const std::string& str);

#endif // PARSER_HPP
