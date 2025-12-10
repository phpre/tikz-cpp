#pragma once
#include <deque>
#include <functional>
#include <string>
#include "defines.h"

extern std::string OUTDIR_DEFAULT;
extern std::string TEXDIR_DEFAULT;
extern std::string FONT_FILENAME;
extern std::string COLOR_FILENAME;
extern std::string MACROS_FILENAME;

namespace TIKZ {
    extern std::string OUT_DIR;
    extern std::string TEX_DIR;
    extern std::string FONT_PATH;
    extern std::string COLOR_PATH;
    extern std::string MACRO_PATH;
    extern std::string PROGRAM_NAME;

    extern std::deque<std::function<void( )>> SIMPLE_PICS;

    u64 add_simple_picture( std::function<void( )> p_f );

#define WITH_DOCUMENT( p_varName, p_fileName, p_outDir, p_fontPath, p_colorPath, p_macroPath ) \
    for( TIKZ::document p_varName{ }; !p_varName.is_final( );                                  \
         p_varName.finalize( ),                                                                \
         TIKZ::document::output( p_outDir, p_fileName,                                         \
                                 p_varName.render( p_fontPath, p_colorPath, p_macroPath ) ) )

#define WITH_PICTURE( p_varName, p_picArgs, p_docName )             \
    for( TIKZ::picture p_varName p_picArgs; !p_varName.is_final( ); \
         p_varName.finalize( ), p_docName.add_picture( p_varName ) )

#define MAIN( ... )                                         \
    int main( int p_argc, char* p_argv[] ) {                \
        TIKZ::PROGRAM_NAME = p_argv[ 0 ];                   \
        if( p_argc > 1 ) {                                  \
            TIKZ::OUT_DIR = std::string{ p_argv[ 1 ] };     \
        } else {                                            \
            TIKZ::OUT_DIR = OUTDIR_DEFAULT;                 \
        }                                                   \
        if( p_argc > 2 ) {                                  \
            TIKZ::TEX_DIR = std::string{ p_argv[ 2 ] };     \
        } else {                                            \
            TIKZ::TEX_DIR = TEXDIR_DEFAULT;                 \
        }                                                   \
        TIKZ::FONT_PATH  = TIKZ::TEX_DIR + FONT_FILENAME;   \
        TIKZ::COLOR_PATH = TIKZ::TEX_DIR + COLOR_FILENAME;  \
        TIKZ::MACRO_PATH = TIKZ::TEX_DIR + MACROS_FILENAME; \
        for( const auto& s : TIKZ::SIMPLE_PICS ) { s( ); }  \
        __VA_ARGS__                                         \
    }

#define FILE_SIMPLE( p_name, ... )                                                                 \
    void pic_##p_name( ) {                                                                         \
        WITH_DOCUMENT( doc, #p_name ".tex", TIKZ::OUT_DIR, TIKZ::FONT_PATH, TIKZ::COLOR_PATH, \
                       TIKZ::MACRO_PATH ) {                                                        \
            __VA_ARGS__                                                                            \
        }                                                                                          \
    }                                                                                              \
    auto __##p_name = TIKZ::add_simple_picture( pic_##p_name );

} // namespace TIKZ
