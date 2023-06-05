#include <application.h>


class Triangle : public Application
{
  public:
    void BeginPlay() override
    {
        super::BeginPlay();
    }

    void OnEvent() override
    {
        super::OnEvent();
    }

    void Tick(uint32_t DeltaT) override
    {
        super::Tick(DeltaT);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(2, 1, 1);
        glPushMatrix();
        glScaled(0.5, 0.5, 0.5);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(-1, -1, 0);
            glVertex3f(-1, 1, 0);
            glVertex3f(1, 1, 0);
        }
        glEnd();
        glPopMatrix();
    }
};

int main()
{
    Triangle t;
    t.Run();
}
