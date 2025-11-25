#include <cstdio>
#include "tikz_document.h"

namespace TIKZ {
    document::document( ) : _packages{ "mathtools", "tikz" }, _libraries{ "calc" }, _pictures{ } {
    }

    void document::add_picture( const picture& p_picture ) {
        _libraries.insert_range( p_picture.libraries( ) );
        _packages.insert_range( p_picture.packages( ) );
        _pictures.push_back( p_picture );
    }

    FILE* document::open_or_abort( const std::string& p_path ) {
        FILE* out = fopen( p_path.c_str( ), "w" );
        if( !out ) {
            fprintf( stderr, "IO error" );
            return nullptr;
        }
        return out;
    }

    void document::indent( FILE* p_out, u64 p_indentLevel, u64 p_indent ) {
        char buffer[ 10 ] = { 0 };
        snprintf( buffer, 9, "%%%llu s", p_indentLevel * p_indent );
        fprintf( p_out, buffer, "" );
    }

    bool document::output( const std::string& p_path, const std::string& p_name, render_t p_render,
                           u64 p_indent ) {
        FILE* doc = open_or_abort( p_path + p_name );
        if( !doc ) { return false; }

        for( const auto& [ i, s ] : p_render ) {
            indent( doc, i, p_indent );
            fprintf( doc, "%s\n", s.c_str( ) );
        }

        fclose( doc );
        return true;
    }

    render_t document::render( const std::string& p_fontPath, const std::string& p_colorPath,
                               const std::string&   p_macroPath,
                               const std::set<u64>& p_times ) const {
        render_t result{ };

        result.push_back( { 0, "\\documentclass[multi=page]{standalone}" } );

        for( const auto& str : _packages ) {
            result.push_back( { 0, "\\usepackage{" + str + "}" } );
        }

        if( p_macroPath != EMPTY_STR ) {
            result.push_back( { 0, "\\input{" + p_macroPath + "}" } );
        }
        if( p_fontPath != EMPTY_STR ) { result.push_back( { 0, "\\input{" + p_fontPath + "}" } ); }
        if( p_colorPath != EMPTY_STR ) {
            result.push_back( { 0, "\\input{" + p_colorPath + "}" } );
        }

        for( const auto& str : _libraries ) {
            result.push_back( { 0, "\\usetikzlibrary{" + str + "}" } );
        }

        // make document compilations reproducible
        result.push_back( { 0, "\\begin{document}" } );
        result.push_back( { 0, "\\pdfvariable suppressoptionalinfo \\numexpr" } );
        result.push_back( { 1, "0" } );
        result.push_back( { 1, "+   1   %% PTEX.FullBanner" } );
        result.push_back( { 1, "+   2   %% PTEX.FileName" } );
        result.push_back( { 1, "+   4   %% PTEX.PageNumber" } );
        result.push_back( { 1, "+   8   %% PTEX.InfoDict" } );
        result.push_back( { 1, "+  16   %% Creator" } );
        result.push_back( { 1, "+  32   %% CreationDate" } );
        result.push_back( { 1, "+  64   %% ModDate" } );
        result.push_back( { 1, "+ 128   %% Producer" } );
        result.push_back( { 1, "+ 256   %% Trapped" } );
        result.push_back( { 1, "+ 512   %% ID" } );
        result.push_back( { 0, "\\relax" } );

        // print pictures
        for( const auto& pic : _pictures ) {
            for( auto time : p_times ) {
                auto pres = pic.render( time, 1 );
                if( pres.empty( ) ) { continue; }
                result.push_back( { 0, "\\begin{page}" } );
                result.append_range( pres );
                result.push_back( { 0, "\\end{page}" } );
            }
        }

        result.push_back( { 0, "\\end{document}" } );
        return result;
    }
} // namespace TIKZ
