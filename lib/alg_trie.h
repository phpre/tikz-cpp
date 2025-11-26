#pragma once
#include <deque>
#include <map>
#include <set>
#include <string>

#include "defines.h"

namespace ALG {
    struct trie_vertex {
        std::map<char, u64> m_next = { }; // map char -> position in next depth's deque

        bool m_marked = false; // string ends at current vertex
        u64  m_size   = 0;     // number of leaves in subtree rooted at this vertex
    };

    struct trie {
        // for each depth, store a deque of all vertices at this depth
        std::deque<std::deque<trie_vertex>> m_vertices{ { trie_vertex{} } };

        inline std::map<std::pair<u64, u64>, std::string> names( const std::string& p_namePrefix
                                                                 = EMPTY_STR ) const {
            u64             i = 0;
            std::deque<u64> cur{ 0 };

            std::map<std::pair<u64, u64>, std::string> names{ { { 0, 0 }, p_namePrefix + "-" } };

            for( ; i < m_vertices.size( ); ++i ) {
                std::deque<u64> next{ };
                for( u64 j : cur ) {
                    const auto& vtx = m_vertices[ i ][ j ];
                    for( const auto& [ c, t ] : vtx.m_next ) {
                        next.push_back( t );
                        names[ { i + 1, t } ] = names[ { i, j } ] + c;
                    }
                }
                cur = next;
            }
            return names;
        }

        inline std::map<std::pair<u64, u64>, u64> heights( ) const {
            u64             i = 0;
            std::deque<u64> cur{ 0 };

            std::map<std::pair<u64, u64>, u64> heights{ { { 0, 0 }, 0 } };

            for( ; i < m_vertices.size( ); ++i ) {
                std::deque<u64> next{ };
                for( u64 j : cur ) {
                    const auto& vtx  = m_vertices[ i ][ j ];
                    u64         hcur = 0;
                    for( const auto& [ c, t ] : vtx.m_next ) {
                        next.push_back( t );
                        heights[ { i + 1, t } ] = heights[ { i, j } ] + hcur;
                        hcur += m_vertices[ i + 1 ][ t ].m_size;
                    }
                }
                cur = next;
            }
            return heights;
        }

        inline void insert( const std::string& p_string ) {
            u64  i = 0, j = 0;
            bool addition = false;
            u64  lm       = 0;

            for( ; i < p_string.length( ); ++i ) {
                while( m_vertices.size( ) <= i + 1 ) {
                    m_vertices.push_back( std::deque<trie_vertex>{ } );
                }
                auto& vtx = m_vertices[ i ][ j ];
                if( vtx.m_marked ) { lm = i; }

                if( vtx.m_next.count( p_string[ i ] ) ) {
                    j = vtx.m_next[ p_string[ i ] ];
                } else {
                    addition = true;
                    j = vtx.m_next[ p_string[ i ] ] = m_vertices[ i + 1 ].size( );
                    m_vertices[ i + 1 ].push_back( trie_vertex{ } );
                }
            }
            m_vertices[ i ][ j ].m_marked = true;

            // update sizes
            if( addition ) {
                i = 0, j = 0;
                for( ; i < p_string.length( ); ++i ) {
                    auto& vtx = m_vertices[ i ][ j ];
                    if( i >= lm ) { vtx.m_size++; }
                    j = vtx.m_next[ p_string[ i ] ];
                }
                auto& vtx = m_vertices[ i ][ j ];
                if( i >= lm ) { vtx.m_size++; }
            }
        }
    };
} // namespace ALG
