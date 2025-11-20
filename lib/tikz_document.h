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

        std::string to_string( const std::string&   p_fontPath  = EMPTY_STR,
                               const std::string&   p_colorPath = EMPTY_STR,
                               const std::string&   p_macroPath = EMPTY_STR,
                               const std::set<u32>& p_times     = { 0 } ) const;
    };
} // namespace TIKZ
