#pragma once
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz_point.h"
#include "tikz_command.h"

namespace TIKZ {
    class picture {
        std::set<std::string> _packages;
        std::set<std::string> _libraries;

        kv_store _globalMacros;
        kv_store _localMacros;
        kv_store _options;

        std::deque<std::unique_ptr<command>> _content;

      public:
        picture( kv_store p_options = { } );

        void add_package( const std::string& p_package );
        void add_library( const std::string& p_library );
        void add_global_macro( const std::string& p_name, const std::string& p_content );
        void add_local_macro( const std::string& p_name, const std::string& p_content );

        void add_coordinate( const std::string& p_position, const std::string& p_name,
                             const kv_store& p_options = NO_OPTS );
        void add_coordinate( tikz_point p_position, const std::string& p_name,
                             const kv_store& p_options = NO_OPTS );

        void add_node( const std::string& p_position, const std::string& p_content = EMPTY_STR,
                       const kv_store& p_options = NO_OPTS, const std::string& p_name = EMPTY_STR );
        void add_node( tikz_point p_position, const std::string& p_content = EMPTY_STR,
                       const kv_store& p_options = NO_OPTS, const std::string& p_name = EMPTY_STR );

        void add_connection( const std::string& p_topLeft, const std::string& p_bottomRight,
                             const std::string& p_connection = LINE_CONN,
                             const kv_store&    p_options    = NO_OPTS );
        void add_connection( tikz_point p_topLeft, tikz_point p_bottomRight,
                             const std::string& p_connection = LINE_CONN,
                             const kv_store&    p_options    = NO_OPTS );

        void add_text( const std::string& p_text, tikz_point p_position,
                       const kv_store& p_options = NO_OPTS, color p_textColor = COLOR_TEXT );

        void add_cross( tikz_point p_position, color p_color = COLOR_TEXT );

        void add_debug_point( tikz_point p_position );

        void print( FILE* p_out, u32 p_time = 0, u32 p_startIndent = 1, u32 p_indent = 4 );
    };
} // namespace TIKZ
