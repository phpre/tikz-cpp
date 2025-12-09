#pragma once
#include <deque>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz_option.h"
#include "tikz_point.h"

namespace TIKZ {
    struct command;

    class picture {
      private:
        std::set<std::string>                _packages;
        std::set<std::string>                _libraries;
        kv_store                             _options;
        std::deque<std::shared_ptr<command>> _content;
        bool                                 _final = false;

      public:
        // extra basic option so that they don't need to be specified when other options
        // are specified (but it is still possible to get rid of them if truly needed).
        picture( const kv_store& p_options      = { },
                 const kv_store& p_basicOptions = OPT::TEXT_COLOR );

        render_t render( u64                p_startIndent = 1,
                         const std::string& p_environName = "tikzpicture" ) const;

        inline void finalize( ) {
            _final = true;
        }
        inline bool is_final( ) const {
            return _final;
        }

        std::set<std::string> libraries( ) const;
        std::set<std::string> packages( ) const;

        void add_package( const std::string& p_package );
        void add_library( const std::string& p_library );

        void add_command( std::shared_ptr<command> p_command );

        void add_scope( const picture& p_content );

        void place_coordinate( tikz_position p_position, const std::string& p_name,
                               const kv_store& p_options = { } );

        void place_node( tikz_position p_position, const std::string& p_content = EMPTY_STR,
                         const kv_store& p_options = { }, const std::string& p_name = EMPTY_STR );

        void place_debug_point( tikz_position p_position );

        void place_text( const std::string& p_text, tikz_position p_position,
                         const kv_store&    p_options = OPT::INNER_SEP( "0pt" ),
                         const std::string& p_name    = EMPTY_STR );

        typedef u8                     render_mode_t;
        static constexpr render_mode_t RM_FILL           = 0;
        static constexpr render_mode_t RM_STROKE         = 1;
        static constexpr render_mode_t RM_FILLSTROKE     = 2;
        static constexpr render_mode_t RM_INVISIBLE      = 3;
        static constexpr render_mode_t RM_FILLCLIP       = 4;
        static constexpr render_mode_t RM_STROKECLIP     = 5;
        static constexpr render_mode_t RM_FILLSTROKECLIP = 6;
        static constexpr render_mode_t RM_CLIP           = 7;

        typedef u8                       line_cap_mode_t;
        static constexpr line_cap_mode_t LCM_BUTT              = 0;
        static constexpr line_cap_mode_t LCM_ROUND             = 1;
        static constexpr line_cap_mode_t LCM_PROJECTING_SQUARE = 2;

        typedef u8                        line_join_mode_t;
        static constexpr line_join_mode_t LJM_MITER = 0;
        static constexpr line_join_mode_t LJM_ROUND = 1;
        static constexpr line_join_mode_t LJM_BEVEL = 2;

        void place_double_text( const std::string& p_text, tikz_position p_position,
                                color p_outlineColor = COLOR_WHITE, color p_fillColor = COLOR_TEXT,
                                double             p_outlineWidth = 0.5,
                                const kv_store&    p_options      = OPT::INNER_SEP( "0pt" ),
                                const std::string& p_name         = EMPTY_STR,
                                render_mode_t      p_renderMode   = RM_FILLSTROKE,
                                line_cap_mode_t    p_lineCapMode  = LCM_BUTT,
                                line_join_mode_t   p_lineJoinMode = LJM_MITER );

        void place_line( tikz_position p_topLeft, tikz_position p_bottomRight,
                         const kv_store& p_options = OPT::DRAW );

        void place_vh_line( tikz_position p_topLeft, tikz_position p_bottomRight,
                            const kv_store& p_options = OPT::DRAW );

        void place_hv_line( tikz_position p_topLeft, tikz_position p_bottomRight,
                            const kv_store& p_options = OPT::DRAW );

        void place_vhv_line( tikz_position p_start, tikz_position p_mid, tikz_position p_end,
                             const kv_store& p_options = OPT::DRAW );

        void place_hvh_line( tikz_position p_start, tikz_position p_mid, tikz_position p_end,
                             const kv_store& p_options = OPT::DRAW );

        void place_rectangle( tikz_position p_topLeft, tikz_position p_bottomRight,
                              const kv_store& p_options = OPT::ROUNDED_CORNERS | OPT::DRAW );

        void place_simple_path( std::deque<tikz_position> p_coords,
                                const kv_store&           p_options = OPT::ROUNDED_CORNERS );
        // special stuff

        void place_cross( tikz_position p_position, const kv_store& p_options = { },
                          double p_lineWidth = 2.5, double p_size = 5,
                          const std::string& p_name = EMPTY_STR );
        void place_circle( tikz_position p_position, const kv_store& p_options = { },
                           double p_lineWidth = 2.5, double p_size = 5,
                           const std::string& p_name = EMPTY_STR );

        void place_double_cross( tikz_position p_position, const kv_store& p_options = { },
                                 double p_lineWidth = 2.5, double p_outlineWidth = .5,
                                 double p_size = 5, const std::string& p_name = EMPTY_STR );

        void place_double_circle( tikz_position p_position, const kv_store& p_options = { },
                                  double p_lineWidth = 2.5, double p_outlineWidth = .5,
                                  double p_size = 5, const std::string& p_name = EMPTY_STR );

        inline void place_batsu( tikz_position   p_position,
                                 color           p_outlineColor = COLOR_C3.deemphasize_weak( ),
                                 color           p_fillColor    = COLOR_C3.deemphasize( ),
                                 const kv_store& p_options = { }, double p_lineWidth = 2.5,
                                 double p_outlineWidth = .5, double p_size = 5,
                                 const std::string& p_name = EMPTY_STR ) {
            place_double_cross(
                p_position, OPT::DRAW( p_outlineColor ) | OPT::DOUBLE( p_fillColor ) | p_options,
                p_lineWidth, p_outlineWidth, p_size, p_name );
        }

        inline void place_maru( tikz_position   p_position,
                                color           p_outlineColor = COLOR_C1.deemphasize_weak( ),
                                color           p_fillColor    = COLOR_C1.deemphasize( ),
                                const kv_store& p_options = { }, double p_lineWidth = 2.5,
                                double p_outlineWidth = .5, double p_size = 5,
                                const std::string& p_name = EMPTY_STR ) {
            place_double_circle(
                p_position, OPT::DRAW( p_outlineColor ) | OPT::DOUBLE( p_fillColor ) | p_options,
                p_lineWidth, p_outlineWidth, p_size, p_name );
        }

        inline void place_hatena( tikz_position   p_position,
                                  color           p_outlineColor = COLOR_C2.deemphasize_weak( ),
                                  color           p_fillColor    = COLOR_C2.to_bg( ),
                                  const kv_store& p_options = { }, double p_outlineWidth = .5,
                                  const std::string& p_name = EMPTY_STR ) {
            place_double_text( "\\textbf{\\Large ?}", p_position, p_outlineColor, p_fillColor,
                               p_outlineWidth, p_options, p_name );
        }
    };
} // namespace TIKZ
