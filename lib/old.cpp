#include "old.h"
#include "tikz.h"
#include "tikz_document.h"
#include "tikz_string.h"

namespace TIKZ {
    const std::string CROSS_STYLE
        = "\\tikzset{cross/.style={cross out, draw, minimum size=2*(#1-\\pgflinewidth), inner sep=0pt, outer sep=0pt}}\n";

    void print_width_macro( FILE* p_out, double p_width, const std::string& p_text,
                            u64 p_startIndent, u64 p_indent ) {
        if( p_text == EMPTY_STR ) {
            INDENT_PRINT( p_startIndent )(
                p_out, "\\pgfmathsetmacro\\twd{ %5.3lf * 1cm / width(\" \") }\n", p_width );
        } else {
            INDENT_PRINT( p_startIndent )(
                p_out, "\\pgfmathsetmacro\\twd{ %5.3lf * 1cm / width(\"%s\") }\n", p_width,
                p_text.c_str( ) );
        }
    }

    void print_text( FILE* p_out, const std::string& p_text, tikz_point p_position,
                     const std::string& p_options, color p_textColor, u64 p_startIndent,
                     u64 p_indent ) {
        INDENT_PRINT( p_startIndent )(
            p_out, "\\node[inner sep = 0pt, text = %s, %s ] at (%lf, %lf) { %s };\n",
            p_textColor.c_str( ), p_options.c_str( ), p_position.m_x, p_position.m_y,
            p_text.c_str( ) );
    }

    FILE* open_or_abort( const std::string& p_path ) {
        return document::open_or_abort( p_path );
    }

    void indent( FILE* p_out, u64 p_indentLevel, u64 p_indent ) {
        document::indent( p_out, p_indentLevel, p_indent );
    }

    void initialize_tikzpicture( FILE* p_out, const std::string& p_options ) {
        fprintf( p_out,
                 "\\begin{page}\n"
                 "\\begin{tikzpicture}[%s]\n",
                 p_options.c_str( ) );
    }
    void finish_tikzpicture( FILE* p_out ) {
        fprintf( p_out, "\\end{tikzpicture}\n"
                        "\\end{page}\n" );
    }

    void print_cross( FILE* p_out, tikz_point p_position, color p_color, u64 p_startIndent,
                      u64 p_indent ) {
        INDENT_PRINT( p_startIndent )( p_out,
                                       "\\node[cross=5, line width=2.5pt, %s] at (%lf, %lf) {};\n",
                                       p_color.c_str( ), p_position.m_x, p_position.m_y );
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
