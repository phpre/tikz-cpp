#include "tikz_picture.h"

namespace TIKZ {
    picture::picture( kv_store p_options )
        : _packages{ }, _libraries{ }, _globalMacros{ }, _localMacros{ }, _options{ p_options },
          _content{ } {
    }

    std::string picture::to_string( u32 p_time ) const {
        std::string result = EMPTY_STR;

        // for( auto cmd : _content ) { result += cmd->to_string( p_time ); }

        if( result == EMPTY_STR ) {
            // picture is empty
            return result;
        }

        std::string env = "\\begin{tikzpicture}[";
        for( const auto& [ k, v ] : _options ) {
            env += k;
            if( v != EMPTY_STR ) { env += " = " + v; }
            env += ",";
        }
        env += "]\n";

        return env + result + "\\end{tikzpicture}\n";
    }
} // namespace TIKZ
