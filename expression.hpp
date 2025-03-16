#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <cmath>
#include <memory>
#include <map>
#include <sstream>
#include <stdexcept>
#include <complex>
#include <string>

template<typename T>
class Expression {
public:
    enum class Type {
        Constant, Variable, Add, Subtract, Multiply, Divide, Power,
        Sin, Cos, Ln, Exp
    };

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
    Expression operator/(const Expression& other) const;
    Expression operator^(const Expression& other) const;

    static Expression sin(const Expression& expr);
    static Expression cos(const Expression& expr);
    static Expression ln(const Expression& expr);
    static Expression exp(const Expression& expr);

    Expression substitute(const std::string& var, const T& value) const;

    T evaluate(const std::map<std::string, T>& variables) const;

    std::string toString() const;
    std::string typeToString() const;

    Expression derivative(const std::string& var) const;

    Type type;
    T value = T();
    std::string variable;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
};

extern template class Expression<double>;
extern template class Expression<std::complex<double>>;

#endif // EXPRESSION_HPP