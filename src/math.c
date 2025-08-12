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

vec2f vec2u_tof(vec2u v) { return (vec2f){(f32)v.x, (f32)v.y }; } 
vec2f vec2i_tof(vec2i v) { return (vec2f){(f32)v.x, (f32)v.y }; } 
vec2u vec2f_tou(vec2f v) { return (vec2u){(u32)v.x, (u32)v.y }; } 
vec2u vec2i_tou(vec2i v) { return (vec2u){(u32)v.x, (u32)v.y }; } 
vec2i vec2f_toi(vec2f v) { return (vec2i){(i32)v.x, (i32)v.y }; } 
vec2i vec2u_toi(vec2u v) { return (vec2i){(i32)v.x, (i32)v.y }; } 
