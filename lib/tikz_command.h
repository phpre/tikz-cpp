#pragma once

#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz.h"

namespace TIKZ {
    struct command {
        std::set<u32>         m_time;      // time points at which this command is active
        std::set<std::string> m_libraries; // dependencies
        kv_store              m_options;

        virtual void print( FILE* p_out, u32 p_time = 0, u32 p_startIndent = 1, u32 p_indent = 4 )
            = 0;
    };

    struct node_command : public command {
        tikz_position m_position;
        std::string   m_name    = EMPTY_STR;
        std::string   m_content = EMPTY_STR;

        static inline node_command place_text( const std::string& p_text, tikz_position p_position,
                                               color           p_textColor,
                                               const kv_store& p_options = { } ) {
            return { };
        }
    };

    struct path_command : public command {};

    struct math_command : public command {};

    struct scope_command : public command {};
} // namespace TIKZ
