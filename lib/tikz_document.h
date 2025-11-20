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

        kv_store            _macros;
        std::deque<picture> _pictures;

      public:
        document( );

        void add_picture( const picture& p_picture );

        bool output( const std::string& p_path, const std::string& p_fontPath = EMPTY_STR,
                     const std::string& p_colorPath = EMPTY_STR,
                     const std::string& p_macroPath = EMPTY_STR );
    };
} // namespace TIKZ
