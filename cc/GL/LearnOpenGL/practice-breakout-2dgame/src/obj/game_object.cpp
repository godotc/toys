#include "game_object.h"



GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), IsSolid(false), IsDestroyed(false), Sprite() {}

GameObject::GameObject(vec2 pos, vec2 size, Texture2D &sprite, vec3 color, vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), IsSolid(false), IsDestroyed(false), Sprite(sprite) {}

void GameObject::Draw(SpriteRender &render)
{
    render.DrawSprite(Sprite, Position, Size, Rotation, Color);
}
