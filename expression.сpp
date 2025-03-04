#include "expression.hpp"

template<typename T>
Expression<T>::Expression(T value) : type(Type::Constant), value(value) {}

template<typename T>
Expression<T>::Expression(const std::string& variable) : type(Type::Variable), variable(variable) {}

template<typename T>
Expression<T>::Expression(const Expression& other)
    : type(other.type), value(other.value), variable(other.variable) {
    if (other.left) left = std::make_unique<Expression>(*other.left);
    if (other.right) right = std::make_unique<Expression>(*other.right);
}

template<typename T>
Expression<T>::Expression(Expression&& other) noexcept
    : type(other.type), value(std::move(other.value)), variable(std::move(other.variable)),
      left(std::move(other.left)), right(std::move(other.right)) {}

template<typename T>
Expression<T>::~Expression() {}

template<typename T>
Expression<T>& Expression<T>::operator=(const Expression& other) {
    if (this != &other) {
        type = other.type;
        value = other.value;
        variable = other.variable;
        if (other.left) left = std::make_unique<Expression>(*other.left);
        if (other.right) right = std::make_unique<Expression>(*other.right);
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

template<typename T>
Expression<T> Expression<T>::operator+(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Add;
    result.left = std::make_unique<Expression>(*this);
    result.right = std::make_unique<Expression>(other);
    return result;
}

template<typename T>
Expression<T> Expression<T>::operator-(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Subtract;
    result.left = std::make_unique<Expression>(*this);
    result.right = std::make_unique<Expression>(other);
    return result;
}

template<typename T>
Expression<T> Expression<T>::operator*(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Multiply;
    result.left = std::make_unique<Expression>(*this);
    result.right = std::make_unique<Expression>(other);
    return result;
}

template<typename T>
Expression<T> Expression<T>::operator/(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Divide;
    result.left = std::make_unique<Expression>(*this);
    result.right = std::make_unique<Expression>(other);
    return result;
}

template<typename T>
Expression<T> Expression<T>::operator^(const Expression& other) const {
    Expression result(*this);
    result.type = Type::Power;
    result.left = std::make_unique<Expression>(*this);
    result.right = std::make_unique<Expression>(other);
    return result;
}

template<typename T>
Expression<T> Expression<T>::sin(const Expression& expr) {
    Expression result(expr);
    result.type = Type::Sin;
    result.left = std::make_unique<Expression>(expr);
    return result;
}

template<typename T>
Expression<T> Expression<T>::cos(const Expression& expr) {
    Expression result(expr);
    result.type = Type::Cos;
    result.left = std::make_unique<Expression>(expr);
    return result;
}

template<typename T>
Expression<T> Expression<T>::ln(const Expression& expr) {
    Expression result(expr);
    result.type = Type::Ln;
    result.left = std::make_unique<Expression>(expr);
    return result;
}

template<typename T>
Expression<T> Expression<T>::exp(const Expression& expr) {
    Expression result(expr);
    result.type = Type::Exp;
    result.left = std::make_unique<Expression>(expr);
    return result;
}

template<typename T>
Expression<T> Expression<T>::substitute(const std::string& var, const T& value) const {
    if (type == Type::Variable && variable == var) {
        return Expression(value);
    } else if (left && right) {
        return Expression(type, left->substitute(var, value), right->substitute(var, value));
    } else if (left) {
        return Expression(type, left->substitute(var, value));
    }
    return *this;
}

template<typename T>
T Expression<T>::evaluate(const std::map<std::string, T>& variables) const {
    switch (type) {
        case Type::Constant:
            return value;
        case Type::Variable:
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
    throw std::runtime_error("Unsupported expression type");
}

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
        case Type::Constant:
            return "Constant";
        case Type::Variable:
            return "Variable";
        case Type::Add:
            return "Add";
        case Type::Subtract:
            return "Subtract";
        case Type::Multiply:
            return "Multiply";
        case Type::Divide:
            return "Divide";
        case Type::Power:
            return "Power";
        case Type::Sin:
            return "Sin";
        case Type::Cos:
            return "Cos";
        case Type::Ln:
            return "Ln";
        case Type::Exp:
            return "Exp";
    }
    return "Unknown";
}

template class Expression<double>;
template class Expression<std::complex<double>>;
