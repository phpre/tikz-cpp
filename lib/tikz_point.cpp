#include "tikz_point.h"

namespace TIKZ {
    std::pair<tikz_point, tikz_point> translate_right( std::pair<tikz_point, tikz_point> p_pos,
                                                       double p_amount ) {
        return { tikz_point{ p_pos.first.m_x + p_amount, p_pos.first.m_y },
                 tikz_point{ p_pos.second.m_x + p_amount, p_pos.second.m_y } };
    }

    std::pair<tikz_point, tikz_point> translate_down( std::pair<tikz_point, tikz_point> p_pos,
                                                      double                            p_amount ) {
        return { tikz_point{ p_pos.first.m_x, p_amount + p_pos.first.m_y },
                 tikz_point{ p_pos.second.m_x, p_amount + p_pos.second.m_y } };
    }
} // namespace TIKZ
