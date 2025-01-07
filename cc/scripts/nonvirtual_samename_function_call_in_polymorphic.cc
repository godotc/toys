

#include <stdio.h>


#define A__FUNC __PRETTY_FUNCTION__

class Parent
{
    auto call_1()
    {
        printf(A__FUNC);
    }

  protected:
    auto call_2()
    {
        printf(A__FUNC);
    }

  public:
    auto call_3()
    {
        printf(A__FUNC);
    }
};

class Child : public Parent
{
    auto call_1()
    {
        printf(A__FUNC);
    }

  protected:
    auto call_2()
    {
        printf(A__FUNC);
    }

  public:
    auto call_3()
    {
        printf(A__FUNC);
    }
};



int main()
{
    Parent *p = new Child;
    p->call_3();
}
