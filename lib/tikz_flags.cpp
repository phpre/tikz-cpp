#include "tikz_flags.h"

namespace TIKZ {
    bool CROSS_FILL = false;

    double CHAR_WIDTH  = .5;
    double CHAR_HEIGHT = .5;
    double CHAR_GLOW   = .075;

    color MAT_COL = COLOR_C1;
    color DEL_COL = COLOR_C3;
    color INS_COL = COLOR_C4;
    color SUB_COL = COLOR_C5;
    color SEP_COL = COLOR_C2;

    double DEFAULT_SLICE_SPACING = 0.25;

    double DEFAULT_TRIE_VERTEX_DIST_X  = 2.5 * CHAR_WIDTH;
    double DEFAULT_TRIE_VERTEX_DIST_Y  = -1.5 * CHAR_HEIGHT;
    double DEFAULT_TRIE_CHAR_DIVERTION = CHAR_WIDTH / 6;
} // namespace TIKZ
