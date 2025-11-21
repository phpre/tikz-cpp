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
        std::set<u64>         m_times;     // time points at which this command is active
        std::set<std::string> m_libraries; // dependencies
        std::set<std::string> m_packages;  // dependencies
        kv_store              m_options;

        virtual render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const = 0;

        inline virtual ~command( ) {
        }
    };

    struct node_command : public command {
        tikz_position m_position;
        std::string   m_name    = EMPTY_STR;
        std::string   m_content = EMPTY_STR;

        static node_command place_text( const std::string& p_text, tikz_position p_position,
                                        const kv_store&    p_options = OPT::TEXT_COLOR,
                                        const std::string& p_name    = EMPTY_STR );

        virtual render_t render( u64 p_time, u64 p_startIndent, bool p_internal ) const;

        virtual inline render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const {
            return render( p_time, p_startIndent, false );
        }
    };

    /*
    struct path_command : public command {
        virtual render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const {
            return { };
        }
    };

    struct math_command : public command {
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
