#include "tikz_picture.h"

namespace TIKZ {
    picture::picture( kv_store p_options )
        : _packages{ }, _libraries{ }, _options{ p_options }, _content{ } {
    }

    render_t picture::render( u64 p_time, u64 p_startIndent ) const {
        render_t result{ };

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
        _packages.insert( p_package );
    }

    void picture::add_library( const std::string& p_library ) {
        _libraries.insert( p_library );
    }

    void picture::add_command( std::shared_ptr<command> p_command ) {
        if( !p_command ) { return; }
        _content.emplace_back( p_command );
    }

    void picture::place_text( const std::string& p_text, tikz_position p_position,
                              const kv_store& p_options, const std::string& p_name ) {
        _content.emplace_back( std::make_shared<TIKZ::node_command>(
            TIKZ::node_command::place_text( p_text, p_position, p_options, p_name ) ) );
    }

    void picture::place_cross( tikz_position p_position, const kv_store& p_options,
                               double p_lineWidth, double p_size, const std::string& p_name ) {
        _content.emplace_back(
            std::make_shared<TIKZ::node_command>( TIKZ::node_command::place_cross(
                p_position, p_options, p_lineWidth, p_size, p_name ) ) );
    }

    void picture::place_circle( tikz_position p_position, const kv_store& p_options,
                                double p_lineWidth, double p_size, const std::string& p_name ) {
        _content.emplace_back(
            std::make_shared<TIKZ::node_command>( TIKZ::node_command::place_circle(
                p_position, p_options, p_lineWidth, p_size, p_name ) ) );
    }

    void picture::place_double_cross( tikz_position p_position, const kv_store& p_options,
                                      double p_lineWidth, double p_outlineWidth, double p_size,
                                      const std::string& p_name ) {
        _content.emplace_back(
            std::make_shared<TIKZ::node_command>( TIKZ::node_command::place_double_cross(
                p_position, p_options, p_lineWidth, p_outlineWidth, p_size, p_name ) ) );
    }

    void picture::place_double_circle( tikz_position p_position, const kv_store& p_options,
                                       double p_lineWidth, double p_outlineWidth, double p_size,
                                       const std::string& p_name ) {
        _content.emplace_back(
            std::make_shared<TIKZ::node_command>( TIKZ::node_command::place_double_circle(
                p_position, p_options, p_lineWidth, p_outlineWidth, p_size, p_name ) ) );
    }

} // namespace TIKZ
