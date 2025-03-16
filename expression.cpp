#include "expression.hpp"
#include <cmath>
#include <complex>
#include <stdexcept>
#include <sstream>

// --------------------- Конструкторы и оператор= ---------------------

template<typename T>
Expression<T>::Expression(T value)
    : type(Type::Constant), value(value) {}

template<typename T>
Expression<T>::Expression(const std::string& variable)
    : type(Type::Variable), variable(variable) {}

template<typename T>
Expression<T>::Expression(const Expression& other)
    : type(other.type), value(other.value), variable(other.variable)
{
    if (other.left)
        left = std::unique_ptr<Expression>(new Expression(*other.left));
    if (other.right)
        right = std::unique_ptr<Expression>(new Expression(*other.right));
}

template<typename T>
Expression<T>::Expression(Expression&& other) noexcept
    : type(other.type), value(std::move(other.value)),
      variable(std::move(other.variable)),
      left(std::move(other.left)), right(std::move(other.right))
{}

template<typename T>
Expression<T>::Expression(typename Expression<T>::Type type,
                          std::unique_ptr<Expression> left,
                          std::unique_ptr<Expression> right)
    : type(type), left(std::move(left)), right(std::move(right))
{}

template<typename T>
Expression<T>::~Expression() {}

template<typename T>
Expression<T>& Expression<T>::operator=(const Expression& other) {
    if (this != &other) {
        type = other.type;
        value = other.value;
        variable = other.variable;
        if (other.left)
            left = std::unique_ptr<Expression>(new Expression(*other.left));
        else
            left.reset();
        if (other.right)
            right = std::unique_ptr<Expression>(new Expression(*other.right));
        else
            right.reset();
    }
    return *this;
}

template<typename T>
Expression<T>& Expression<T>::operator=(Expression&& other) noexcept {
    if (this != &other) {
        type = other.type;
        value = std::move(other.value);
        variable = std::move(other.variable);
        left = std::move(other.left);
        right = std::move(other.right);
    }
    return *this;
}

// --------------------- Арифметические операторы ---------------------

template<typename T>
Expression<T> Expression<T>::operator+(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Add;
    result.left = std::unique_ptr<Expression>(new Expression(*this));
    result.right = std::unique_ptr<Expression>(new Expression(other));
    return result;
}

template<typename T>
Expression<T> Expression<T>::operator-(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Subtract;
    result.left = std::unique_ptr<Expression>(new Expression(*this));
    result.right = std::unique_ptr<Expression>(new Expression(other));
    return result;
}

template<typename T>
Expression<T> Expression<T>::operator*(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Multiply;
    result.left = std::unique_ptr<Expression>(new Expression(*this));
    result.right = std::unique_ptr<Expression>(new Expression(other));
    return result;
}

template<typename T>
Expression<T> Expression<T>::operator/(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Divide;
    result.left = std::unique_ptr<Expression>(new Expression(*this));
    result.right = std::unique_ptr<Expression>(new Expression(other));
    return result;
}

template<typename T>
Expression<T> Expression<T>::operator^(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Power;
    result.left = std::unique_ptr<Expression>(new Expression(*this));
    result.right = std::unique_ptr<Expression>(new Expression(other));
    return result;
}

// --------------------- Функции (sin, cos, ln, exp) ---------------------

template<typename T>
Expression<T> Expression<T>::sin(const Expression& expr) {
    Expression result(expr);
    result.type = Type::Sin;
    result.left = std::unique_ptr<Expression>(new Expression(expr));
    return result;
}

template<typename T>
Expression<T> Expression<T>::cos(const Expression& expr) {
    Expression result(expr);
    result.type = Type::Cos;
    result.left = std::unique_ptr<Expression>(new Expression(expr));
    return result;
}

template<typename T>
Expression<T> Expression<T>::ln(const Expression& expr) {
    Expression result(expr);
    result.type = Type::Ln;
    result.left = std::unique_ptr<Expression>(new Expression(expr));
    return result;
}

template<typename T>
Expression<T> Expression<T>::exp(const Expression& expr) {
    Expression result(expr);
    result.type = Type::Exp;
    result.left = std::unique_ptr<Expression>(new Expression(expr));
    return result;
}

// --------------------- Подстановка и вычисление ---------------------

template<typename T>
Expression<T> Expression<T>::substitute(const std::string& var, const T& val) const {
    if (type == Type::Variable && variable == var) {
        return Expression<T>(val);
    }
    if (left && right) {
        auto newLeft = left->substitute(var, val);
        auto newRight = right->substitute(var, val);
        return Expression<T>(type,
            std::unique_ptr<Expression>(new Expression(newLeft)),
            std::unique_ptr<Expression>(new Expression(newRight)));
    } else if (left) {
        auto newLeft = left->substitute(var, val);
        return Expression<T>(type,
            std::unique_ptr<Expression>(new Expression(newLeft)),
            nullptr);
    }
    return *this;
}

template<typename T>
T Expression<T>::evaluate(const std::map<std::string, T>& variables) const {
    switch (type) {
        case Type::Constant:
            return value;
        case Type::Variable:
            if (variables.find(variable) == variables.end())
                throw std::runtime_error("Не задано значение для переменной " + variable);
            return variables.at(variable);
        case Type::Add:
            return left->evaluate(variables) + right->evaluate(variables);
        case Type::Subtract:
            return left->evaluate(variables) - right->evaluate(variables);
        case Type::Multiply:
            return left->evaluate(variables) * right->evaluate(variables);
        case Type::Divide:
            return left->evaluate(variables) / right->evaluate(variables);
        case Type::Power:
            return std::pow(left->evaluate(variables), right->evaluate(variables));
        case Type::Sin:
            return std::sin(left->evaluate(variables));
        case Type::Cos:
            return std::cos(left->evaluate(variables));
        case Type::Ln:
            return std::log(left->evaluate(variables));
        case Type::Exp:
            return std::exp(left->evaluate(variables));
    }
    throw std::runtime_error("Неподдерживаемый тип выражения");
}

// --------------------- Строковое представление ---------------------

template<typename T>
std::string Expression<T>::toString() const {
    std::ostringstream oss;
    switch (type) {
        case Type::Constant:
            oss << value;
            break;
        case Type::Variable:
            oss << variable;
            break;
        case Type::Add:
            oss << "(" << left->toString() << " + " << right->toString() << ")";
            break;
        case Type::Subtract:
            oss << "(" << left->toString() << " - " << right->toString() << ")";
            break;
        case Type::Multiply:
            oss << "(" << left->toString() << " * " << right->toString() << ")";
            break;
        case Type::Divide:
            oss << "(" << left->toString() << " / " << right->toString() << ")";
            break;
        case Type::Power:
            oss << "(" << left->toString() << " ^ " << right->toString() << ")";
            break;
        case Type::Sin:
            oss << "sin(" << left->toString() << ")";
            break;
        case Type::Cos:
            oss << "cos(" << left->toString() << ")";
            break;
        case Type::Ln:
            oss << "ln(" << left->toString() << ")";
            break;
        case Type::Exp:
            oss << "exp(" << left->toString() << ")";
            break;
    }
    return oss.str();
}

template<typename T>
std::string Expression<T>::typeToString() const {
    switch (type) {
        case Type::Constant:  return "Constant";
        case Type::Variable:  return "Variable";
        case Type::Add:       return "Add";
        case Type::Subtract:  return "Subtract";
        case Type::Multiply:  return "Multiply";
        case Type::Divide:    return "Divide";
        case Type::Power:     return "Power";
        case Type::Sin:       return "Sin";
        case Type::Cos:       return "Cos";
        case Type::Ln:        return "Ln";
        case Type::Exp:       return "Exp";
    }
    return "Unknown";
}

// --------------------- Символьная производная ---------------------

template<typename T>
Expression<T> Expression<T>::derivative(const std::string& var) const {
    switch (type) {
        case Type::Constant:
            return Expression<T>(static_cast<T>(0));
        case Type::Variable:
            return (variable == var) ? Expression<T>(static_cast<T>(1))
                                     : Expression<T>(static_cast<T>(0));
        case Type::Add:
            return left->derivative(var) + right->derivative(var);
        case Type::Subtract:
            return left->derivative(var) - right->derivative(var);
        case Type::Multiply:
            return (left->derivative(var) * (*right)) +
                   ((*left) * right->derivative(var));
        case Type::Divide:
            return ((left->derivative(var) * (*right))
                    - ((*left) * right->derivative(var)))
                    / ((*right) ^ Expression<T>(static_cast<T>(2)));
        case Type::Power:
            if (right->type == Type::Constant) {
                T c = right->value;
                return Expression<T>(c)
                       * ((*left) ^ Expression<T>(c - static_cast<T>(1)))
                       * left->derivative(var);
            } else {
                return (*this) * (
                    right->derivative(var) * Expression<T>::ln(*left)
                    + (*right) * left->derivative(var) / (*left)
                );
            }
        case Type::Sin:
            return Expression<T>::cos(*left) * left->derivative(var);
        case Type::Cos:
            return Expression<T>(static_cast<T>(-1))
                   * Expression<T>::sin(*left)
                   * left->derivative(var);
        case Type::Ln:
            return left->derivative(var) / (*left);
        case Type::Exp:
            return Expression<T>::exp(*left) * left->derivative(var);
    }
    throw std::runtime_error("Derivative not implemented for this expression type");
}

template class Expression<double>;
template class Expression<std::complex<double>>;