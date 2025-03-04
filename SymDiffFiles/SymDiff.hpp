#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <map>
#include <memory>
#include <cmath>

template <typename T>
class Expression {
public:
    Expression(T value);
    Expression(const std::string& variable);

    ~Expression();

    Expression(const Expression& other);
    Expression(Expression&& other) noexcept;

    Expression& operator=(const Expression& other);
    Expression& operator=(Expression&& other) noexcept;

    Expression operator+(const Expression& other) const;
    Expression operator-(const Expression& other) const;
    Expression operator*(const Expression& other) const;
    Expression operator/(const Expression& other) const;
    Expression operator^(const Expression& other) const;

    Expression sin() const;
    Expression cos() const;
    Expression ln() const;
    Expression exp() const;

    std::string toString() const;

    Expression substitute(const std::string& variable, T value) const;

    T evaluate(const std::map<std::string, T>& variables) const;

    Expression derivative(const std::string& variable) const;

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // EXPRESSION_HPP