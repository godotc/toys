#pragma once

#include <filesystem>
// #include <experimental/filesystem>
// #endif
#include <iterator>
#include <map>
#include <memory>
#include <string>

#include <glad/glad.h>

#include "__microshit_api_gltk.h"
#include "shader.h"
#include "sound.h"
#include "texture.h"



struct ShaderProgramSource {
    std::string VertexSource, FragmentSource, GemoetySource;
};


class SDL_AUDIO;

class GLTK_API ResourceManager //: public utils::disable_copy_and_move
{
  public:

    // static void PlaySound(const char *name);
    // static void LoadSound(const char *file, std::string name);


	// shader from ".glsl" file and split by `#shader vertex/fragment/geometry`
    static Shader  LoadShader(const char *universal_file, const char *name);
    static Shader  LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    static Shader &GetShader(const char *name);

    static Texture2D  LoadTexture(const char *file, std::string name);
    static Texture2D  GetTexture(std::string name);
    static Texture2D &GetTextureRef(std::string name);

    static void Clear();

  private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() = default;

    // static auto GetAudio() -> Audio *;

    static Shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    static Shader    loadShaderFromFile(std::filesystem::path UniversalFilePath);
    static Texture2D loadTextureFromFile(const char *file);

    static ShaderProgramSource parseShaderFile(const std::filesystem::path &UniversalFilePath);

  public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string, Sound>     Sounds;
};
