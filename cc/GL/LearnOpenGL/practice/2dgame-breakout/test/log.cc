#include "../src/log.h"


int main()
{
    Logger::Get()->log("hello{} {}", "world", "bb");
}
