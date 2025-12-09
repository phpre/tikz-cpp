#pragma once
#include <deque>
#include <fstream>
#include <set>
#include <string>

#include "defines.h"
#include "tikz_picture.h"
#include "tikz_point.h"

namespace TIKZ {
    class document {
        std::set<std::string> _packages;
        std::set<std::string> _libraries;
        std::deque<picture>   _pictures;

        bool _final = false;

      public:
        document( );

        inline void finalize( ) {
            _final = true;
        }
        inline bool is_final( ) const {
            return _final;
        }

        void add_picture( const picture& p_picture );

        render_t render( const std::string& p_fontPath  = EMPTY_STR,
                         const std::string& p_colorPath = EMPTY_STR,
                         const std::string& p_macroPath = EMPTY_STR ) const;

        static std::ofstream open_or_abort( const std::string& p_path );
        static void indent( std::ofstream& p_out, u64 p_indentLevel = 1, u64 p_indent = 4 );
        static bool output( const std::string& p_path, const std::string& p_name, render_t p_render,
                            u64 p_indent = 4 );
    };

#define WITH_DOCUMENT( p_varName, p_fileName, p_outDir, p_fontPath, p_colorPath, p_macroPath ) \
    for( document p_varName{ }; !p_varName.is_final( );                                        \
         p_varName.finalize( ),                                                                \
         document::output( p_outDir, p_fileName,                                               \
                           p_varName.render( p_fontPath, p_colorPath, p_macroPath ) ) )

#define WITH_PICTURE( p_varName, p_picArgs, p_docName )       \
    for( picture p_varName p_picArgs; !p_varName.is_final( ); \
         p_varName.finalize( ), p_docName.add_picture( p_varName ) )

} // namespace TIKZ
