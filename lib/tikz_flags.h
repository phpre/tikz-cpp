#pragma once
#include "tikz_color.h"

namespace TIKZ {
    // use filled crosses to symbolize mismatches
    extern bool CROSS_FILL;

    extern double CHAR_WIDTH;
    extern double CHAR_HEIGHT;
    extern double CHAR_GLOW;

    extern color MAT_COL;
    extern color DEL_COL;
    extern color INS_COL;
    extern color SUB_COL;
    extern color SEP_COL;

    extern double DEFAULT_SLICE_SPACING;

    extern double DEFAULT_TRIE_VERTEX_DIST_X;
    extern double DEFAULT_TRIE_VERTEX_DIST_Y;
    extern double DEFAULT_TRIE_CHAR_DIVERTION;

} // namespace TIKZ
