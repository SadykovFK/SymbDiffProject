#include "SymDiff.hpp"
#include <cmath>
#include <sstream>
#include <stdexcept>

template <typename T>
struct Expression<T>::Impl {
    virtual ~Impl() = default;
    virtual std::unique_ptr<Impl> clone() const = 0;
    virtual std::string toString() const = 0;
    virtual T evaluate(const std::map<std::string, T>& variables) const = 0;
    virtual std::unique_ptr<Impl> derivative(const std::string& variable) const = 0;
};

template <typename T>
class Constant : public Expression<T>::Impl {
    T value;
public:
    Constant(T value) : value(value) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Constant<T>>(value);
    }
    std::string toString() const override {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    T evaluate(const std::map<std::string, T>&) const override {
        return value;
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string&) const override {
        return std::make_unique<Constant<T>>(0);
    }
};

template <typename T>
class Variable : public Expression<T>::Impl {
    std::string name;
public:
    Variable(const std::string& name) : name(name) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Variable>(name);
    }
    std::string toString() const override {
        return name;
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        return variables.at(name);
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        return std::make_unique<Constant<T>>(variable == name ? 1 : 0);
    }
};

template <typename T>
class Add : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> left, right;
public:
    Add(std::unique_ptr<typename Expression<T>::Impl> left, std::unique_ptr<typename Expression<T>::Impl> right)
        : left(std::move(left)), right(std::move(right)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Add>(left->clone(), right->clone());
    }
    std::string toString() const override {
        return "(" + left->toString() + " + " + right->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        return left->evaluate(variables) + right->evaluate(variables);
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        return std::make_unique<Add>(left->derivative(variable), right->derivative(variable));
    }
};

template <typename T>
class Subtract : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> left, right;
public:
    Subtract(std::unique_ptr<typename Expression<T>::Impl> left, std::unique_ptr<typename Expression<T>::Impl> right)
        : left(std::move(left)), right(std::move(right)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Subtract>(left->clone(), right->clone());
    }
    std::string toString() const override {
        return "(" + left->toString() + " - " + right->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        return left->evaluate(variables) - right->evaluate(variables);
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        return std::make_unique<Subtract>(left->derivative(variable), right->derivative(variable));
    }
};

template <typename T>
class Multiply : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> left, right;
public:
    Multiply(std::unique_ptr<typename Expression<T>::Impl> left, std::unique_ptr<typename Expression<T>::Impl> right)
        : left(std::move(left)), right(std::move(right)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Multiply>(left->clone(), right->clone());
    }
    std::string toString() const override {
        return "(" + left->toString() + " * " + right->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        return left->evaluate(variables) * right->evaluate(variables);
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        auto leftDeriv = left->derivative(variable);
        auto rightDeriv = right->derivative(variable);
        return std::make_unique<Add>(
            std::make_unique<Multiply>(left->clone(), rightDeriv->clone()),
            std::make_unique<Multiply>(leftDeriv->clone(), right->clone())
        );
    }
};

template <typename T>
class Divide : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> left, right;
public:
    Divide(std::unique_ptr<typename Expression<T>::Impl> left, std::unique_ptr<typename Expression<T>::Impl> right)
        : left(std::move(left)), right(std::move(right)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Divide>(left->clone(), right->clone());
    }
    std::string toString() const override {
        return "(" + left->toString() + " / " + right->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        T denominator = right->evaluate(variables);
        if (denominator == 0) {
            throw std::runtime_error("Division by zero");
        }
        return left->evaluate(variables) / denominator;
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        auto leftDeriv = left->derivative(variable);
        auto rightDeriv = right->derivative(variable);
        return std::make_unique<Divide>(
            std::make_unique<Subtract>(
                std::make_unique<Multiply>(leftDeriv->clone(), right->clone()),
                std::make_unique<Multiply>(left->clone(), rightDeriv->clone())
            ),
            std::make_unique<Multiply>(right->clone(), right->clone())
        );
    }
};

template <typename T>
class Power : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> base, exponent;
public:
    Power(std::unique_ptr<typename Expression<T>::Impl> base, std::unique_ptr<typename Expression<T>::Impl> exponent)
        : base(std::move(base)), exponent(std::move(exponent)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Power>(base->clone(), exponent->clone());
    }
    std::string toString() const override {
        return "(" + base->toString() + " ^ " + exponent->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        return std::pow(base->evaluate(variables), exponent->evaluate(variables));
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        auto baseDeriv = base->derivative(variable);
        auto exponentDeriv = exponent->derivative(variable);
        return std::make_unique<Multiply>(
            std::make_unique<Power>(base->clone(), exponent->clone()),
            std::make_unique<Add>(
                std::make_unique<Multiply>(exponentDeriv->clone(), std::make_unique<Ln>(base->clone())),
                std::make_unique<Multiply>(exponent->clone(), std::make_unique<Divide>(baseDeriv->clone(), base->clone()))
            )
        );
    }
};

template <typename T>
class Sin : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> arg;
public:
    Sin(std::unique_ptr<typename Expression<T>::Impl> arg) : arg(std::move(arg)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Sin>(arg->clone());
    }
    std::string toString() const override {
        return "sin(" + arg->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        return std::sin(arg->evaluate(variables));
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        return std::make_unique<Multiply>(
            std::make_unique<Cos>(arg->clone()),
            arg->derivative(variable)
        );
    }
};

template <typename T>
class Cos : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> arg;
public:
    Cos(std::unique_ptr<typename Expression<T>::Impl> arg) : arg(std::move(arg)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Cos>(arg->clone());
    }
    std::string toString() const override {
        return "cos(" + arg->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        return std::cos(arg->evaluate(variables));
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        return std::make_unique<Multiply>(
            std::make_unique<Constant<T>>(-1),
            std::make_unique<Multiply>(
                std::make_unique<Sin<T>>(arg->clone()),
                arg->derivative(variable)
            )
        );
    }
};

template <typename T>
class Ln : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> arg;
public:
    Ln(std::unique_ptr<typename Expression<T>::Impl> arg) : arg(std::move(arg)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Ln>(arg->clone());
    }
    std::string toString() const override {
        return "ln(" + arg->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        T val = arg->evaluate(variables);
        if (val <= 0) {
            throw std::runtime_error("Logarithm of non-positive number");
        }
        return std::log(val);
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        return std::make_unique<Divide>(
            arg->derivative(variable),
            arg->clone()
        );
    }
};

template <typename T>
class Exp : public Expression<T>::Impl {
    std::unique_ptr<typename Expression<T>::Impl> arg;
public:
    Exp(std::unique_ptr<typename Expression<T>::Impl> arg) : arg(std::move(arg)) {}
    std::unique_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_unique<Exp>(arg->clone());
    }
    std::string toString() const override {
        return "exp(" + arg->toString() + ")";
    }
    T evaluate(const std::map<std::string, T>& variables) const override {
        return std::exp(arg->evaluate(variables));
    }
    std::unique_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        return std::make_unique<Multiply>(
            std::make_unique<Exp>(arg->clone()),
            arg->derivative(variable)
        );
    }
};

template <typename T>
Expression<T>::Expression(T value) : pImpl(std::make_unique<Constant<T>>(value)) {}

template <typename T>
Expression<T>::Expression(const std::string& variable) : pImpl(std::make_unique<Variable<T>>(variable)) {}

template <typename T>
Expression<T>::~Expression() = default;

template <typename T>
Expression<T>::Expression(const Expression& other) : pImpl(other.pImpl->clone()) {}

template <typename T>
Expression<T>::Expression(Expression&& other) noexcept : pImpl(std::move(other.pImpl)) {}

template <typename T>
Expression<T>& Expression<T>::operator=(const Expression& other) {
    if (this != &other) {
        pImpl = other.pImpl->clone();
    }
    return *this;
}

template <typename T>
Expression<T>& Expression<T>::operator=(Expression&& other) noexcept {
    if (this != &other) {
        pImpl = std::move(other.pImpl);
    }
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator+(const Expression& other) const {
    return Expression(std::make_unique<Add<T>>(pImpl->clone(), other.pImpl->clone()));
}

template <typename T>
Expression<T> Expression<T>::operator-(const Expression& other) const {
    return Expression(std::make_unique<Subtract<T>>(pImpl->clone(), other.pImpl->clone()));
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression& other) const {
    return Expression(std::make_unique<Multiply<T>>(pImpl->clone(), other.pImpl->clone()));
}

template <typename T>
Expression<T> Expression<T>::operator/(const Expression& other) const {
    return Expression(std::make_unique<Divide<T>>(pImpl->clone(), other.pImpl->clone()));
}

template <typename T>
Expression<T> Expression<T>::operator^(const Expression& other) const {
    return Expression(std::make_unique<Power<T>>(pImpl->clone(), other.pImpl->clone()));
}

template <typename T>
Expression<T> Expression<T>::sin() const {
    return Expression(std::make_unique<Sin<T>>(pImpl->clone()));
}

template <typename T>
Expression<T> Expression<T>::cos() const {
    return Expression(std::make_unique<Cos<T>>(pImpl->clone()));
}

template <typename T>
Expression<T> Expression<T>::ln() const {
    return Expression(std::make_unique<Ln<T>>(pImpl->clone()));
}

template <typename T>
Expression<T> Expression<T>::exp() const {
    return Expression(std::make_unique<Exp<T>>(pImpl->clone()));
}

template <typename T>
std::string Expression<T>::toString() const {
    return pImpl->toString();
}

template <typename T>
Expression<T> Expression<T>::substitute(const std::string& variable, T value) const {
    return Expression(std::make_unique<Constant<T>>(value));
}

template <typename T>
T Expression<T>::evaluate(const std::map<std::string, T>& variables) const {
    return pImpl->evaluate(variables);
}

template <typename T>
Expression<T> Expression<T>::derivative(const std::string& variable) const {
    return Expression(pImpl->derivative(variable));
}