#pragma once

#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz.h"
#include "tikz_option.h"

namespace TIKZ {
    struct command {
        std::set<u64> m_times; // time points at which this command is active

      protected:
        std::set<std::string> _libraries; // dependencies
        std::set<std::string> _packages;  // dependencies
        kv_store              _options;

      public:
        virtual render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const = 0;

        inline virtual ~command( ) {
        }

        virtual inline std::set<std::string> libraries( ) const {
            std::set<std::string> res{ };
            res.insert_range( _libraries );
            res.insert_range( _options.libraries( ) );
            return res;
        }

        virtual inline std::set<std::string> packages( ) const {
            std::set<std::string> res{ };
            res.insert_range( _packages );
            res.insert_range( _options.packages( ) );
            return res;
        }
    };

    struct node_command : public command {
        tikz_position m_position;
        std::string   m_name    = EMPTY_STR;
        std::string   m_content = EMPTY_STR;

        static node_command place_text( const std::string& p_text, tikz_position p_position,
                                        const kv_store&    p_options = OPT::INNER_SEP( "0pt" ),
                                        const std::string& p_name    = EMPTY_STR );

        static node_command place_cross( tikz_position p_position, const kv_store& p_options = { },
                                         double p_lineWidth = 2.5, double p_size = 5,
                                         const std::string& p_name = EMPTY_STR );

        static node_command place_circle( tikz_position p_position, const kv_store& p_options = { },
                                          double p_lineWidth = 2.5, double p_size = 5,
                                          const std::string& p_name = EMPTY_STR );

        static node_command place_double_cross( tikz_position   p_position,
                                                const kv_store& p_options   = { },
                                                double          p_lineWidth = 2.5,
                                                double p_outlineWidth = .5, double p_size = 5,
                                                const std::string& p_name = EMPTY_STR );

        static node_command place_double_circle( tikz_position   p_position,
                                                 const kv_store& p_options   = { },
                                                 double          p_lineWidth = 2.5,
                                                 double p_outlineWidth = .5, double p_size = 5,
                                                 const std::string& p_name = EMPTY_STR );

        virtual render_t render( u64 p_time, u64 p_startIndent, bool p_internal ) const;

        virtual inline render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const {
            return render( p_time, p_startIndent, false );
        }
    };

    struct math_command : public command {
        std::string m_name;
        std::string m_macro;

        virtual inline render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const {
            if( p_time && !m_times.count( p_time ) ) { return { }; }
            if( !m_times.empty( ) && !m_times.count( p_time ) ) { return { }; }

            std::string result = "\\pgfmathsetmacro";
            result += "\\" + m_name;
            result += "{" + m_macro + "}";
            return { { p_startIndent, result } };
        }
    };

    /*
    struct path_command : public command {
        virtual render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const {
            return { };
        }
    };


    struct scope_command : public command {
        virtual render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const {
            return { };
        }
    };
    */
} // namespace TIKZ
