#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <complex>
#include <map>
#include <sstream>

template<typename T>
class Expression {
public:
    Expression(T value);
    Expression(const std::string& variable);
    Expression(const Expression& other);
    Expression(Expression&& other) noexcept;
    Expression(Type type, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right); 
    ~Expression();

    Expression& operator=(const Expression& other);
    Expression& operator=(Expression&& other) noexcept;

    Expression operator+(const Expression& other) const;
    Expression operator-(const Expression& other) const;
    Expression operator*(const Expression& other) const;
    Expression operator*(Expression&& other) const;
    Expression operator/(const Expression& other) const;
    Expression operator^(const Expression& other) const;

    static Expression sin(const Expression& expr);
    static Expression cos(const Expression& expr);
    static Expression ln(const Expression& expr);
    static Expression exp(const Expression& expr);

    Expression substitute(const std::string& var, const T& value) const;


    T evaluate(const std::map<std::string, T>& variables) const;

    std::string toString() const;

private:
    enum class Type { Constant, Variable, Add, Subtract, Multiply, Divide, Power, Sin, Cos, Ln, Exp };
    Type type;
    T value;
    std::string variable;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    std::string typeToString() const;
};

#endif // EXPRESSION_HPP