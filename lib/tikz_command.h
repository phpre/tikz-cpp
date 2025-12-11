#pragma once
#include <deque>
#include <memory>
#include <set>
#include <string>

#include "defines.h"
#include "tikz_option.h"
#include "tikz_picture.h"

namespace TIKZ {
    struct command {
      protected:
        std::set<std::string> _libraries; // dependencies
        std::set<std::string> _packages;  // dependencies
        kv_store              _options;

        inline command( kv_store p_options = { } )
            : _libraries{ }, _packages{ }, _options{ p_options } {
        }

      public:
        virtual render_t render( u64 p_startIndent = 1 ) const = 0;

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

    struct path_operation {
        tikz_position m_position; // target position

        inline path_operation( tikz_position p_position ) : m_position{ p_position } {
        }

        inline virtual ~path_operation( ) {
        }

        virtual std::set<std::string> libraries( ) const = 0;
        virtual std::set<std::string> packages( ) const  = 0;

        virtual render_t render_op( u64 p_startIndent = 2 ) const = 0;
    };

    struct arc_to_operation : public path_operation {
        double m_startAngle;
        double m_endAngle;
        double m_radius;

      public:
        inline arc_to_operation( double p_startAngle, double p_endAngle, double p_radius )
            : path_operation( tikz_position{ } ), m_startAngle{ p_startAngle },
              m_endAngle{ p_endAngle }, m_radius{ p_radius } {
        }

        virtual inline std::set<std::string> libraries( ) const {
            return { };
        }

        virtual inline std::set<std::string> packages( ) const {
            return { };
        }

        virtual render_t render_op( u64 p_startIndent = 2 ) const;
    };

    struct move_to_operation : public path_operation {
      protected:
        kv_store _options;

      public:
        inline move_to_operation( tikz_position p_position, kv_store p_options = { } )
            : path_operation( p_position ), _options{ p_options } {
        }

        virtual inline std::set<std::string> libraries( ) const {
            std::set<std::string> res{ };
            res.insert_range( _options.libraries( ) );
            return res;
        }

        virtual inline std::set<std::string> packages( ) const {
            std::set<std::string> res{ };
            res.insert_range( _options.packages( ) );
            return res;
        }

        virtual render_t render_op( u64 p_startIndent = 2 ) const;
    };

    struct line_to_operation : public move_to_operation {
        inline line_to_operation( tikz_position p_position, kv_store p_options = { } )
            : move_to_operation( p_position, p_options ) {
        }

        virtual render_t render_op( u64 p_startIndent = 2 ) const;
    };

    struct vh_line_to_operation : public move_to_operation {
        inline vh_line_to_operation( tikz_position p_position, kv_store p_options = { } )
            : move_to_operation( p_position, p_options ) {
        }

        virtual render_t render_op( u64 p_startIndent = 2 ) const;
    };

    struct hv_line_to_operation : public move_to_operation {
        inline hv_line_to_operation( tikz_position p_position, kv_store p_options = { } )
            : move_to_operation( p_position, p_options ) {
        }

        virtual render_t render_op( u64 p_startIndent = 2 ) const;
    };

    struct rectangle_operation : public move_to_operation {
        inline rectangle_operation( tikz_position p_position )
            : move_to_operation( p_position, { } ) {
        }

        virtual render_t render_op( u64 p_startIndent = 2 ) const;
    };

    struct scope_operation : public path_operation {
      protected:
        std::deque<std::shared_ptr<path_operation>> m_operations;

      public:
        inline scope_operation( std::deque<std::shared_ptr<path_operation>> p_operations )
            : path_operation( tikz_position{ } ), m_operations{ p_operations } {
        }

        virtual inline std::set<std::string> libraries( ) const {
            std::set<std::string> res{ };
            for( const auto& op : m_operations ) { res.insert_range( op->libraries( ) ); }
            return res;
        }

        virtual inline std::set<std::string> packages( ) const {
            std::set<std::string> res{ };
            for( const auto& op : m_operations ) { res.insert_range( op->packages( ) ); }
            return res;
        }

        virtual render_t render_op( u64 p_startIndent = 1 ) const;
    };

    struct path_command : public command {
        tikz_position                               m_startPosition;
        std::deque<std::shared_ptr<path_operation>> m_operations;

        inline path_command( tikz_position                               p_position,
                             std::deque<std::shared_ptr<path_operation>> p_operations,
                             const kv_store&                             p_options = { } )
            : command( p_options ), m_startPosition{ p_position }, m_operations{ p_operations } {
        }

        virtual render_t render( u64 p_startIndent = 1 ) const;

        virtual inline std::set<std::string> libraries( ) const {
            std::set<std::string> res{ };
            res.insert_range( _libraries );
            res.insert_range( _options.libraries( ) );
            for( const auto& op : m_operations ) { res.insert_range( op->libraries( ) ); }
            return res;
        }

        virtual inline std::set<std::string> packages( ) const {
            std::set<std::string> res{ };
            res.insert_range( _packages );
            res.insert_range( _options.packages( ) );
            for( const auto& op : m_operations ) { res.insert_range( op->packages( ) ); }
            return res;
        }
    };

    struct coordinate_command : public command, public path_operation {
        std::string m_name = EMPTY_STR;

        inline coordinate_command( tikz_position p_position, const std::string& p_name,
                                   const kv_store& p_options = { } )
            : command( p_options ), path_operation( p_position ), m_name{ p_name } {
        }

        virtual render_t        render( u64 p_startIndent, bool p_internal ) const;
        virtual inline render_t render( u64 p_startIndent = 1 ) const {
            return render( p_startIndent, false );
        }

        virtual inline render_t render_op( u64 p_startIndent = 2 ) const {
            return render( p_startIndent, true );
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

    struct node_command : public coordinate_command {
        std::string m_content = EMPTY_STR;

        inline node_command( tikz_position p_position, const std::string& p_content = EMPTY_STR,
                             const kv_store&    p_options = { },
                             const std::string& p_name    = EMPTY_STR )
            : coordinate_command( p_position, p_name, p_options ), m_content{ p_content } {
        }

        virtual render_t render( u64 p_startIndent, bool p_internal ) const;
    };

    struct math_command : public command {
        std::string m_name;
        std::string m_macro;

        inline math_command( const std::string& p_name, const std::string& p_macro,
                             const kv_store& p_options = { } )
            : command( p_options ), m_name{ p_name }, m_macro{ p_macro } {
        }

        virtual inline render_t render( u64 p_startIndent = 1 ) const {
            std::string result = "\\pgfmathsetmacro";
            result += "\\" + m_name;
            result += "{" + m_macro + "}";
            return { { p_startIndent, result } };
        }
    };

    struct scope_command : public command {
      protected:
        picture _content;

      public:
        inline scope_command( const picture& p_content ) : command( ), _content{ p_content } {
        }

        virtual std::set<std::string> libraries( ) const;
        virtual std::set<std::string> packages( ) const;

        virtual render_t render( u64 p_startIndent = 1 ) const;
    };
} // namespace TIKZ
