#pragma once
#include <string>
#include "tikz_command.h"
#include "tikz_option.h"

namespace TIKZ {
    inline std::string math_mode( const std::string& p_string ) {
        return "\\(" + p_string + "\\)";
    }

    inline std::string text_typewriter( const std::string& p_string ) {
        return "\\texttt{" + p_string + "}";
    }

    inline std::string text_boldface( const std::string& p_string ) {
        return "\\textbf{" + p_string + "}";
    }

    constexpr std::string VSIZE_CORRECTION       = "\\vphantom{Ag|}";
    constexpr std::string WILDCARD_SYMBOL_FILLED = "\\wcsymbolf";
    constexpr std::string WILDCARD_SYMBOL        = "\\wcsymbol";

    inline std::string textsize_small( const std::string& p_string ) {
        return "{\\small " + p_string + "}";
    }
    inline std::string textsize_footnotesize( const std::string& p_string ) {
        return "{\\footnotesize " + p_string + "}";
    }
    inline std::string textsize_tiny( const std::string& p_string ) {
        return "{\\tiny " + p_string + "}";
    }
    inline std::string textsize_large( const std::string& p_string ) {
        return "{\\large " + p_string + "}";
    }
    inline std::string textsize_Large( const std::string& p_string ) {
        return "{\\Large " + p_string + "}";
    }
    inline std::string textsize_LARGE( const std::string& p_string ) {
        return "{\\LARGE " + p_string + "}";
    }

    const kv_store      XSCALE_TO_WIDTH  = OPT::XSCALE( "{min(1, \\twd)}" );
    const kv_store      YSCALE_TO_WIDTH  = OPT::YSCALE( "{min(1, \\twd)}" );
    const kv_store      XSCALE_TO_HEIGHT = OPT::XSCALE( "{min(1, \\thg)}" );
    const kv_store      YSCALE_TO_HEIGHT = OPT::YSCALE( "{min(1, \\thg)}" );
    inline math_command width_macro( double p_width, const std::string& p_text,
                                     const std::string& p_unit      = "1cm",
                                     const std::string& p_macroName = "twd" ) {
        std::string buf;
        if( p_text == EMPTY_STR ) {
            buf = std::format( "{:5.3f} * {} / width(\" \")", p_width, p_unit );
        } else {
            buf = std::format( "{:5.3f} * {} / width(\"{}\")", p_width, p_unit, p_text );
        }
        return math_command{ p_macroName, buf };
    }

    inline math_command height_macro( double p_height, const std::string& p_text,
                                      const std::string& p_unit      = "1cm",
                                      const std::string& p_macroName = "thg" ) {
        std::string buf;
        if( p_text == EMPTY_STR ) {
            buf = std::format( "{:5.3f} * {} / height(\" \")", p_height, p_unit );
        } else {
            buf = std::format( "{:5.3f} * {} / height(\"{}\")", p_height, p_unit, p_text );
        }
        return math_command{ p_macroName, buf };
    }

} // namespace TIKZ
