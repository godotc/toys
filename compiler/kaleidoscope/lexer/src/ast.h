#pragma once

#include <future>
#include <memory>
#include <string>
#include <vector>

class ExprAST
{
  public:
    virtual ~ExprAST() = default;

    virtual std::string to_string() { return ""; }
};

class NumberExprAST
    : public ExprAST
{
    double Val;

  public:
    NumberExprAST(double val) : Val(val) {}
    std::string to_string() override { return std::to_string(Val); }
};

class VariableExprAST
    : public ExprAST
{
    std::string Name;

  public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
    std::string to_string() override { return Name; }
};

// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST
    : public ExprAST
{
    char                     Op;
    std::unique_ptr<ExprAST> LHS, RHS;

  public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    std::string to_string() override { return LHS->to_string() + Op + RHS->to_string(); }
};

// CallExprAST - Expression class for function calls.
class CallExprAST
    : public ExprAST
{
    std::string                           Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

  public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args) : Callee(Callee), Args(std::move(Args)) {}

    std::string to_string() override
    {
        auto ret = Callee;
        ret.push_back('(');
        for (auto &a : Args)
        {
            ret += a->to_string();
        }
        ret.pop_back();
        ret.push_back(')');
        return ret;
    }
};

class PrototypeAST
{
    std::string              Name;
    std::vector<std::string> Args;

  public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args) : Name(Name), Args(std::move(Args)) {}

    const std::string &getName() const { return Name; }
    std::string        to_string()
    {
        auto ret = "def " + getName() + "(";
        for (auto &a : Args) {
            ret += a + ',';
        }
        ret.pop_back();
        ret.push_back(')');
        return ret;
    }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST
{
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST>      Body;

  public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST>      Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}

    std::string to_string()
    {
        auto ret = Proto->to_string();
        ret.push_back(' ');
        ret += Body->to_string();
        return ret;
    }
};
