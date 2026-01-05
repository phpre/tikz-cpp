#pragma once
#include <deque>
#include <map>
#include <string>

#include "alg_circuit.h"
#include "tikz_picture.h"
#include "tikz_point.h"
#include "tikz_util.h"

namespace TIKZ {
    extern double GATE_HEIGHT;
    extern double GATE_WIDTH;
    extern double DEFAULT_GATE_SEP_X;
    extern double DEFAULT_GATE_SEP_Y;

    std::map<std::string, tikz_point>
    place_input_gate( picture& p_pic, const std::string& p_text = EMPTY_STR,
                      tikz_point p_topLeft = { .0, .0 }, u64 p_fanOut = 1,
                      const std::string& p_name = EMPTY_STR, color p_color = COLOR_TEXT,
                      color p_bgColor = COLOR_TEXT.to_bg( ), const kv_store& p_options = { } );

    std::map<std::string, tikz_point>
    place_output_gate( picture& p_pic, const std::string& p_text = EMPTY_STR,
                       tikz_point p_topLeft = { .0, .0 }, const std::string& p_name = EMPTY_STR,
                       color p_color = COLOR_TEXT, color p_bgColor = COLOR_TEXT.to_bg( ),
                       const kv_store& p_options = { } );

    std::map<std::string, tikz_point>
    place_const_gate( picture& p_pic, const std::string& p_text = EMPTY_STR,
                      tikz_point p_topLeft = { .0, .0 }, u64 p_fanOut = 1,
                      const std::string& p_name = EMPTY_STR, color p_color = COLOR_TEXT,
                      color p_bgColor = COLOR_TEXT.to_bg( ), const kv_store& p_options = { } );

    std::map<std::string, tikz_point>
    place_op_gate( picture& p_pic, const std::string& p_text = EMPTY_STR,
                   tikz_point p_topLeft = { .0, .0 }, u64 p_fanIn = 2, u64 p_fanOut = 1,
                   const std::string& p_name = EMPTY_STR, color p_color = COLOR_TEXT,
                   color p_bgColor = COLOR_TEXT.to_bg( ), bool p_invert = false,
                   const kv_store& p_options = { } );

    void place_const_connection( picture& p_pic, const std::string& p_startName,
                                 const std::string& p_endName );

    void place_gate_connection( picture& p_pic, const std::string& p_startName, tikz_point p_start,
                                const std::string& p_endName, tikz_point p_end );

    template <typename R, typename T = R>
    std::map<std::string, tikz_point>
    place_gate( picture& p_pic, std::shared_ptr<gate<R, T>> p_gate, tikz_point p_topLeft,
                u64 p_fanOut, const std::string& p_name, color p_color, color p_bgColor,
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
            return place_const_gate( p_pic, math_mode( p_gate->m_name ),
                                     p_topLeft + tikz_point{ GATE_WIDTH / 2, 0 }, p_fanOut, p_name,
                                     p_color, p_bgColor, p_options );
        }
        if( p_gate->is_operator( ) ) {
            return place_op_gate( p_pic, p_gate->m_name, p_topLeft, p_gate->children( ).size( ),
                                  p_fanOut, p_name, p_color, p_bgColor,
                                  !p_invertOps.count( p_gate->m_name ), p_options );
        }
        return { };
    }

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
                    p_pic, c,
                    tikz_point{ p_topLeft.m_x, p_topLeft.m_y - height * DEFAULT_GATE_SEP_Y },
                    p_totalDepth, p_currentDepth + 1, p_name + "_" + std::to_string( i ), p_color,
                    p_bgColor, p_invertOps, p_options );
                conns.merge( cnns );
                height += h;
                lh = conns[ p_name + "_" + std::to_string( i ) + "_out_1" ].m_y;
            }
        }
        // place current gate
        // lh shall become height of last input of current gate
        auto   sz  = p_gate->children( ).size( );
        double ghg = std::max( 2LU, sz ) * GATE_HEIGHT / 2.0;
        double lhg = ( sz > 0 ) ? ghg * ( sz - .5 ) / sz : GATE_HEIGHT / 2.0;

        auto cnns = place_gate<R, T>( p_pic, p_gate, tikz_point{ px, lh + lhg }, 1, p_name, p_color,
                                      p_bgColor, p_invertOps, p_options );
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
                          const std::set<std::string> p_invertOps = { "\\textbullet", "+" },
                          const kv_store&             p_options   = { } ) {
        double height = 0;
        u64    i      = 0;
        for( std::shared_ptr<gate<R, T>> out : p_circuit.m_outputs ) {
            auto res = place_circuit<R, T>( p_pic, out, p_topLeft, p_circuit.depth( ), 0,
                                            p_name + "_" + std::to_string( i++ ), p_color,
                                            p_bgColor, p_invertOps, p_options );
            height += res.first + .5;
            p_topLeft.m_y -= ( .5 + res.first ) * DEFAULT_GATE_SEP_Y;
        }
        return height;
    }
} // namespace TIKZ
