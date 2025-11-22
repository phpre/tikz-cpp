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

        inline command( kv_store p_options = { }, std::set<u64> p_times = { } )
            : m_times{ p_times }, _libraries{ }, _packages{ }, _options{ p_options } {
        }

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

    struct path_operation {
        tikz_position m_position; // target position

        inline path_operation( tikz_position p_position ) : m_position{ p_position } {
        }

        inline virtual ~path_operation( ) {
        }

        virtual std::set<std::string> libraries( ) const = 0;
        virtual std::set<std::string> packages( ) const  = 0;

        virtual render_t render_op( u64 p_time = 0, u64 p_startIndent = 2 ) const = 0;
    };

    struct move_to_operation : public path_operation {
        std::set<u64> m_times; // time points at which this command is active
      protected:
        kv_store _options;

      public:
        inline move_to_operation( tikz_position p_position, kv_store p_options = { },
                                  std::set<u64> p_times = { } )
            : path_operation( p_position ), m_times{ p_times }, _options{ p_options } {
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

        virtual render_t render_op( u64 p_time = 0, u64 p_startIndent = 2 ) const;
    };

    struct line_to_operation : public move_to_operation {
        inline line_to_operation( tikz_position p_position, kv_store p_options = { },
                                  std::set<u64> p_times = { } )
            : move_to_operation( p_position, p_options, p_times ) {
        }

        virtual render_t render_op( u64 p_time = 0, u64 p_startIndent = 2 ) const;
    };

    struct rectangle_operation : public move_to_operation {
        inline rectangle_operation( tikz_position p_position, std::set<u64> p_times = { } )
            : move_to_operation( p_position, { }, p_times ) {
        }

        virtual render_t render_op( u64 p_time = 0, u64 p_startIndent = 2 ) const;
    };

    struct path_command : public command {
        tikz_position                               m_startPosition;
        std::deque<std::shared_ptr<path_operation>> m_operations;

        inline path_command( tikz_position                               p_position,
                             std::deque<std::shared_ptr<path_operation>> p_operations,
                             const kv_store& p_options = { }, std::set<u64> p_times = { } )
            : command( p_options, p_times ), m_startPosition{ p_position },
              m_operations{ p_operations } {
        }

        virtual render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const;

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
                                   const kv_store& p_options = { }, std::set<u64> p_times = { } )
            : command( p_options, p_times ), path_operation( p_position ), m_name{ p_name } {
        }

        virtual render_t        render( u64 p_time, u64 p_startIndent, bool p_internal ) const;
        virtual inline render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const {
            return render( p_time, p_startIndent, false );
        }

        virtual inline render_t render_op( u64 p_time = 0, u64 p_startIndent = 2 ) const {
            return render( p_time, p_startIndent, true );
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
                             const kv_store& p_options = { }, const std::string& p_name = EMPTY_STR,
                             std::set<u64> p_times = { } )
            : coordinate_command( p_position, p_name, p_options, p_times ), m_content{ p_content } {
        }

        virtual render_t render( u64 p_time, u64 p_startIndent, bool p_internal ) const;
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
    struct scope_command : public command {
        virtual render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const {
            return { };
        }
    };
    */
} // namespace TIKZ
