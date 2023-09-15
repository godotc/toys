#pragma once

#include "log.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>


#include <glad/glad.h>

namespace gltk {

struct GLExtensions {

    static GLExtensions &Get()
    {
        static auto ptr = new GLExtensions();
        return *ptr;
    }

    bool is_extension_supported(const char *ext_name)
    {
        if (m_extensions.size() == 0) {
            reload();
        }
        return m_extensions.find(ext_name) != m_extensions.end();
    }


    void reload()
    {
        GLint num_extensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
        LOG_DEBUG("Supprt {} extensions: ", num_extensions);

        for (GLint i = 0; i < num_extensions; ++i) {
            m_extensions.insert(
                (const char *)glGetStringi(GL_EXTENSIONS, i));
        }
    }

    void print_all_extensions()
    {
        reload();
        int i = 0;
        for (auto &ext : m_extensions) {
            std::cout << fmt::format("{}: {}\n", i++, ext);
        }
    }


    std::set<std::string> m_extensions;
};



} // namespace gltk
