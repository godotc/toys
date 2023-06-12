
#pragma once

#include <filesystem>
#include <map>
#include <string>

#include <glad/glad.h>

#include "shader.h"
#include "texture.h"

struct ShaderProgramSource
{
    std::string VertexSource, FragmentSource, GemoetySource;
};


class ResourceManager
{
  public:

    static Shader  LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    static Shader  LoadShader(const char *UniversalFileName, const char *name);
    static Shader &GetShader(const char *name);

    static Texture2D  LoadTexture(const char *file, bool alpha, std::string name);
    static Texture2D &GetTexture(std::string name);

    static void Clear();

  private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() = default;

    static Shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    static Shader    loadShaderFromFile(std::filesystem::path UniversalFilePath);
    static Texture2D loadTextureFromFile(const char *file, bool alpha);

    static ShaderProgramSource parseShaderFile(const std::filesystem::path &UniversalFilePath);

  public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
};
