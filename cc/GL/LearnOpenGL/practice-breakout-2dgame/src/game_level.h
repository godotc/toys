#include "render/game_object.h"
#include "render/sprite_render.h"
#include <cstdint>
#include <vector>


class GameLevel
{
    using uint = unsigned int;

  public:
    GameLevel() = default;

  public:
    void Load(const char *file, uint level_width, uint level_height);
    void Draw(SpriteRender &render);

    bool IsCompleted();

  public:
    std::vector<GameObject> Bricks;

  private:
    void init(std::vector<std::vector<uint>> tile_data, uint level_width, uint level_height);
};
