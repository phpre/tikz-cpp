#include "tikz.h"
#include "tikz_document.h"

namespace TIKZ {
    document::document( )
        : _packages{ "mathtools", "tikz" }, _libraries{ "calc",
                                                        "fit",
                                                        "shapes",
                                                        "shapes.geometric",
                                                        "decorations",
                                                        "decorations.pathmorphing",
                                                        "decorations.markings",
                                                        "decorations.pathreplacing",
                                                        "decorations.shapes" },
          _globalStyles{ { "cross", CROSS_STYLE } }, _macros{ }, _pictures{ } {
    }

    void document::add_picture( const picture& p_picture ) {
    }

    FILE* open_or_abort( const std::string& p_path ) {
        FILE* out = fopen( p_path.c_str( ), "w" );
        if( !out ) {
            fprintf( stderr, "IO error" );
            return nullptr;
        }
        return out;
    }

    std::string document::to_string( const std::string& p_fontPath, const std::string& p_colorPath,
                                     const std::string&   p_macroPath,
                                     const std::set<u32>& p_times ) const {
        std::string result = "";

        result += "\\documentclass[multi=page]{standalone}\n";

        for( const auto& str : _packages ) { result += "\\usepackage{" + str + "}\n"; }

        if( p_macroPath != EMPTY_STR ) { result += "\\input{" + p_macroPath + "}\n"; }
        if( p_fontPath != EMPTY_STR ) { result += "\\input{" + p_fontPath + "}\n"; }
        if( p_colorPath != EMPTY_STR ) { result += "\\input{" + p_colorPath + "}\n"; }

        for( const auto& str : _libraries ) { result += "\\usetikzlibrary{" + str + "}\n"; }

        // some default extra styles
        for( const auto& [ name, str ] : _globalStyles ) { result += str + "\n"; }

        // make document compilations reproducible
        result += "\\begin{document}\n";
        result += "\\pdfvariable suppressoptionalinfo \\numexpr\n"
                  "0\n"
                  "+   1   %% PTEX.FullBanner\n"
                  "+   2   %% PTEX.FileName\n"
                  "+   4   %% PTEX.PageNumber\n"
                  "+   8   %% PTEX.InfoDict\n"
                  "+  16   %% Creator\n"
                  "+  32   %% CreationDate\n"
                  "+  64   %% ModDate\n"
                  "+ 128   %% Producer\n"
                  "+ 256   %% Trapped\n"
                  "+ 512   %% ID\n"
                  "\\relax\n";

        // print pictures
        for( const auto& pic : _pictures ) {
            for( auto time : p_times ) {
                auto pres = pic.to_string( time );
                if( pres == EMPTY_STR ) { continue; }
                result += "\\begin{page}\n" + pres + "\\end{page}\n";
            }
        }

        result += "\\end{document}\n";
        return result;
    }

    FILE* new_document( const std::string& p_path, const std::string& p_fontPath,
                        const std::string& p_colorPath, const std::string& p_macroPath,
                        const std::string& p_packages, const std::string& p_tikzLibraries,
                        const std::string& p_extra ) {
        FILE* doc = open_or_abort( p_path );
        fprintf( doc,
                 "\\documentclass[multi=page]{standalone}\n"
                 "\\usepackage{algorithm2e,mathtools,tikz}\n"
                 "\\usetikzlibrary{calc,fit}\n"
                 "\\usetikzlibrary{shapes,shapes.geometric}\n"
                 "\\usetikzlibrary{decorations,decorations.pathmorphing,decorations.markings}\n"
                 "\\usetikzlibrary{decorations.pathreplacing,decorations.shapes}\n" );

        if( p_packages != EMPTY_STR ) { fprintf( doc, "\\usepackage{%s}\n", p_packages.c_str( ) ); }

        if( p_macroPath != EMPTY_STR ) { fprintf( doc, "\\input{%s}\n", p_macroPath.c_str( ) ); }
        if( p_fontPath != EMPTY_STR ) { fprintf( doc, "\\input{%s}\n", p_fontPath.c_str( ) ); }
        if( p_colorPath != EMPTY_STR ) { fprintf( doc, "\\input{%s}\n", p_colorPath.c_str( ) ); }

        if( p_tikzLibraries != EMPTY_STR ) {
            fprintf( doc, "\\usetikzlibrary{%s}\n", p_tikzLibraries.c_str( ) );
        }

        if( p_extra != EMPTY_STR ) { fprintf( doc, "%s\n", p_extra.c_str( ) ); }

        // some default extra styles
        fprintf( doc, "%s\n", CROSS_STYLE.c_str( ) );

        fprintf( doc, "\\begin{document}\n" );
        fprintf( doc, "\\pdfvariable suppressoptionalinfo \\numexpr\n"
                      "0\n"
                      "+   1   %% PTEX.FullBanner\n"
                      "+   2   %% PTEX.FileName\n"
                      "+   4   %% PTEX.PageNumber\n"
                      "+   8   %% PTEX.InfoDict\n"
                      "+  16   %% Creator\n"
                      "+  32   %% CreationDate\n"
                      "+  64   %% ModDate\n"
                      "+ 128   %% Producer\n"
                      "+ 256   %% Trapped\n"
                      "+ 512   %% ID\n"
                      "\\relax\n" );

        return doc;
    }

    void finish_document( FILE* p_out ) {
        fprintf( p_out, "\\end{document}\n" );
        fclose( p_out );
    }

} // namespace TIKZ
