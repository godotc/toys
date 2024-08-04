#pragma once


#include "dynamical_type.hpp"
#include "ut.hpp"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <optional>
#include <ranges>
#include <type_traits>


class Any
{
  public:
    template <class T>
    friend class operations_traits;

    enum class EStorageType
    {
        Empty,
        Owned,
        Stolen,
        Ref,
        ConstRef,
    };

    struct Operations {
        std::function<Any(const Any &)> copy;
        std::function<Any(Any &)>       steal;
        std::function<void(Any &)>      release;
    };

  private:
#if __SSO
    union
    {
        void *payload;
        char  small_size_object[1024]; // SSO
    };
#else
    void *payload;
#endif
    Type const  *typeinfo;
    EStorageType storage_type = EStorageType::Empty;
    Operations   operations;

  public:
    template <class T>
    static auto make_copy(const T &other);

    template <class T>
    static auto make_steal(T &&other);

    template <class T>
    static auto make_ref(T &other);

    template <class T>
    static auto make_const_ref(const T &other);

    Any() = default;

    // Copy constructor
    Any(const Any &other)
    {
        typeinfo     = other.typeinfo;
        storage_type = other.storage_type;
        operations   = other.operations;
        if (operations.copy) {
            // payload = operations.copy(other).payload;
            auto new_any               = operations.copy(other);
            payload                    = new_any.payload;
            new_any.payload            = nullptr; //
            new_any.operations.release = nullptr;
        }
        else {
            storage_type = EStorageType::Empty;
            typeinfo     = nullptr;
        }
    }

    // Move constructor
    Any(Any &&other) // noexcept
    {
        typeinfo     = std::move(other.typeinfo);
        payload      = std::move(other.payload);
        storage_type = std::move(other.storage_type);
        operations   = std::move(other.operations);
    }

    // Copy assignment
    Any &operator=(const Any &other)
    {
        if (this != &other) {
            typeinfo     = other.typeinfo;
            storage_type = other.storage_type;
            operations   = other.operations;

            if (operations.copy) {
                auto new_any               = operations.copy(other);
                payload                    = new_any.payload;
                new_any.payload            = nullptr;
                new_any.operations.release = nullptr;
            }
            else {
                storage_type = EStorageType::Empty;
                typeinfo     = nullptr;
            }
        }
        return *this;
    }


    // Move assignment
    Any &operator=(Any &&other) // noexcept
    {
        if (this != &other) {
            typeinfo     = std::move(other.typeinfo);
            payload      = std::move(other.payload);
            storage_type = std::move(other.storage_type);
            operations   = std::move(other.operations);
        }
        return *this;
    }


    ~Any()
    {
        if (operations.release && (storage_type == EStorageType::Owned || storage_type == EStorageType::Stolen)) {
#ifndef NODEBUG
            printf("Any::~Any() deallocate payload, storage_type: %s\n", StorageType2String(storage_type).c_str());
#endif
            operations.release(*this);
        }
        else {
            // #ifndef NODEBUG
            //             printf("Any::~Any() %s -> Do not deallocate\n", StorageType2String(storage_type).c_str());
            // #endif
        }
    }

    // temp
    const void  *GetPayload() const { return payload; }
    const Type  *GetTypeInfo() const { return typeinfo; }
    EStorageType GetStorageType() const { return storage_type; }

    template <class T>
    T *as() const
    {
        if (typeinfo == GetType<T>()) {
            return static_cast<T *>(payload);
        }
        return nullptr;
    }


    static std::string StorageType2String(EStorageType type)
    {
        switch (type) {
        case EStorageType::Empty:
            return "Empty";
        case EStorageType::Owned:
            return "Owned";
        case EStorageType::Stolen:
            return "Stolen";
        case EStorageType::Ref:
            return "Ref";
        case EStorageType::ConstRef:
            return "ConstRef";
            break;
        }
    }


  private:
};

template <class T>
struct operations_traits {
    static Any copy(const Any &elem)
    {
        assert(elem.typeinfo == GetType<T>());

        Any ret;
        ret.payload      = new T{*static_cast<T *>(elem.payload)}; // erase type
        ret.typeinfo     = elem.typeinfo;
        ret.storage_type = Any::EStorageType::Owned;
        ret.operations   = elem.operations;
        return ret;
    }

    static Any steal(Any &elem)
    {
        assert(elem.typeinfo == GetType<T>());
        Any ret;

#if __SSO
        if constexpr (sizeof(T) <= sizeof(Any::small_size_object)) {
            // Emplacement New:  call the construct on a memory that already allocated which on stack
            new (ret.small_size_object) T{*static_cast<T *>(other.payload)};
        }
        else {
            ret.payload = new T{*static_cast<T *>(other.payload)};
        }
#else
        ret.payload = new T{std::move(*static_cast<T *>(elem.payload))}; // erase type
#endif
        ret.typeinfo      = elem.typeinfo;
        ret.storage_type  = Any::EStorageType::Owned;
        elem.storage_type = Any::EStorageType::Stolen; // update origin's data state
        ret.operations    = elem.operations;
        return ret;
    }


    static void release(Any &elem)
    {
        assert(elem.typeinfo == GetType<T>());

        delete static_cast<T *>(elem.payload); // TODO: maybe force convert (void*)
        elem.payload      = nullptr;
        elem.storage_type = Any::EStorageType::Empty;
        elem.typeinfo     = nullptr;
    }
};


// TODO: little object optimized for int short char long ....
template <class T>
inline auto Any::make_copy(const T &other)
{
    Any ret;
    ret.payload      = new T{other};
    ret.typeinfo     = GetType<T>();
    ret.storage_type = EStorageType::Owned;
    if constexpr (std::is_copy_constructible_v<T>) {
        ret.operations.copy = operations_traits<T>::copy;
    }
    if constexpr (std::is_move_constructible_v<T>) {
        ret.operations.steal = operations_traits<T>::steal;
    }
    if constexpr (std::is_destructible_v<T>) {
        ret.operations.release = operations_traits<T>::release;
    }
    return ret;
}

template <class T>
inline auto Any::make_steal(T &&other)
{
    using t1 = std::remove_cv_t< // remove const and volatile
        std::remove_reference_t<T>>;
    Any ret;
    ret.payload      = new t1{std::move(other)};
    ret.typeinfo     = GetType<t1>();
    ret.storage_type = EStorageType::Owned;
    if constexpr (std::is_copy_constructible_v<t1>) {
        ret.operations.copy = operations_traits<t1>::copy;
    }
    if constexpr (std::is_move_constructible_v<t1>) {
        ret.operations.steal = operations_traits<t1>::steal;
    }
    if constexpr (std::is_destructible_v<t1>) {
        ret.operations.release = operations_traits<t1>::release;
    }
    return ret;
}

template <class T>
inline auto Any::make_ref(T &other)
{
    Any ret;
    ret.payload      = &other; // pointer to this deref value
    ret.typeinfo     = GetType<T>();
    ret.storage_type = EStorageType::Ref;
    if constexpr (std::is_copy_constructible_v<T>) {
        ret.operations.copy = operations_traits<T>::copy;
    }
    if constexpr (std::is_move_constructible_v<T>) {
        ret.operations.steal = operations_traits<T>::steal;
    }
    if constexpr (std::is_destructible_v<T>) {
        ret.operations.release = operations_traits<T>::release;
    }
    return ret;
}

template <class T>
inline auto Any::make_const_ref(const T &other)
{
    Any ret;
    // ret.payload      = static_cast<void *>(&other); // also pointer to this deref value
    ret.payload      = (void *)(&other); // also pointer to this deref value
    ret.typeinfo     = GetType<T>();
    ret.storage_type = EStorageType::ConstRef;
    if constexpr (std::is_copy_constructible_v<T>) {
        ret.operations.copy = operations_traits<T>::copy;
    }
    if constexpr (std::is_move_constructible_v<T>) {
        ret.operations.steal = operations_traits<T>::steal;
    }
    if constexpr (std::is_destructible_v<T>) {
        ret.operations.release = operations_traits<T>::release;
    }
    return ret;
}



struct Person3 {
    std::string last_name;
    float       height;
    // bool        is_transaxle;
    bool is_female;

    Person3() { printf("Person3 default construct\n"); }

    // Copy
    Person3(const Person3 &other) : last_name(other.last_name), height(other.height), is_female(other.is_female)
    {
        printf("Person3 copy\n");
    }
    // Move
    Person3(Person3 &&other) : last_name(std::move(other.last_name)), height(other.height), is_female(other.is_female)
    {
        printf("Person3 move/steal\n");
    }
    // Copy assign
    Person3 &operator=(const Person3 &other)
    {
        last_name = other.last_name;
        height    = other.height;
        is_female = other.is_female;
        printf("Person3 copy assign\n");
        return *this;
    }

    ~Person3()
    {
        printf("Person3 destruct\n");
    }



    void Sleep() const
    {
        printf("Zzzzz....\n");
    }
    // bool IsTransaxle() const { return is_transaxle; }
    bool IsFemale() const
    {
        printf("???....\n");
        return is_female;
    }
    bool GetMarriedWith(Person3 other)
    {
        if (other.is_female != is_female) //&& is_transaxle != other.is_transaxle;)
        {
            if (is_female) {
                last_name = "Mrs." + other.last_name;
            }
            else {
                last_name = "Mr." + last_name;
            }
            return true;
        }
        return false;
    }
};


inline void any_test()
{
    Register<Person3>()
        .Register("Person3")
        .AddVariable<decltype(&Person3::height)>("height")
        .AddFunction<decltype(&Person3::GetMarriedWith)>("GetMarriedWith");

    {
        Person3 p;
        p.last_name = "John";
        p.height    = 1.75f;
        p.is_female = true;
        Any a       = Any::make_copy(p);

        // debug(), Any::StorageType2String(a.GetStorageType()), " ", a.GetTypeInfo()->GetName(), " ", (int)a.GetTypeInfo()->GetKind();
        // auto b = a.GetPayload();
        // auto c = (Person3 *)b;
        // debug(), c->last_name, " ", c->height, " ", c->is_female;
    }
    debug(), "------------------";
    {
        Person3 p;
        p.last_name = "John";
        p.height    = 1.75f;
        p.is_female = true;
        Any a       = Any::make_steal(p);

        debug(), Any::StorageType2String(a.GetStorageType()), " ", a.GetTypeInfo()->GetName(), " ", (int)a.GetTypeInfo()->GetKind();
        auto b = a.GetPayload();
        auto c = (Person3 *)b;
        debug(), c->last_name, " ", c->height, " ", c->is_female;
    }
    debug(), "------------------";
    {
        Person3 p;
        p.last_name = "John";
        p.height    = 1.75f;
        p.is_female = true;
        Any a       = Any::make_ref(p);

        debug(), Any::StorageType2String(a.GetStorageType()), " ", a.GetTypeInfo()->GetName(), " ", Type::EKind2String(a.GetTypeInfo()->GetKind());
        auto b = a.GetPayload();
        auto c = (Person3 *)b;
        debug(), c->last_name, " ", c->height, " ", c->is_female;
    }
    debug(), "------------------";
    {
        Person3 p;
        p.last_name = "John";
        p.height    = 1.75f;
        p.is_female = true;
        Any a       = Any::make_const_ref(p);


        debug(), Any::StorageType2String(a.GetStorageType()), " ", a.GetTypeInfo()->GetName(), " ", Type::EKind2String(a.GetTypeInfo()->GetKind());
        auto b = a.GetPayload();
        auto c = (Person3 *)b;
        debug(), c->last_name, " ", c->height, " ", c->is_female;


        auto e = a.as<Person3>();
        assert(e != nullptr);
        debug(), e->last_name, " ", e->height, " ", e->is_female;
    }
}