#pragma once
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz_command.h"
#include "tikz_point.h"

namespace TIKZ {
    class picture {
      private:
        std::set<std::string>                _packages;
        std::set<std::string>                _libraries;
        kv_store                             _options;
        std::deque<std::shared_ptr<command>> _content;

      public:
        picture( kv_store p_options = OPT::TEXT_COLOR );

        render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const;

        inline std::set<std::string> libraries( ) const {
            std::set<std::string> res{ };
            res.insert_range( _libraries );
            res.insert_range( _options.libraries( ) );
            for( const auto& cmd : _content ) { res.insert_range( cmd->libraries( ) ); }
            return res;
        }

        inline std::set<std::string> packages( ) const {
            std::set<std::string> res{ };
            res.insert_range( _packages );
            res.insert_range( _packages );
            for( const auto& cmd : _content ) { res.insert_range( cmd->packages( ) ); }
            return res;
        }

        void add_package( const std::string& p_package );
        void add_library( const std::string& p_library );

        void add_command( std::shared_ptr<command> p_command );

        void place_text( const std::string& p_text, tikz_position p_position,
                         const kv_store&    p_options = OPT::INNER_SEP( "0pt" ),
                         const std::string& p_name    = EMPTY_STR );

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
