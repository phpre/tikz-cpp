#pragma once
#include <deque>
#include <map>
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

        inline void insert( const std::string& p_string ) {
            u64  i = 0, j = 0;
            bool addition = false;

            for( ; i < p_string.length( ); ++i ) {
                while( m_vertices.size( ) <= i + 1 ) {
                    m_vertices.push_back( std::deque<trie_vertex>{ } );
                }
                auto& vtx = m_vertices[ i ][ j ];

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
                    vtx.m_size++;
                    j = vtx.m_next[ p_string[ i ] ];
                }
            }
        }
    };
} // namespace ALG
