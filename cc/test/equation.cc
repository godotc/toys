#include <type_traits>

// 定义数据 type 类
enum Type
{
    TYPE_1,
    TYPE_2,
    TYPE_3
};

// 自定义数据类型
class Foo
{
  public:
    Type type = TYPE_1;


    bool operator=(Type type)
    {
        if (type == TYPE_1)
            return true;
        else
            return false;
    }
};
class Bar
{
  public:
    Type type = TYPE_2;

    bool operator=(Type type)
    {
        if (type == TYPE_2)
            return true;
        else
            return false;
    }
};


template <typename T>
struct type_traits
{
    typename Type type = T::type;
};

template <typename T>
void decode(const T &data, char *buf)
{
    if (type_traits<T>::type == Type::TYPE_1) {
    }
    else if (type_traits<T>::type == Type::TYPE_2) {
    }
}

int main()
{

    Foo   foo = Foo();
    char *str = new char[6];

    decode<Foo>(foo, str);
    return 0;
}
