#pragma once

#include <algorithm>
#include <cstdio>
#include <string>

#define PRINT_MACRO \
    if( p_print ) printf

typedef unsigned int        u32;
typedef signed int          s32;
typedef std::pair<u32, u32> point;

constexpr std::string EMPTY_STR = "";

struct color {
  private:
    std::string m_name;

    static constexpr std::string MIX_SYMBOL = "!";

  public:
    constexpr color( const std::string& p_name ) : m_name{ p_name } {
    }

    color mix( color p_other, u32 p_amount ) const;
    color lighten( u32 p_amount ) const;
    color darken( u32 p_amount ) const;

    inline void replace_if_non_empty( const color& p_other ) {
        if( p_other.is_non_empty( ) ) { m_name = p_other.m_name; }
    }

    inline bool is_non_empty( ) const {
        return m_name != EMPTY_STR;
    }

    inline color to_flavor_bg( ) const {
        return lighten( 4 );
    }
    inline color to_bg( ) const {
        return lighten( 10 );
    }

    inline color deemphasize_strong( ) const {
        return lighten( 25 );
    }
    inline color deemphasize( ) const {
        return lighten( 50 );
    }
    inline color deemphasize_weak( ) const {
        return lighten( 75 );
    }

    const char* c_str( ) const {
        return m_name.c_str( );
    }

    std::string to_string( ) const {
        return m_name;
    }
};

struct tikz_point {
    double m_x;
    double m_y;

    tikz_point( double p_x, double p_y ) : m_x{ p_x }, m_y{ p_y } {};

    tikz_point( const std::pair<double, double>& p_point )
        : m_x{ p_point.first }, m_y{ p_point.second } {};

    inline operator std::pair<double, double>( ) const {
        return std::pair<double, double>{ m_x, m_y };
    }

    inline tikz_point operator*( double p_scale ) const {
        return { m_x * p_scale, m_y * p_scale };
    }

    inline tikz_point operator+( tikz_point p_other ) const {
        return { m_x + p_other.m_x, m_y + p_other.m_y };
    }
};

constexpr color COLOR_NONE{ "" };
constexpr color COLOR_BLACK{ "black" };
constexpr color COLOR_TEXT{ "black!80!white" };
constexpr color COLOR_WHITE{ "white" };
constexpr color COLOR_FILL_WHITE{ "black!5!white" };

constexpr color COLOR_C1{ "c1" };
constexpr color COLOR_C2{ "c2" };
constexpr color COLOR_C3{ "c3" };
constexpr color COLOR_C4{ "c4" };
constexpr color COLOR_C5{ "c5" };

constexpr std::string LINE_CONN     = " -- ";
constexpr std::string RECT_CONN     = " rectangle ";
constexpr std::string CIRC_CONN     = " circle ";
constexpr std::string ARR_TIP_LATEX = "-latex";

constexpr std::string LW_THIN_OUTLINE       = "line width = .75pt";
constexpr std::string LW_OUTLINE            = "line width = 1pt";
constexpr std::string LW_SUPPORT_LINE       = "line width = .5pt";
constexpr std::string LW_THICK_SUPPORT_LINE = "line width = .75pt";
constexpr std::string LW_LINE               = "line width = 1.25pt";

inline std::string math_mode( const std::string& p_string ) {
    return "\\(" + p_string + "\\)";
}

inline std::string text_typewriter( const std::string& p_string ) {
    return "\\texttt{" + p_string + "}";
}

inline std::string vsize_correction( const std::string& p_string ) {
    return "\\vphantom{Ag|}" + p_string;
}
inline std::string textsize_small( const std::string& p_string ) {
    return "{\\small " + p_string + "}";
}
inline std::string textsize_footnotesize( const std::string& p_string ) {
    return "{\\footnotesize " + p_string + "}";
}

FILE* new_document( const std::string& p_path, const std::string& p_fontPath = EMPTY_STR,
                    const std::string& p_colorPath     = EMPTY_STR,
                    const std::string& p_macroPath     = EMPTY_STR,
                    const std::string& p_packages      = EMPTY_STR,
                    const std::string& p_tikzLibraries = EMPTY_STR,
                    const std::string& p_extra         = EMPTY_STR );

void finish_document( FILE* p_out );

void initialize_tikzpicture( FILE* p_out, const std::string& p_options = EMPTY_STR );
void finish_tikzpicture( FILE* p_out );
void indent( FILE* p_out, u32 p_indentLevel = 1, u32 p_indent = 4 );

#define INDENT_PRINT( p_level )         \
    indent( p_out, p_level, p_indent ); \
    fprintf

void print_coordinate( FILE* p_out, const std::string& p_position, const std::string& p_name,
                       const std::string& p_extraOptions = EMPTY_STR, u32 p_startIndent = 1,
                       u32 p_indent = 4 );

void print_coordinate( FILE* p_out, tikz_point p_position, const std::string& p_name,
                       const std::string& p_extraOptions = EMPTY_STR, u32 p_startIndent = 1,
                       u32 p_indent = 4 );

void print_node( FILE* p_out, const std::string& p_position,
                 const std::string& p_content = EMPTY_STR, const std::string& p_options = EMPTY_STR,
                 const std::string& p_name = EMPTY_STR, u32 p_startIndent = 1, u32 p_indent = 4 );

void print_node( FILE* p_out, tikz_point p_position, const std::string& p_content = EMPTY_STR,
                 const std::string& p_options = EMPTY_STR, const std::string& p_name = EMPTY_STR,
                 u32 p_startIndent = 1, u32 p_indent = 4 );

void print_connection( FILE* p_out, const std::string& p_topLeft, const std::string& p_bottomRight,
                       const std::string& p_connection = LINE_CONN,
                       const std::string& p_options = EMPTY_STR, u32 p_startIndent = 1,
                       u32 p_indent = 4 );
void print_connection( FILE* p_out, tikz_point p_topLeft, tikz_point p_bottomRight,
                       const std::string& p_connection = LINE_CONN,
                       const std::string& p_options = EMPTY_STR, u32 p_startIndent = 1,
                       u32 p_indent = 4 );

void print_text( FILE* p_out, const std::string& p_text, tikz_point p_position,
                 const std::string& p_options = EMPTY_STR, color p_textColor = COLOR_TEXT,
                 u32 p_startIndent = 1, u32 p_indent = 4 );

void print_cross( FILE* p_out, tikz_point p_position, color p_color = COLOR_TEXT,
                  u32 p_startIndent = 1, u32 p_indent = 4 );

void print_debug_point( FILE* p_out, tikz_point p_position, u32 p_startIndent = 1,
                        u32 p_indent = 4 );

std::pair<tikz_point, tikz_point> translate_right( std::pair<tikz_point, tikz_point> p_pos,
                                                   double                            p_amount );

std::pair<tikz_point, tikz_point> translate_down( std::pair<tikz_point, tikz_point> p_pos,
                                                  double                            p_amount );
