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
circuit<R, T>
dedekind( const std::string& p_outputName, std::function<std::string( u64 )> p_inputName,
          std::function<std::string( const std::string&, const std::string& )> p_nameCombination,
          const std::deque<symbolic_simple_function<R, T>>&                    p_phi ) {
    if( p_phi.size( ) < 1 ) { return circuit<R, T>{ {} }; }

    auto ig = std::make_shared<input_gate<R, T>>( p_inputName( 1 ) );

    auto f = std::make_shared<function_gate<R, T>>(
        std::format( "1 / {}({})", p_phi[ 0 ].m_name, p_inputName( 1 ) ),
        inverse<R, T>( [ p_inputName, p_phi ]( function_input<T> p_l_input ) {
            return p_phi[ 0 ].m_f( p_l_input( p_inputName( 1 ) ).m_data );
        } ) );

    std::shared_ptr<gate<R, T>> mg = std::make_shared<multiplication_gate<R, T>>(
        std::deque<std::shared_ptr<gate<R, T>>>{ f, ig } );

    for( u64 k = 2; k <= p_phi.size( ); ++k ) {
        // replace each input with small circuit
        for( const auto& input : mg->inputs( ) ) {
            auto igk = std::make_shared<input_gate<R, T>>( input );
            auto igi = std::make_shared<input_gate<R, T>>(
                p_nameCombination( p_inputName( k ), input ) );

            auto fi = std::make_shared<function_gate<R, T>>(
                std::format( "-{}({})", p_phi[ k - 1 ].m_name, p_inputName( k ) ),
                negate<R, T>( [ p_inputName, p_phi, k ]( function_input<T> p_l_input ) {
                    return p_phi[ k - 1 ].m_f( p_l_input( p_inputName( k ) ).m_data );
                } ) );

            auto mgk = std::make_shared<multiplication_gate<R, T>>(
                std::deque<std::shared_ptr<gate<R, T>>>{ fi, igk } );
            auto mp = std::make_shared<addition_gate<R, T>>(
                std::deque<std::shared_ptr<gate<R, T>>>{ mgk, igi } );

            mg = mg->replace_input( input, mp );
        }
        // add extra multiplication

        auto phi1 = [ p_inputName, p_phi, k ]( function_input<T> p_l_input ) {
            return p_phi[ 0 ].m_f( p_l_input( p_inputName( k ) ).m_data );
        };
        auto phi2 = [ p_inputName, p_phi, k ]( function_input<T> p_l_input ) {
            return p_phi[ k - 1 ].m_f( p_l_input( p_inputName( k ) ).m_data );
        };

        auto fo = std::make_shared<function_gate<R, T>>(
            std::format( "1/({}({}) - {}({}))", p_phi[ 0 ].m_name, p_inputName( k ),
                         p_phi[ k - 1 ].m_name, p_inputName( k ) ),
            inverse<R, T>( add<R, T>( phi1, negate<R, T>( phi2 ) ) ) );

        mg = std::make_shared<multiplication_gate<R, T>>(
            std::deque<std::shared_ptr<gate<R, T>>>{ fo, mg } );
    }

    return circuit<R, T>{ { std::make_shared<output_gate<R, T>>( p_outputName, mg ) } };
}

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

template <typename R, typename T = R>
std::map<std::string, tikz_point>
place_gate( picture& p_pic, std::shared_ptr<gate<R, T>> p_gate, tikz_point p_topLeft, u64 p_fanOut,
            const std::string& p_name, color p_color, color p_bgColor,
            const std::set<std::string> p_invertOps, const kv_store& p_options ) {
    if( !p_gate ) { return { }; }

    if( p_gate->is_input( ) ) {
        return place_input_gate( p_pic, "in[" + math_mode( p_gate->m_name ) + "]", p_topLeft,
                                 p_fanOut, p_name, p_color, p_bgColor, p_options );
    }
    if( p_gate->is_output( ) ) {
        return place_output_gate( p_pic, math_mode( "\\mathrm{out}_{" + p_gate->m_name + "}" ),
                                  p_topLeft, p_name, p_color, p_bgColor, p_options );
    }
    if( p_gate->is_constant( ) ) {
        return place_const_gate( p_pic, math_mode( p_gate->m_name ), p_topLeft, p_fanOut, p_name,
                                 p_color, p_bgColor, p_options );
    }
    if( p_gate->is_operator( ) ) {
        return place_op_gate( p_pic, p_gate->m_name, p_topLeft, p_gate->children( ).size( ),
                              p_fanOut, p_name, p_color, p_bgColor,
                              p_invertOps.count( p_gate->m_name ), p_options );
    }
    return { };
}

constexpr double DEFAULT_GATE_SEP_X = 2 * GATE_WIDTH;
constexpr double DEFAULT_GATE_SEP_Y = 1.75 * GATE_HEIGHT;
template <typename R, typename T = R>
std::pair<double, std::map<std::string, tikz_point>>
place_circuit( picture& p_pic, std::shared_ptr<gate<R, T>> p_gate, tikz_point p_topLeft,
               u64 p_totalDepth, u64 p_currentDepth, const std::string& p_name, color p_color,
               color p_bgColor, const std::set<std::string> p_invertOps,
               const kv_store& p_options ) {
    std::map<std::string, tikz_point> conns{ };
    auto px = p_topLeft.m_x + ( p_totalDepth - p_currentDepth ) * DEFAULT_GATE_SEP_X;
    // place all inputs on left boundary
    if( p_gate->is_input( ) ) { px = p_topLeft.m_x; }

    double height = 0, lh = p_topLeft.m_y - 1.5 * GATE_HEIGHT;
    // place children
    u64 i = 0;
    for( auto& c : p_gate->children( ) ) {
        ++i;
        if( c->is_constant( ) ) {
            auto [ p, cnns ] = place_circuit<R, T>(
                p_pic, c,
                tikz_point{ p_topLeft.m_x + DEFAULT_GATE_SEP_X - 1.25 * GATE_WIDTH,
                            p_topLeft.m_y - height * DEFAULT_GATE_SEP_Y + .425 * GATE_HEIGHT },
                p_totalDepth, p_currentDepth + 1, p_name + "_" + std::to_string( i ), p_color,
                p_bgColor, p_invertOps, p_options );
            // height += 1 / 2.0;
        } else {
            auto [ h, cnns ] = place_circuit<R, T>(
                p_pic, c, tikz_point{ p_topLeft.m_x, p_topLeft.m_y - height * DEFAULT_GATE_SEP_Y },
                p_totalDepth, p_currentDepth + 1, p_name + "_" + std::to_string( i ), p_color,
                p_bgColor, p_invertOps, p_options );
            conns.merge( cnns );
            height += h;
            lh = conns[ p_name + "_" + std::to_string( i ) + "_out_1" ].m_y;
        }
    }
    // place current gate
    auto cnns = place_gate<R, T>(
        p_pic, p_gate,
        tikz_point{ px,
                    lh + .5 * GATE_HEIGHT + ( !p_gate->is_output( ) ? .25 * GATE_HEIGHT : 0.0 ) },
        1, p_name, p_color, p_bgColor, p_invertOps, p_options );
    if( !height ) { ++height; }
    conns.merge( cnns );
    i = 0;
    for( auto& c : p_gate->children( ) ) {
        ++i;
        if( c->is_constant( ) ) {
            place_const_connection( p_pic, p_name + "_" + std::to_string( i ) + "_out_1",
                                    p_name + "_in_" + std::to_string( i ) );
        } else {
            place_gate_connection( p_pic, p_name + "_" + std::to_string( i ) + "_out_1",
                                   conns[ p_name + "_" + std::to_string( i ) + "_out_1" ],
                                   p_name + "_in_" + std::to_string( i ),
                                   conns[ p_name + "_in_" + std::to_string( i ) ] );
        }
    }

    return { height, conns };
}

template <typename R, typename T = R>
double place_circuit( picture& p_pic, circuit<R, T> p_circuit,
                      tikz_point         p_topLeft = tikz_point{ 0, 0 },
                      const std::string& p_name = EMPTY_STR, color p_color = COLOR_TEXT,
                      color                       p_bgColor   = COLOR_TEXT.to_bg( ),
                      const std::set<std::string> p_invertOps = { "\\textbullet" },
                      const kv_store&             p_options   = { } ) {
    double height = 0;
    u64    i      = 0;
    for( std::shared_ptr<gate<R, T>> out : p_circuit.m_outputs ) {
        auto res = place_circuit<R, T>( p_pic, out, p_topLeft, p_circuit.depth( ), 0,
                                        p_name + "_" + std::to_string( i++ ), p_color, p_bgColor,
                                        p_invertOps, p_options );
        p_topLeft.m_y -= height * DEFAULT_GATE_SEP_Y;
    }
    return height;
}

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures
//
// ---------------------------------------------------------------------------------------------

void add_dede_circ_picture( document& p_out, u64 p_k ) {
    WITH_PICTURE( pic, { }, p_out ) {
        std::deque<symbolic_simple_function<std::string>> phi{ };
        for( u64 i = 1; i <= p_k; ++i ) {
            phi.push_back( symbolic_phi( std::format( "\\phi_{}", i ) ) );
        }
        auto d1 = dedekind<std::string>(
            // std::format( "\\circout{{D_{{{}}}}}", p_k  ),
            "", []( u64 p_l_i ) { return std::format( "g_{}", p_l_i ); },
            []( const std::string& p_l_a, const std::string& p_l_b ) {
                return std::format( "{} \\ast {}", p_l_a, p_l_b );
            },
            phi );
        place_circuit<std::string, std::string>( pic, d1 );
    }
}

FILE_SIMPLE( g01, {
    for( u64 i = 1; i <= 4; ++i ) { add_dede_circ_picture( doc, i ); }
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
