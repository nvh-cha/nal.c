#include <utils.h>

#define __VEC2OP_IMPL__(T) \
  vec2##T vec2##T##_add(vec2##T a, vec2##T b) { \
    return (vec2##T) { .x = a.x+b.x, .y = a.y+b.y }; \
  } \
  vec2##T vec2##T##_sub(vec2##T a, vec2##T b) { \
    return (vec2##T) { .x = a.x-b.x, .y = a.y-b.y }; \
  } \
  vec2##T vec2##T##_mul(vec2##T a, vec2##T b) { \
    return (vec2##T) { .x = a.x*b.x, .y = a.y*b.y }; \
  } \
  vec2##T vec2##T##_div(vec2##T a, vec2##T b) { \
    return (vec2##T) { .x = a.x/b.x, .y = a.y/b.y }; \
  } \

__VEC2OP_IMPL__(f);
__VEC2OP_IMPL__(u);
__VEC2OP_IMPL__(i);
