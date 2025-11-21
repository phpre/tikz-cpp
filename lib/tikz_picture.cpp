#include "tikz_picture.h"

namespace TIKZ {
    picture::picture( kv_store p_options )
        : m_packages{ }, m_libraries{ }, m_globalMacros{ }, _localMacros{ }, _options{ p_options },
          _content{ } {
    }

    render_t picture::render( u64 p_time, u64 p_startIndent ) const {
        render_t result{ };

        for( const auto& [ name, str ] : _localMacros ) {
            result.push_back( { p_startIndent, str } );
        }

        if( _content.empty( ) ) {
            // picture is empty
            return result;
        }

        for( const auto& cmd : _content ) {
            result.append_range( cmd->render( p_time, p_startIndent + 1 ) );
        }

        std::string env = "\\begin{tikzpicture}";
        if( !_options.empty( ) ) {
            env += "[";
            env += _options.to_string( );
            env += "]";
        }

        result.push_front( { p_startIndent, env } );
        result.push_back( { p_startIndent, "\\end{tikzpicture}" } );

        return result;
    }

    void picture::add_package( const std::string& p_package ) {
        m_packages.insert( p_package );
    }

    void picture::add_library( const std::string& p_library ) {
        m_libraries.insert( p_library );
    }

    void picture::add_global_macro( const std::string& p_name, const std::string& p_content ) {
        m_globalMacros[ p_name ] = p_content;
    }

    void picture::add_local_macro( const std::string& p_name, const std::string& p_content ) {
        _localMacros[ p_name ] = p_content;
    }

    void picture::add_command( std::shared_ptr<command> p_command ) {
        if( !p_command ) { return; }

        m_libraries.insert_range( p_command->m_libraries );
        m_packages.insert_range( p_command->m_packages );

        _content.emplace_back( p_command );
    }

    void picture::place_text( const std::string& p_text, tikz_position p_position,
                              const kv_store& p_options, const std::string& p_name ) {
        _content.emplace_back( std::make_shared<TIKZ::node_command>(
            TIKZ::node_command::place_text( p_text, p_position, p_options, p_name ) ) );
    }
} // namespace TIKZ
