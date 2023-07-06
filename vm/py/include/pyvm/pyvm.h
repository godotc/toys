#include "microshit_pyvm_api.h"

#pragma once

#define pyvm_top_level_namespace pyvm


#define PYVM_TOP_LEVEL_NAMESPACE_BEGIN \
    namespace pyvm_top_level_namespace {

#define PYVM_SUB_LEVEL_NAMESPACE_BEGIN(x) \
    namespace pyvm_top_level_namespace::x {

#define PYVM_TOP_LEVEL_NAMESPACE_END }
#define PYVM_SUB_LEVEL_NAMESPACE_END }
