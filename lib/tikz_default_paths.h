#pragma once

#include "tikz_option.h"
#include "tikz_picture.h"
#include "tikz_point.h"

namespace TIKZ {
    enum class puzzle_bend_t { STRAIGHT, CURVED_IN, CURVED_OUT };
    void place_puzzle_piece( picture& p_pic, tikz_point p_center,
                             const kv_store& p_extraOptions = { },
                             puzzle_bend_t   p_top          = puzzle_bend_t::CURVED_IN,
                             puzzle_bend_t   p_left         = puzzle_bend_t::CURVED_OUT,
                             puzzle_bend_t   p_bottom       = puzzle_bend_t::CURVED_IN,
                             puzzle_bend_t   p_right        = puzzle_bend_t::CURVED_OUT,
                             double p_cornerBend = 2, double p_innerBend = 6,
                             double p_outerBend = 5, double p_coreSize = 2 * .975,
                             double p_totalSize = 2 * 1.55, double p_bendpointSize = 2 * 1.275,
                             const kv_store& p_options = OPT::DRAW | OPT::FILL );

} // namespace TIKZ
