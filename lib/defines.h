#pragma once

#include <deque>
#include <map>
#include <string>

#define PRINT_MACRO \
    if( p_print ) printf

typedef unsigned long                      u32;
typedef signed long                        s32;
typedef std::pair<u32, u32>                point;
typedef std::map<std::string, std::string> kv_store;

constexpr std::string EMPTY_STR = "";
constexpr std::string WILDCARD  = "*";

#define NO_OPTS \
    {}

inline std::string kv_to_string( const kv_store& p_kv ) {
    std::string res = "";
    for( const auto& [ k, v ] : p_kv ) {
        res += k;
        if( v != EMPTY_STR ) { res += " = " + v; }
        res += ",";
    }
    return res;
}
