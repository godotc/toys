#pragma once

#include "fame_buffer.h"
namespace toplevel_namespace {

class Window
{
  public:
    void main()
    {
        // system("clear");
        buffer.CleanBuffer();
        buffer.Display();
    }

  private:
    FrameBuffer buffer;
};



} // namespace toplevel_namespace
