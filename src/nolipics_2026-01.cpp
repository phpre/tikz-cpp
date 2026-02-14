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

std::string variable( u64 p_name ) {
    return std::format( "g_{}", p_name );
}

symbolic_simple_function<std::string, std::string> symbolic_phi( const std::string& p_name ) {
    return { p_name,
             [ p_name ]( std::string p_l_input ) { return p_name + "(" + p_l_input + ")"; } };
}

template <typename R, typename T = R>
std::shared_ptr<output_gate<R, T>> dedekind_initial( const std::string&                p_outputName,
                                                     std::function<std::string( u64 )> p_inputName,
                                                     const symbolic_simple_function<R, T>& p_phi ) {

    auto ig = std::make_shared<input_gate<R, T>>( p_inputName( 1 ) );

    auto f = std::make_shared<function_gate<R, T>>(
        std::format( "1 / {}({})", p_phi.m_name, p_inputName( 1 ) ),
        inverse<R, T>( [ p_inputName, p_phi ]( function_input<T> p_l_input ) {
            return p_phi.m_f( p_l_input( p_inputName( 1 ) ).m_data );
        } ) );

    std::shared_ptr<gate<R, T>> mg = std::make_shared<multiplication_gate<R, T>>(
        std::deque<std::shared_ptr<gate<R, T>>>{ f, ig } );

    return std::make_shared<output_gate<R, T>>( p_outputName, mg );
}

template <typename R, typename T = R>
std::shared_ptr<output_gate<R, T>> dedekind_step(
    std::shared_ptr<output_gate<R, T>> p_inner, u64 p_k, const std::string& p_outputName,
    std::function<std::string( u64 )>                                    p_inputName,
    std::function<std::string( const std::string&, const std::string& )> p_nameCombination,
    const symbolic_simple_function<R, T>& p_phi1, const symbolic_simple_function<R, T>& p_phik ) {

    std::shared_ptr<gate<R, T>> inner = p_inner->m_child;
    // replace each input with small circuit
    for( const auto& input : inner->inputs( ) ) {
        auto igk = std::make_shared<input_gate<R, T>>( input );
        auto igi
            = std::make_shared<input_gate<R, T>>( p_nameCombination( p_inputName( p_k ), input ) );

        auto fi = std::make_shared<function_gate<R, T>>(
            std::format( "-{}({})", p_phik.m_name, p_inputName( p_k ) ),
            negate<R, T>( [ p_inputName, p_phik, p_k ]( function_input<T> p_l_input ) {
                return p_phik.m_f( p_l_input( p_inputName( p_k ) ).m_data );
            } ) );

        auto innerk = std::make_shared<multiplication_gate<R, T>>(
            std::deque<std::shared_ptr<gate<R, T>>>{ fi, igk } );
        auto mp = std::make_shared<addition_gate<R, T>>(
            std::deque<std::shared_ptr<gate<R, T>>>{ innerk, igi } );

        inner = inner->replace_input( input, mp );
    }
    // add extra multiplication

    auto phi1 = [ p_inputName, p_phi1, p_k ]( function_input<T> p_l_input ) {
        return p_phi1.m_f( p_l_input( p_inputName( p_k ) ).m_data );
    };
    auto phi2 = [ p_inputName, p_phik, p_k ]( function_input<T> p_l_input ) {
        return p_phik.m_f( p_l_input( p_inputName( p_k ) ).m_data );
    };

    auto fo = std::make_shared<function_gate<R, T>>(
        std::format( "1/({}({}) - {}({}))", p_phi1.m_name, p_inputName( p_k ), p_phik.m_name,
                     p_inputName( p_k ) ),
        inverse<R, T>( add<R, T>( phi1, negate<R, T>( phi2 ) ) ) );

    inner = std::make_shared<multiplication_gate<R, T>>(
        std::deque<std::shared_ptr<gate<R, T>>>{ fo, inner } );
    return std::make_shared<output_gate<R, T>>( p_outputName, inner );
}

template <typename R, typename T = R>
circuit<R, T>
dedekind( const std::string& p_outputName, std::function<std::string( u64 )> p_inputName,
          std::function<std::string( const std::string&, const std::string& )> p_nameCombination,
          const std::deque<symbolic_simple_function<R, T>>& p_phi, bool p_opaqueInner = false,
          bool p_opaque = false ) {
    if( p_phi.size( ) < 1 ) { return circuit<R, T>{ {} }; }
    if( p_opaque ) { p_opaqueInner = false; }

    auto circ = dedekind_initial( p_outputName, p_inputName, p_phi[ 0 ] );
    for( u64 k = 2; k <= p_phi.size( ) - p_opaqueInner; ++k ) {
        circ = dedekind_step( circ, k, p_outputName, p_inputName, p_nameCombination, p_phi[ 0 ],
                              p_phi[ k - 1 ] );
    }
    if( p_opaque or ( p_opaqueInner && p_phi.size( ) > 1 ) ) {
        auto circ_o = std::make_shared<output_gate<R, T>>(
            p_outputName,
            std::make_shared<subcircuit_gate<R, T>>(
                math_mode( std::format( "C_{{{}}}", p_phi.size( ) - ( !p_opaque ) ) ), circ ) );
        if( p_opaque ) {
            circ = circ_o;
        } else {
            circ = dedekind_step( circ_o, p_phi.size( ), p_outputName, p_inputName,
                                  p_nameCombination, p_phi[ 0 ], p_phi[ p_phi.size( ) - 1 ] );
        }
    }
    return circuit<R, T>{ { circ } };
}

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

void add_dede_circ_picture( document& p_out, u64 p_k, bool p_opaqueInner = false,
                            bool p_opaque = false ) {
    WITH_PICTURE( pic, { }, p_out ) {
        std::deque<symbolic_simple_function<std::string>> phi{ };
        for( u64 i = 1; i <= p_k; ++i ) {
            phi.push_back( symbolic_phi( std::format( "\\phi_{}", i ) ) );
        }
        auto d1 = dedekind<std::string>(
            "", []( u64 p_l_i ) { return std::format( "g_{}", p_l_i ); },
            []( const std::string& p_l_a, const std::string& p_l_b ) {
                return std::format( "{} \\ast {}", p_l_a, p_l_b );
            },
            phi, p_opaqueInner, p_opaque );
        place_circuit<std::string, std::string>( pic, d1 );
    }
}

void add_dede_circ_full_picture( document& p_out, u64 p_k, bool p_opaqueInner = false,
                                 bool p_opaque = false ) {
    WITH_PICTURE( pic, { }, p_out ) {
        circuit<std::string> c;
        for( u64 r = 0; r < p_k; ++r ) {
            std::deque<symbolic_simple_function<std::string>> phi{ };
            for( u64 i = 0; i < p_k; ++i ) {
                phi.push_back( symbolic_phi( std::format( "\\phi_{}", 1 + ( ( i + r ) % p_k ) ) ) );
            }
            auto d1 = dedekind<std::string>(
                std::to_string( r + 1 ),
                [ r, p_k ]( u64 p_l_i ) {
                    return std::format( "g_{}", 1 + ( ( p_l_i + r - 1 ) % p_k ) );
                },
                []( const std::string& p_l_a, const std::string& p_l_b ) {
                    return std::format( "{} \\ast {}", p_l_a, p_l_b );
                },
                phi, p_opaqueInner, p_opaque );
            c = c.merge( d1 );
        }
        place_circuit<std::string, std::string>( pic, c );
    }
}

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( g01, {
    for( u64 i = 1; i <= 4; ++i ) { add_dede_circ_picture( doc, i ); }
} )
FILE_SIMPLE( g02, {
    for( u64 i = 1; i <= 4; ++i ) {
        add_dede_circ_picture( doc, i, true );
        add_dede_circ_picture( doc, i, false, true );
    }
} )

FILE_SIMPLE( g03, {
    for( u64 i = 1; i <= 4; ++i ) { add_dede_circ_full_picture( doc, i ); }
} )
FILE_SIMPLE( g04, {
    for( u64 i = 1; i <= 4; ++i ) {
        add_dede_circ_full_picture( doc, i, true );
        add_dede_circ_full_picture( doc, i, false, true );
    }
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
