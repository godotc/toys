
#pragma once


#include "log.h"
#include "pyvm/core/code_object.hpp"
#include "pyvm/io/buffer_input_stream.hpp"
#include "pyvm/pyvm.h"

PYVM_TOP_LEVEL_NAMESPACE_BEGIN

class BinaryFileParser
{
  public:
    BinaryFileParser(BufferInputStream *stream) { m_Filestream = stream; }

  public:
    CodeObject *parse()
    {
        int magic_number = m_Filestream->read_int();
        LOG_DEBUG("magic number is 0x{:x}", magic_number);
        // m_Filestream->read_int();

        int mod_date = m_Filestream->read_int();
        LOG_DEBUG("mod date is 0x{:x}", mod_date);

        char object_type = m_Filestream->read();
        if (object_type == 'c') {
            CodeObject *result = get_code_object();
            LOG_TRACE("parse successfully!\n");
            return result;
        }

        return nullptr;
    }


  private:
    CodeObject *get_code_object() {}


  private:
    BufferInputStream *m_Filestream;
};


PYVM_TOP_LEVEL_NAMESPACE_END
