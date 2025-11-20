#pragma once

#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz_picture.h"
#include "tikz_point.h"

namespace TIKZ {
    class document {
        std::set<std::string> _packages;
        std::set<std::string> _libraries;

        std::map<std::string, std::string> _globalStyles;

        kv_store            _macros;
        std::deque<picture> _pictures;

      public:
        document( );

        void add_picture( const picture& p_picture );

        render_t render( const std::string&   p_fontPath  = EMPTY_STR,
                         const std::string&   p_colorPath = EMPTY_STR,
                         const std::string&   p_macroPath = EMPTY_STR,
                         const std::set<u32>& p_times     = { 0 } ) const;

        static FILE* open_or_abort( const std::string& p_path );
        static void  indent( FILE* p_out, u32 p_indentLevel = 1, u32 p_indent = 4 );
        static bool output( const std::string& p_path, const std::string& p_name, render_t p_render,
                            u32 p_indent = 4 );
    };
} // namespace TIKZ
