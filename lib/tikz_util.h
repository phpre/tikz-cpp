#pragma once
#include <string>

namespace TIKZ {
    inline std::string math_mode( const std::string& p_string ) {
        return "\\(" + p_string + "\\)";
    }

    inline std::string text_typewriter( const std::string& p_string ) {
        return "\\texttt{" + p_string + "}";
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
} // namespace TIKZ
