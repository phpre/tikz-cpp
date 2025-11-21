#pragma once

#include <string>

#define PRINT_MACRO \
    if( p_print ) printf

typedef uint_fast8_t        u8;
typedef int_fast8_t         s8;
typedef uint_fast16_t       u16;
typedef int_fast16_t        s16;
typedef uint_fast32_t       u32;
typedef int_fast32_t        s32;
typedef uint_fast64_t       u64;
typedef int_fast64_t        s64;
typedef std::pair<u64, u64> point;

constexpr std::string EMPTY_STR = "";
constexpr std::string WILDCARD  = "*";

#define NO_OPTS \
    {}
