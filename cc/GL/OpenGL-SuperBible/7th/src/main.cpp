#include "SDL_video.h"
#include <application.h>
#include <iostream>


int main(int argc, char **argv)
{
    using namespace std;
    cout << "hello world!" << endl;

    Application app("Hello", 800, 600, 0);
    app.Run();

    return 0;
}
