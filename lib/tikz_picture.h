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

      public:
        // extra basic option so that they don't need to be specified when other options
        // are specified (but it is still possible to get rid of them if truly needed).
        picture( const kv_store& p_options      = { },
                 const kv_store& p_basicOptions = OPT::TEXT_COLOR );

        render_t render( u64                p_startIndent = 1,
                         const std::string& p_environName = "tikzpicture" ) const;

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
    };
} // namespace TIKZ
