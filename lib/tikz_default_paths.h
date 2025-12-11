#pragma once

#include "tikz_option.h"
#include "tikz_picture.h"
#include "tikz_point.h"

namespace TIKZ {
    void place_arrow( picture& p_pic, tikz_position p_startPos, tikz_position p_endPos,
                      const kv_store& p_options = { },
                      const kv_store& p_basicOptions
                      = OPT::LW_SUPPORT_LINE | OPT::DRAW( COLOR_TEXT ) | OPT::ARR_TIP_LATEX );

    void place_selected_arrow( picture& p_pic, tikz_position p_startPos, tikz_position p_endPos,
                               color p_lineColor, color p_fillColor, double p_angle = 0,
                               const kv_store& p_options = { } );

    void place_cross( picture& p_pic, tikz_position p_position, const kv_store& p_options = { },
                      double p_lineWidth = 2.5, double p_size = 5,
                      const std::string& p_name = EMPTY_STR );
    void place_circle( picture& p_pic, tikz_position p_position, const kv_store& p_options = { },
                       double p_lineWidth = 2.5, double p_size = 5,
                       const std::string& p_name = EMPTY_STR );

    void place_double_cross( picture& p_pic, tikz_position p_position,
                             const kv_store& p_options = { }, double p_lineWidth = 2.5,
                             double p_outlineWidth = .5, double p_size = 5,
                             const std::string& p_name = EMPTY_STR );

    void place_double_circle( picture& p_pic, tikz_position p_position,
                              const kv_store& p_options = { }, double p_lineWidth = 2.5,
                              double p_outlineWidth = .5, double p_size = 5,
                              const std::string& p_name = EMPTY_STR );

    inline void place_batsu( picture& p_pic, tikz_position p_position,
                             color           p_outlineColor = COLOR_C3.deemphasize_weak( ),
                             color           p_fillColor    = COLOR_C3.deemphasize( ),
                             const kv_store& p_options = { }, double p_lineWidth = 2.5,
                             double p_outlineWidth = .5, double p_size = 5,
                             const std::string& p_name = EMPTY_STR ) {
        place_double_cross( p_pic, p_position,
                            OPT::DRAW( p_outlineColor ) | OPT::DOUBLE( p_fillColor ) | p_options,
                            p_lineWidth, p_outlineWidth, p_size, p_name );
    }

    inline void place_maru( picture& p_pic, tikz_position p_position,
                            color           p_outlineColor = COLOR_C1.deemphasize_weak( ),
                            color           p_fillColor    = COLOR_C1.deemphasize( ),
                            const kv_store& p_options = { }, double p_lineWidth = 2.5,
                            double p_outlineWidth = .5, double p_size = 5,
                            const std::string& p_name = EMPTY_STR ) {
        place_double_circle( p_pic, p_position,
                             OPT::DRAW( p_outlineColor ) | OPT::DOUBLE( p_fillColor ) | p_options,
                             p_lineWidth, p_outlineWidth, p_size, p_name );
    }

    inline void place_hatena( picture& p_pic, tikz_position p_position,
                              color           p_outlineColor = COLOR_C2.deemphasize_weak( ),
                              color           p_fillColor    = COLOR_C2.to_bg( ),
                              const kv_store& p_options = { }, double p_outlineWidth = .5,
                              const std::string& p_name = EMPTY_STR ) {
        p_pic.place_double_text( "\\textbf{\\Large ?}", p_position, p_outlineColor, p_fillColor,
                                 p_outlineWidth, p_options, p_name );
    }

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
