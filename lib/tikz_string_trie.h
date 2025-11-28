#pragma once
#include <string>

#include "tikz_string.h"

namespace TIKZ {
    typedef std::pair<u64, u64> trie_point;
    constexpr trie_point        TRIE_ROOT = { 0, 0 };

    struct placed_trie_vertex {
        bool                m_marked;
        u64                 m_height;
        tikz_point          m_pos{ 0.0, 0.0 };
        std::string         m_name;
        std::map<char, u64> m_next{ };
        trie_point          m_parent{ };
    };

    constexpr double DEFAULT_TRIE_VERTEX_DIST_X  = 2.5 * CHAR_WIDTH;
    constexpr double DEFAULT_TRIE_VERTEX_DIST_Y  = -1.5 * CHAR_HEIGHT;
    constexpr double DEFAULT_TRIE_CHAR_DIVERTION = CHAR_WIDTH / 6;

    class placed_trie {
      public:
        typedef std::map<trie_point, placed_trie_vertex> placed_trie_vertices_t;

      private:
        placed_trie_vertices_t _vertices{ };
        tikz_point             _topLeft;
        double                 _distX;
        double                 _distY;
        std::string            _name;
        u64                    _depth{ 0 };

      public:
        explicit placed_trie( const trie& p_trie, tikz_point p_topLeft = { .0, .0 },
                              double             p_distX = DEFAULT_TRIE_VERTEX_DIST_X,
                              double             p_distY = DEFAULT_TRIE_VERTEX_DIST_Y,
                              const std::string& p_name  = EMPTY_STR );

        inline u64 depth( ) const {
            return _depth;
        }

        tikz_point vertex_position( u64 p_depth, u64 p_height ) const;

        inline placed_trie_vertex operator[]( trie_point p_point ) const {
            return _vertices.at( p_point );
        }

        inline trie_point next( trie_point p_point, char p_char ) const {
            const auto& vtx = ( *this )[ p_point ];
            auto        j   = vtx.m_next.at( p_char );
            return { p_point.first + 1, j };
        }

        inline std::deque<trie_point> in_order( trie_point p_start = TRIE_ROOT ) {
            std::deque<trie_point> res{ p_start };
            for( const auto& [ c, n ] : ( *this )[ p_start ].m_next ) {
                res.append_range( in_order( { p_start.first + 1, n } ) );
            }
            return res;
        }
    };

    placed_trie place_trie_coordinates( picture& p_pic, const trie& p_trie,
                                        tikz_point         p_topLeft = { .0, .0 },
                                        double             p_distX   = 2.5 * CHAR_WIDTH,
                                        double             p_distY   = -1.5 * CHAR_HEIGHT,
                                        const std::string& p_name    = EMPTY_STR );

    placed_trie place_trie_vertices( picture& p_pic, const trie& p_trie,
                                     tikz_point         p_topLeft = { .0, .0 },
                                     double             p_distX   = 2.5 * CHAR_WIDTH,
                                     double             p_distY   = -1.5 * CHAR_HEIGHT,
                                     const std::string& p_name    = EMPTY_STR );

    void place_trie_vertex( picture& p_pic, const placed_trie_vertex& p_vertex,
                            const kv_store& p_options );

    void place_trie_edge( picture& p_pic, placed_trie_vertex p_start, placed_trie_vertex p_end,
                          const stylized_string& p_label, const kv_store& p_options = { } );

    void place_diverted_trie_edge( picture& p_pic, placed_trie_vertex p_start,
                                   placed_trie_vertex p_end, const stylized_string& p_label,
                                   double p_downwardDivertion, double p_beginDivertion,
                                   const kv_store& p_options = { }, double p_lineWidth = 1.25 );

    void place_trie_string_on_coordinates( picture& p_pic, const placed_trie& p_trie,
                                           const std::string& p_string,
                                           const kv_store&    p_options = { } );

    void place_diverted_trie_string_on_coordinates( picture& p_pic, const placed_trie& p_trie,
                                                    const std::string& p_string,
                                                    const std::string& p_alphabet,
                                                    double             p_charDiversion
                                                    = DEFAULT_TRIE_CHAR_DIVERTION,
                                                    const kv_store& p_options = { } );

    void place_trie_depth_labels( picture& p_pic, const placed_trie& p_trie,
                                  tikz_point      p_labelTopLeft = { .0, CHAR_HEIGHT },
                                  const kv_store& p_options
                                  = OPT::INNER_SEP( "0pt" )
                                    | OPT::TEXT_COLOR( COLOR_TEXT.deemphasize( ) ) );

    void place_trie_depth_labels( picture& p_pic, const placed_trie& p_trie, u64 p_max,
                                  tikz_point      p_labelTopLeft = { .0, CHAR_HEIGHT },
                                  const kv_store& p_options
                                  = OPT::INNER_SEP( "0pt" )
                                    | OPT::TEXT_COLOR( COLOR_TEXT.deemphasize( ) ) );

    placed_trie place_trie( picture& p_pic, const trie& p_trie, tikz_point p_topLeft = { .0, .0 },
                            const std::string& p_name    = EMPTY_STR,
                            double             p_distX   = DEFAULT_TRIE_VERTEX_DIST_X,
                            double             p_distY   = DEFAULT_TRIE_VERTEX_DIST_Y,
                            const kv_store&    p_options = { } );

    placed_trie place_trie_wide( picture& p_pic, const trie& p_trie, const std::string& p_alphabet,
                                 tikz_point         p_topLeft       = { .0, .0 },
                                 const std::string& p_name          = EMPTY_STR,
                                 double             p_distX         = DEFAULT_TRIE_VERTEX_DIST_X,
                                 double             p_distY         = DEFAULT_TRIE_VERTEX_DIST_Y,
                                 double             p_charDiversion = DEFAULT_TRIE_CHAR_DIVERTION,
                                 const kv_store&    p_options       = { } );

} // namespace TIKZ
