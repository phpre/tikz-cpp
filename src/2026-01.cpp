#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font";
std::string COLOR_FILENAME  = "color";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      globals
//
// ---------------------------------------------------------------------------------------------

std::string variable( std::string p_name ) {
    return "g_" + p_name;
}

std::string output( std::string p_name ) {
    return "\\text{out}_" + p_name;
}

template <typename T, typename R>
meta_circuit<T, R>
dedekind( std::deque<std::pair<std::string, symbolic_simple_function<T, R>>> p_phi ) {
    if( p_phi.size( ) == 1 ) {
        const auto& [ i, phi ] = p_phi[ 0 ];

        auto ig = std::make_shared<meta_input_gate<T, R>>( variable( i ) );
        auto f  = std::make_shared<meta_function_gate<T, R>>(
            inverse<T, R>( [ & ]( function_input<T> p_input ) {
                return phi( p_input.at( variable( i ) ).m_data );
            } ) );

        auto mg  = std::make_shared<meta_multiplication_gate<T, R>>( { ig, f } );
        auto out = std::make_shared<meta_output_gate<T, R>>( output( i ), mg );

        return meta_circuit<T, R>{ { out } };
    }

    return circuit<T>{ {} };
}

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( g01, {
    WITH_PICTURE( pic, {}, doc ) {
        pic.place_text( "Hello world!", tikz_point{0.0,0.0 });
}
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
