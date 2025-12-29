#pragma once
#include <deque>
#include <map>
#include <string>

#include "alg_circuit.h"
#include "tikz_picture.h"
#include "tikz_point.h"

namespace TIKZ {
    constexpr double GATE_HEIGHT = .5;
    constexpr double GATE_WIDTH  = .5;

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

} // namespace TIKZ
