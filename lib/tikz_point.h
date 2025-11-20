#pragma once

#include <algorithm>
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

    union tikz_position {
        tikz_point  m_point;
        std::string m_name;
    };
} // namespace TIKZ
