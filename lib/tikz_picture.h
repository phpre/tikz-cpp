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
      public:
        std::set<std::string>              m_packages;
        std::set<std::string>              m_libraries;
        std::map<std::string, std::string> m_globalMacros;

      private:
        std::map<std::string, std::string>   _localMacros;
        kv_store                             _options;
        std::deque<std::shared_ptr<command>> _content;

      public:
        picture( kv_store p_options = { } );

        render_t render( u64 p_time = 0, u64 p_startIndent = 1 ) const;

        void add_package( const std::string& p_package );
        void add_library( const std::string& p_library );
        void add_global_macro( const std::string& p_name, const std::string& p_content );
        void add_local_macro( const std::string& p_name, const std::string& p_content );

        void add_command( std::shared_ptr<command> p_command );

        void place_text( const std::string& p_text, tikz_position p_position,
                         const kv_store&    p_options = OPT::TEXT_COLOR,
                         const std::string& p_name    = EMPTY_STR );
    };
} // namespace TIKZ
