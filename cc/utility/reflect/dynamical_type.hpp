#pragma once

#include "function_traits.hpp"
#include "variable_traits.hpp"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vcruntime_typeinfo.h>
#include <vector>


class Type;
class Numeric;
class Enum;
class Class;


inline std::set<const Type *> &GlobalTypes()
{
    static std::set<const Type *> global_types;
    return global_types;
};


class Type
{
  public:
    template <class T>
    friend class EnumFactory;
    template <class T>
    friend class ClassFactory;

    enum class EKind
    {
        Numeric,
        Enum,
        Class,
    };

  private:
    std::string name;
    EKind       kind;
    // TODO: add reference type


  public:
    Type(const std::string &name, EKind kind) : name(name), kind(kind) {}
    virtual ~Type() = default;

    std::string GetName() const { return name; }
    EKind       GetKind() const { return kind; }

    const Numeric *AsNumeric() const;
    const Enum    *AsEnum() const;
    const Class   *AsClass() const;

    static std::string EKind2String(EKind kind)
    {
        switch (kind) {
        case EKind::Numeric:
            return "Numeric";
        case EKind::Enum:
            return "Enum";
        case EKind::Class:
            return "Class";
        }
    }
};



class Numeric : public Type
{
  public:
    enum class EKind
    {
        Int8,
        Int16,
        Int32,
        Int64,
        Int128,
        Float,
        Double,
        Bool
    };

  private:
    EKind kind;
    bool  bSigned;

  public:

    Numeric(EKind kind, bool bSigned) : Type(GetName(kind), Type::EKind::Numeric), kind(kind), bSigned(bSigned) {}

    bool IsKind(EKind kind) const { return this->kind == kind; }
    bool IsSigned() const { return bSigned; }

    template <typename T>
    static Numeric Create()
    {
        return Numeric{DetectKind<T>(), std::is_signed_v<T>};
    }

    static std::string GetName(EKind kind)
    {
        switch (kind) {
        case EKind::Int8:
            return "Int8";
        case EKind::Int16:
            return "Int16";
        case EKind::Int32:
            return "Int32";
        case EKind::Int64:
            return "Int64";
        case EKind::Int128:
            return "Int128";
        case EKind::Float:
            return "Float";
        case EKind::Double:
            return "Double";
        case EKind::Bool:
            return "Bool";
        }

        return "Unknown";
    }

  private:
    template <typename T>
    static EKind DetectKind()
    {
        using t1 = std::remove_reference_t<T>;

        if constexpr (std::is_same_v<t1, int8_t>) {
            return EKind::Int8;
        }
        else if constexpr (std::is_same_v<t1, int16_t>) {
            return EKind::Int16;
        }
        else if constexpr (std::is_same_v<t1, int32_t>) {
            return EKind::Int32;
        }
        else if constexpr (std::is_same_v<t1, int64_t>) {
            return EKind::Int64;
        }
        else if constexpr (std::is_same_v<t1, signed long long>) {
            return EKind::Int128;
        }
        else if constexpr (std::is_same_v<t1, float>) {
            return EKind::Float;
        }
        else if constexpr (std::is_same_v<t1, double>) {
            return EKind::Double;
        }
        else if constexpr (std::is_same_v<t1, bool>) {
            return EKind::Bool;
        }

        // what type is T?
        std::cerr << typeid(T).name() << std::endl;

        assert(false);
        return (EKind)-1;
    }
};


class Enum : public Type
{

  public:
    struct Item {
        using value_type = int32_t;
        std::string name;
        value_type  value;
    };

  private:
    std::vector<Item> items;

  public:
    Enum() : Type("Unknown-Enum", Type::EKind::Enum) {}
    Enum(const std::string &name) : Type(name, Type::EKind::Enum) {}

    template <typename T>
    void Add(const std::string &name, T value)
    {
        items.emplace_back(Item{name, static_cast<Item::value_type>(value)});
    }

    const std::vector<Item> &GetItems() const { return items; }
};



class Class : public Type
{
  public:
    struct MemberVariable {
        std::string name;
        const Type *type;

        template <typename T>
        static MemberVariable Create(const std::string &name);
    };

    struct MemberFunction {
        std::string               name;
        const Type               *return_type;
        std::vector<const Type *> parameters;

        template <typename T>
        static MemberFunction Create(const std::string &name);

      private:
        template <typename Params, size_t... Idx>
        static std::vector<const Type *> ConvertTypeList2Vector(std::index_sequence<Idx...>);
    };


  private:
    std::vector<MemberFunction> functions;
    std::vector<MemberVariable> variables;

  public:
    Class() : Type("Unknown-Class", Type::EKind::Class) {}
    Class(const std::string &name) : Type(name, Type::EKind::Class) {}

    void AddVariable(MemberVariable &&member) { variables.emplace_back(std::move(member)); }
    void AddFunction(MemberFunction &&member) { functions.emplace_back(std::move(member)); }

    const std::vector<MemberFunction> &GetFunctions() const { return functions; }
    const std::vector<MemberVariable> &GetVariables() const { return variables; }
};



inline const Numeric *Type::AsNumeric() const
{
    if (kind == EKind::Numeric) {
        return static_cast<const Numeric *>(this);
    }
    return nullptr;
}

inline const Enum *Type::AsEnum() const
{
    if (kind == EKind::Enum) {
        return static_cast<const Enum *>(this);
    }
    return nullptr;
}

inline const Class *Type::AsClass() const
{
    if (kind == EKind::Class) {
        return static_cast<const Class *>(this);
    }
    return nullptr;
}


#pragma endregion type



template <class T>
class NumericFactory final
{

    Numeric info;

  public:
    static NumericFactory &Instance()
    {
        static NumericFactory instance{Numeric::Create<T>()};
        static bool           bInitialized = false;
        if (!bInitialized) {
            GlobalTypes().insert(&instance.Info());
            bInitialized = true;
        }
        return instance;
    }

    NumericFactory(Numeric &&info) : info(std::move(info)) {}
    const Numeric &Info() const { return info; }

    // void Unregister()
    // {
    //      TODO
    //     info = Numeric();
    // }
};


template <class T>
class EnumFactory final
{
    Enum info;

  public:
    static EnumFactory &Instance()
    {
        static EnumFactory instance;
        static bool        bInitialized = false;
        if (!bInitialized) {
            GlobalTypes().insert(&instance.Info());
            bInitialized = true;
        }
        return instance;
    }
    const Enum &Info() const { return info; }


    EnumFactory &Register(const std::string &name)
    {
        info.name = name;
        return *this;
    }

    template <typename U>
    EnumFactory &Add(const std::string &name, U value)
    {
        info.Add(name, value);
        return *this;
    }

    void Unregister()
    {
        // TODO
    }
};

template <class T>
class ClassFactory final
{
    Class info;

  public:
    static ClassFactory &Instance()
    {
        static ClassFactory instance;
        static bool         bInitialized = false;
        if (!bInitialized) {
            GlobalTypes().insert(&instance.Info());
            bInitialized = true;
        }
        return instance;
    }
    const Class &Info() const { return info; }


    ClassFactory &Register(const std::string &name)
    {
        info.name = name;
        return *this;
    }

    template <typename U>
    ClassFactory &AddVariable(const std::string &name)
    {
        info.AddVariable(Class::MemberVariable::Create<U>(name));
        return *this;
    }

    template <typename U>
    ClassFactory &AddFunction(const std::string &name)
    {
        info.AddFunction(Class::MemberFunction::Create<U>(name));
        return *this;
    }
};



class TrivialFactory
{
  public:
    static TrivialFactory &Instance()
    {
        static TrivialFactory instance;
        return instance;
    }
};



template <typename T>
class Factory final
{
  public:
    static auto &GetFactory()
    {
        using t1 = std::remove_reference_t<T>;

        if constexpr (std::is_fundamental_v<t1>) {
            return NumericFactory<T>::Instance();
        }
        else if constexpr (std::is_enum_v<t1>) {
            return EnumFactory<T>::Instance();
        }
        else if constexpr (std::is_class_v<t1>) {
            return ClassFactory<T>::Instance();
        }
        else {
            return TrivialFactory::Instance();
        }
    }
};


template <typename ValueType>
auto &Register()
{
    return Factory<ValueType>::GetFactory();
}

template <typename T>
const Type *GetType()
{
    return &Factory<T>::GetFactory().Info();
}

inline const Type *GetType(std::string_view name)
{
    for (auto type : GlobalTypes()) {
        if (type->GetName() == name) {
            return type;
        }
    }
    return nullptr;
}



template <typename T>
inline Class::MemberFunction Class::MemberFunction::Create(const std::string &name)
{
    using traits     = function_traits<T>;
    using parameters = typename traits::parameters;
    return MemberFunction{
        name,
        // TODO: void type
        GetType<typename traits::return_type>(),
        ConvertTypeList2Vector<parameters>(
            std::make_index_sequence<std::tuple_size_v<parameters>>()),
    };
}

template <typename T>
inline Class::MemberVariable Class::MemberVariable::Create(const std::string &name)
{
    using type = typename variable_traits<T>::type;
    return MemberVariable{name, GetType<type>()};
}

template <typename Params, size_t... Idx>
inline std::vector<const Type *> Class::MemberFunction::ConvertTypeList2Vector(std::index_sequence<Idx...>)
{
    // clang-format off
    return {
        GetType<
            std::tuple_element_t<Idx, Params>
            >
        () ...};
    // clang-format on
}



// TODO: wrapping in debug preprocessor
#include "ut.hpp"

enum class EMyEnum
{
    V1 = 1,
    V2 = 2,
};

struct MyClass {
    int  v1 = 1;
    int  f1() { return 1; };
    void f2() {}
    int  f3(int &i) { return 2; }
};



inline void dynamic_function_test()
{
    Register<EMyEnum>()
        .Register("EMyEnum")
        .Add("V1", EMyEnum::V1)
        .Add("V2", EMyEnum::V2);

    // auto const *typeinfo = GetType<EMyEnum>();
    auto const *typeinfo = GetType("EMyEnum");
    debug(), "1,", typeinfo->GetName();

    auto enum_type = typeinfo->AsEnum();
    debug(), "2,", enum_type->GetName();
    for (auto item : enum_type->GetItems()) {
        debug(), item.name, " = ", item.value;
    }

    Register<MyClass>()
        .Register("MyClass")
        .AddVariable<decltype(&MyClass::v1)>("v1")
        .AddFunction<decltype(&MyClass::f1)>("f1")
        // TODO for void, std::is_fundamental_v<void> return true?
        //  1. add void to numeric type
        // 2. add void type and void_factory
        // .AddFunction<decltype(&MyClass::f2)>("f2")
        .AddFunction<decltype(&MyClass::f3)>("f3");

    // using t = function_traits<decltype(&MyClass::f1)>::return_type;

    // auto type = GetType<MyClass>();
    auto type = GetType("MyClass");
    debug(), "1,", type->GetName();
    auto class_type = type->AsClass();
    debug(), "2,", class_type->GetName();
    for (auto item : class_type->GetVariables()) {
        debug(), item.name;
    }
    for (auto item : class_type->GetFunctions()) {
        auto d = debug();
        d, item.return_type->GetName(), " ";
        d, item.name, " (";
        for (auto param : item.parameters) {
            d, param->GetName(), ",";
        }
        d, ")";
    }
}