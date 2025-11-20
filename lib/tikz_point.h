#pragma once

#include <algorithm>
#include <cstdio>
#include <string>

namespace TIKZ {
    struct tikz_point {
        double m_x;
        double m_y;

        tikz_point( double p_x, double p_y ) : m_x{ p_x }, m_y{ p_y } {};

        tikz_point( const std::pair<double, double>& p_point )
            : m_x{ p_point.first }, m_y{ p_point.second } {};

        inline operator std::pair<double, double>( ) const {
            return std::pair<double, double>{ m_x, m_y };
        }

        inline tikz_point operator*( double p_scale ) const {
            return { m_x * p_scale, m_y * p_scale };
        }

        inline tikz_point operator+( tikz_point p_other ) const {
            return { m_x + p_other.m_x, m_y + p_other.m_y };
        }
    };

    std::pair<tikz_point, tikz_point> translate_right( std::pair<tikz_point, tikz_point> p_pos,
                                                       double                            p_amount );

    std::pair<tikz_point, tikz_point> translate_down( std::pair<tikz_point, tikz_point> p_pos,
                                                      double                            p_amount );

    class tikz_position {
        enum class type { POINT, NAME };
        type _type = type::POINT;

        tikz_point  _point{ 0.0, 0.0 };
        std::string _name = EMPTY_STR;

      public:
        inline tikz_position( tikz_point p_point = tikz_point{ 0.0, 0.0 } )
            : _type{ type::POINT }, _point{ p_point } {
        }

        inline tikz_position( const std::string& p_name ) : _type{ type::NAME }, _name{ p_name } {
        }

        inline std::string to_string( ) const {
            switch( _type ) {
            case type::POINT:
            default: {
                char buffer[ 30 ] = { 0 };
                snprintf( buffer, 29, "%5.3lf, %5.3lf", _point.m_x, _point.m_y );
                return std::string{ buffer };
            }
            case type::NAME: return _name;
            }
        }
    };
} // namespace TIKZ
