#pragma once

#include <map>
#include <string>

#define PRINT_MACRO \
    if( p_print ) printf

typedef unsigned long                      u32;
typedef signed long                        s32;
typedef std::pair<u32, u32>                point;
typedef std::map<std::string, std::string> kv_store;

constexpr std::string EMPTY_STR = "";

#define NO_OPTS \
    {}

constexpr std::string WILDCARD = "*";
