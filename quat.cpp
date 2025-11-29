/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "quat.hpp"
#include <pmmintrin.h>
#include <xmmintrin.h>

#ifdef _MSVC_VER
static const quat quat_identity = { 0.0f, 0.0f, 0.0f, 1.0f };
#else
const quat quat_identity = { 0.0f, 0.0f, 0.0f, 1.0f };
#endif
