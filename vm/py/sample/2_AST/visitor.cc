#include <stdio.h>
#include <vector>

struct Visitor;
struct Dog;
struct Cat;
struct Fox;

struct Animal
{
    virtual void accept(Visitor *v) { printf("emm...\n"); }
};



struct Visitor
{
    void         visit(Animal *and) {}
    virtual void visit(Dog *and) {}
    virtual void visit(Cat *and) {}
    virtual void visit(Fox *and) {}
};

struct Dog : Animal
{
    virtual void accept(Visitor *v) { v->visit(this); }
};
struct Cat : Animal
{
    virtual void accept(Visitor *v) { v->visit(this); }
};
struct Fox : Animal
{
    virtual void accept(Visitor *v) { v->visit(this); }
};


struct Speaker : Visitor
{
    void visit(Animal *pa)
    {
        pa->accept(this);
    }

    virtual void visit(Dog *and)
    {
        printf("wang\n");
    }
    virtual void visit(Cat *and)
    {
        printf("meow\n");
    }
    virtual void visit(Fox *and)
    {

        printf("woo\n");
    }
};

struct Feeding : Visitor
{
    void visit(Animal *pa)
    {
        pa->accept(this);
    }

    virtual void visit(Dog *and)
    {
        printf("bone\n");
    }
    virtual void visit(Cat *and)
    {
        printf("fish\n");
    }
    virtual void visit(Fox *and)
    {

        printf("meat\n");
    }
};


int main()
{
    std::vector<Animal *> animals = {new Dog(), new Cat(), new Fox()};
    Speaker               s;

    for (auto &a : animals) {
        s.visit(a);
    }

    Feeding f;
    for (auto &a : animals) {
        f.visit(a);
    }

    return 0;
}
