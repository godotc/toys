#include <stdio.h>

#define DYNAMIC_BIND

struct Animal
{
#ifdef DYNAMIC_BIND
    virtual
#endif
        void
        speak()
    {
        printf("emm...\n");
    };
};
struct Cat : Animal
{
    void speak() { printf("meow...\n"); };
};
struct Dog : Animal
{
    void speak() { printf("wang...\n"); };
};
struct Wolf : Animal
{
    void speak() { printf("Woo...\n"); };
};


struct Speaker
{
    void spear(Animal *a) { a->speak(); }
};


int main()
{
    Animal *a = new Dog();
    Animal *b = new Cat();
    Animal *c = new Wolf();

    Speaker *s = new Speaker();

    // will all be the `emm...` if not virtual func
    s->spear(a);
    s->spear(b);
    s->spear(c);

    return 0;
}
