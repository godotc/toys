class A
{
  public:
    void do_somthing();
};

class B
{
  public:
    void something_to_do();
};

class Adaptor : public A
{
    B b;

    void do_somthing()
    {
        b.something_to_do();
    }
};
