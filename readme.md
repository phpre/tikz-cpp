tikz-cpp
==

A cpp library to generate tikz code. Featrures are added as needed; the whole library may be considered at an alpha state, so use at your own risk.

Components
--
All of the files below are in `lib/`.
* General utility
  - `fragmentco.h`: data structure for a closed-open interval of integers (`fragmentco`)
  - `breakpoint.h`: alignments between strings (`breakpoint`, `breakpoint_repn` breakpoint representation)
    - Compute an optimal (breakpoint representation of) an alignment between two strings (`breakpoint_repn compute_breakpoints( const std::string& p_P, const std::string& p_T )`)
    - Compute a slice of an alignment (`breakpoint_repn breakpoint_slice( const breakpoint_repn& p_brpts, fragmentco p_frag )`) and the image of a fragment under an alignment (`fragmentco align_fragment( const breakpoint_repn& p_brpts, fragmentco p_frag`)
    - Normalize a breakpoint representation to start with (0,0) (`breakpoint_repn breakpoint_normalize( const breakpoint_repn& p_brpts )`)
  - `ag_slice.h`: slices of the alignment graph of two strings (`graph_slice`), see [[CKW25, Definition 4.3]](https://arxiv.org/abs/2510.17752); for each slice store its status as pure, top, bottom, primitive.
    - generate a list of slices from an alignment (`static std::deque<graph_slice> graph_slice::from_alignment( const breakpoint_repn& p_brpnt, u32 p_d )`)
    - merge a list of slices into a new slice (`static graph_slice graph_slice::merged_slice( const std::deque<graph_slice>& p_primitiveSlices, fragmentco p_fragment )`)
    - group/merge a list of primitive slices into a sequence of one top, several pure/non-pure, and onte bottom slice (`static std::deque<graph_slice> graph_slice::top_pure_bot_decomp( const std::deque<graph_slice>& p_primitiveSlices )`)
* General tikz Utility
  - `tikz.h`: `color`, default color aliases, `tikz_point`, default line width aliases
    - Open a new file `new_document` (also adds default tex code needed for a `standalone` and loads packages; ends with printing `\begin{document}`, close a document `finish_document` (also writes `\end{document}`
    - Start a new page and a new tikzpicture `initialize_tikzpicture`; end a tikzpicture and a page `finish_tikzpicture`
    - print coordinates, nodes, text, and other basic tikz shapes
* Application-specific tikz Code Generation
  - `tikz_string.h`: generate pictures involving string or sequence data.
    - `stylized_string`: data structure to represent a string to be displayed either just by its name or by its individual characters
    - `void print_[vertical_]string( FILE* p_out, const stylized_string& p_S, tikz_point p_StopLeft )`: writes tikz code to print the given string (vertically).
    - `void print_matched_string_pair( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft, const stylized_string& p_T, tikz_point p_TtopLeft, color p_bgColor )`: prints two strings inside of a "glow bubble" and addes thin lines between matching pairs of characters. (See [[CKW25, Figure 4, colored boxes]](https://arxiv.org/abs/2510.17752) for an example.)
    - `void print_separator( FILE* p_out, tikz_point p_PtopLeft, tikz_point p_TtopLeft, color p_color )`: prints separator of a breakpoint representation (withoult a label; See [[CKW25, Figure 4]](https://arxiv.org/abs/2510.17752) for an example)
    - `std::pair<tikz_point, tikz_point> print_alignment( FILE* p_out, const stylized_string& p_P, tikz_point p_PtopLeft, const stylized_string& p_T, tikz_point p_TtopLeft, const breakpoint_repn& p_brpnt )`: prints an alignment between two strings. (See [[CKW25, Figure 4]](https://arxiv.org/abs/2510.17752) for an example.)
    - `vertex_grid print_alignment_graph( FILE* p_out, const std::string& p_P, fragmentco p_fragP, const std::string& p_T, fragmentco p_fragT )`, prints the alignment graph of two strings (without the string labels). (See [[CKW25, Figure 3]](https://arxiv.org/abs/2510.17752) for an example.)
    - `void print_alignment_on_coordinates( FILE* p_out, const vertex_grid& p_vg, const breakpoint_repn& p_brpnt )`: prints an alignment into a given alignment graph as a path of colored arrows. (See [[CKW25, Figure 3]](https://arxiv.org/abs/2510.17752) for an example.)
  - `tikz_graph.h`: generate pictures involving graphs
    - basic utility: `vertex` for all kinds of different ways to style a vertex of a graph; `print_vertex(...)`, `print_arrow(...)`. `print_selected_arrow(...)`
    - `vertex_grid` for producing (and printing) a set of 2D tikz-coordinates on which vertices or other objects may be placed

Build Instructions
--
- A `make` instruction
  - builds the library `libtikzcpp.a`.
  - for each file `src/<>.cpp` in `src/`, produces an executable `<>` (linking in `libtikzcpp.a`) and runs `<>` (which is supposed to write `*.tex` files into the `figs/` folder.
  - compiles each file `figs/{}.tex` into `out/{}.pdf` using `latexmk` and `LuaLatex`.
- `make out/lib/libtikzcpp.a` generates just the library
- For file `src/<>.cpp`, running `make generate_figures_<>` generates and compiles all figures from `src/<>.cpp`.
- `make clean` removes all intermediate files, but not the outputs in `out/`

Usage
--
Below is a list of publications that uses this library for its figures, as well as example figures from said publications (reproduced with permission; note the original copyright).
- [[CKW25]](https://arxiv.org/abs/2510.17752); file `src/2025-10-ckw.cpp` generates Figures 3 to 7. <br/><br/>
  <img width="45%" alt="[CKW25, Figure 3]" src="https://github.com/user-attachments/assets/59d37486-ba64-47a3-a52f-341d4f121074" /> 
  <img width="45%" alt="[CKW25, Figure 4]" src="https://github.com/user-attachments/assets/1b379ef3-4cc4-4fa2-b3a7-264a9e04197f" />
