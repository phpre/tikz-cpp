#include "tikz.h"
using namespace TIKZ;
std::string OUTDIR_DEFAULT  = "../figs/";
std::string TEXDIR_DEFAULT  = "../tex/";
std::string FONT_FILENAME   = "font_talk";
std::string COLOR_FILENAME  = "color_talk";
std::string MACROS_FILENAME = "macros";

// ---------------------------------------------------------------------------------------------
//
//      globals
//
// ---------------------------------------------------------------------------------------------

const color HIGHLIGHT_FG = COLOR_C3.to_flavor_bg( );
// const color HIGHLIGHT_BG = COLOR_C3;

// ---------------------------------------------------------------------------------------------
//
//      non-default pictures
//
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
//
//      files with (multiple) pictures; one file per frame
//
// ---------------------------------------------------------------------------------------------

FILE_SIMPLE( p_f_wapm_kt_dist_split_2, {
    constexpr std::string T      = "AAAcdaAAAaac";
    constexpr std::string P      = "AbAAbcdAAAd";
    const breakpoint_repn BP_P_T = compute_breakpoints( P, T );

    const stylized_string P_NAME2{ std::string{ "P" }, fragmentco{ 0, P.size( ) } };
    const stylized_string T_NAME2{ std::string{ "T" }, fragmentco{ 0, T.size( ) } };

    auto res = compute_occs_with_edits( P, T, 6 );
    auto bp  = res.back( );
    WITH_PICTURE( pic, { OPT::YSCALE( ".93" ) }, doc ) {
        auto fragP = fragmentco{ 0, P.size( ) };
        auto fragT = align_fragment( BP_P_T, fragP );
        place_alignment_graph_label(
            pic, stylized_string{ P_NAME2.m_name, fragP, str_displ_t ::POSITION },
            stylized_string{ T_NAME2.m_name, fragT, str_displ_t ::POSITION } );

        auto vg = place_alignment_graph( pic, P, fragP, T, fragT );

        for( u64 i = fragT.closed_begin( ); i <= fragT.open_end( ); ++i ) {
            pic.place_node( vg.point_for_pos( i, fragP.closed_begin( ) ) + tikz_point{ 0, .14 },
                            textsize_footnotesize( std::to_string( i ) ) );
            pic.place_node( vg.point_for_pos( i, fragP.open_end( ) ) + tikz_point{ 0, -.14 },
                            textsize_footnotesize( std::to_string( i ) ) );

            place_vertex( pic, vg.point_for_pos( i, 5 ), vertex::marked_vertex( SUB_COL ) );
        }

        place_alignment_on_coordinates( pic, vg, bp, true, true );
    }
    WITH_PICTURE( pic, { OPT::YSCALE( ".93" ) }, doc ) {
        auto fragP = fragmentco{ 0, P.size( ) };
        auto fragT = align_fragment( BP_P_T, fragP );
        place_alignment_graph_label(
            pic, stylized_string{ P_NAME2.m_name, fragP, str_displ_t ::POSITION },
            stylized_string{ T_NAME2.m_name, fragT, str_displ_t ::POSITION } );

        auto vg = place_alignment_graph( pic, P, fragP, T, fragT,
                                         tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT },
                                         tikz_point{ 0.0, 0.0 }, COLOR_BLACK.deemphasize( ), 6 );

        for( u64 i = fragT.closed_begin( ); i <= fragT.open_end( ); ++i ) {
            pic.place_node( vg.point_for_pos( i, fragP.closed_begin( ) ) + tikz_point{ 0, .14 },
                            textsize_footnotesize( std::to_string( i ) ) );
            pic.place_node( vg.point_for_pos( i, fragP.open_end( ) ) + tikz_point{ 0, -.14 },
                            textsize_footnotesize( std::to_string( i ) ) );

            place_vertex( pic, vg.point_for_pos( i, 5 ), vertex::marked_vertex( SUB_COL ) );
        }

        place_alignment_on_coordinates( pic, vg, bp, true, true );
    }
} )

FILE_SIMPLE( p_f_wapm_kt_dist_split, {
    constexpr std::string T      = "AAAcdaAAAaac";
    constexpr std::string P      = "AbAAbcdAAAd";
    const breakpoint_repn BP_P_T = compute_breakpoints( P, T );

    const stylized_string P_NAME2{ std::string{ "P" }, fragmentco{ 0, P.size( ) } };
    const stylized_string T_NAME2{ std::string{ "T" }, fragmentco{ 0, T.size( ) } };

    auto res = compute_occs_with_edits( P, T, 6 );
    auto bp  = res.back( );
    WITH_PICTURE( pic, { OPT::YSCALE( ".93" ) }, doc ) {
        auto fragP = fragmentco{ 0, P.size( ) };
        auto fragT = align_fragment( BP_P_T, fragP );
        place_alignment_graph_label(
            pic, stylized_string{ P_NAME2.m_name, fragP, str_displ_t ::POSITION },
            stylized_string{ T_NAME2.m_name, fragT, str_displ_t ::POSITION } );

        auto vg = place_alignment_graph( pic, P, fragP, T, fragT );

        for( u64 i = fragT.closed_begin( ); i <= fragT.open_end( ); ++i ) {
            pic.place_node( vg.point_for_pos( i, fragP.closed_begin( ) ) + tikz_point{ 0, .14 },
                            textsize_footnotesize( std::to_string( i ) ) );
            pic.place_node( vg.point_for_pos( i, fragP.open_end( ) ) + tikz_point{ 0, -.14 },
                            textsize_footnotesize( std::to_string( i ) ) );
        }

        doc.add_picture( pic );
        place_alignment_on_coordinates( pic, vg, bp, true, true );
    }
    WITH_PICTURE( pic, { OPT::YSCALE( ".93" ) }, doc ) {
        auto fragP = fragmentco{ 0, P.size( ) };
        auto fragT = align_fragment( BP_P_T, fragP );
        place_alignment_graph_label(
            pic, stylized_string{ P_NAME2.m_name, fragP, str_displ_t ::POSITION },
            stylized_string{ T_NAME2.m_name, fragT, str_displ_t ::POSITION } );

        auto vg = place_alignment_graph( pic, P, fragP, T, fragT );

        for( u64 i = fragT.closed_begin( ); i <= fragT.open_end( ); ++i ) {
            pic.place_node( vg.point_for_pos( i, fragP.closed_begin( ) ) + tikz_point{ 0, .14 },
                            textsize_footnotesize( std::to_string( i ) ) );
            pic.place_node( vg.point_for_pos( i, fragP.open_end( ) ) + tikz_point{ 0, -.14 },
                            textsize_footnotesize( std::to_string( i ) ) );

            place_vertex( pic, vg.point_for_pos( i, 5 ), vertex::marked_vertex( SUB_COL ) );
        }

        place_alignment_on_coordinates( pic, vg, bp, true, true );
    }
} )

FILE_SIMPLE( p_f_wapm_kt_bd_dist, {
    constexpr std::string T      = "AAAcdaAAAaac";
    constexpr std::string P      = "AbAAbcdAAAd";
    const breakpoint_repn BP_P_T = compute_breakpoints( P, T );

    const stylized_string P_NAME2{ std::string{ "P" }, fragmentco{ 0, P.size( ) } };
    const stylized_string T_NAME2{ std::string{ "T" }, fragmentco{ 0, T.size( ) } };

    WITH_PICTURE( pic, { OPT::YSCALE( ".93" ) }, doc ) {
        auto fragP = fragmentco{ 0, P.size( ) };
        auto fragT = align_fragment( BP_P_T, fragP );
        place_alignment_graph_label(
            pic, stylized_string{ P_NAME2.m_name, fragP, str_displ_t ::POSITION },
            stylized_string{ T_NAME2.m_name, fragT, str_displ_t ::POSITION } );

        auto vg = place_alignment_graph( pic, P, fragP, T, fragT );
        doc.add_picture( pic );

        auto brpnt = breakpoint_slice( BP_P_T, fragP );
        place_alignment_on_coordinates( pic, vg, brpnt, true, true );
    }
    auto res = compute_occs_with_edits( P, T, 6 );
    for( auto bp : res ) {
        WITH_PICTURE( pic, { OPT::YSCALE( ".93" ) }, doc ) {
            auto fragP = fragmentco{ 0, P.size( ) };
            auto fragT = align_fragment( BP_P_T, fragP );
            place_alignment_graph_label(
                pic, stylized_string{ P_NAME2.m_name, fragP, str_displ_t ::POSITION },
                stylized_string{ T_NAME2.m_name, fragT, str_displ_t ::POSITION } );

            auto vg = place_alignment_graph( pic, P, fragP, T, fragT );
            place_alignment_on_coordinates( pic, vg, bp, true, true );
        }
    }
} )

FILE_SIMPLE( p_f_wapm_kt_ag, {
    add_alignment_graph_explanation_pictures( doc );

    constexpr std::string T      = "AAAcdaAAAaacAAaddAAAAA";
    constexpr std::string P      = "AbAAbcdAAAdAAbddAAaAAA";
    const breakpoint_repn BP_P_T = compute_breakpoints( P, T );

    const stylized_string P_NAME2{ std::string{ "P" }, fragmentco{ 0, P.size( ) } };
    const stylized_string T_NAME2{ std::string{ "T" }, fragmentco{ 0, T.size( ) } };

    WITH_PICTURE( pic, { OPT::YSCALE( ".93" ) }, doc ) {
        auto fragP = fragmentco{ 0, 11 };
        auto fragT = align_fragment( BP_P_T, fragP );
        place_alignment_graph_label(
            pic, stylized_string{ P_NAME2.m_name, fragP, str_displ_t ::POSITION },
            stylized_string{ T_NAME2.m_name, fragT, str_displ_t ::POSITION } );
        doc.add_picture( pic );

        vertex_grid vg{ tikz_point{ 0.0, 0.0 }, tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT } };
        vg.place_vertices( pic, vertex::unselected_vertex( COLOR_BLACK.deemphasize_weak( ) ),
                           fragT.length( ) + 1, fragP.length( ) + 1, fragT.closed_begin( ),
                           fragP.closed_begin( ) );
        doc.add_picture( pic );

        vg = place_alignment_graph( pic, P, fragP, T, fragT );
        doc.add_picture( pic );

        auto brpnt = breakpoint_slice( BP_P_T, fragP );
        place_alignment_on_coordinates( pic, vg, brpnt, true, true );
    }

    WITH_PICTURE( pic, { OPT::YSCALE( ".93" ) }, doc ) {
        auto fragP = fragmentco{ 0, 11 };
        auto fragT = align_fragment( BP_P_T, fragP );
        place_alignment_graph_label(
            pic, stylized_string{ P_NAME2.m_name, fragP, str_displ_t ::POSITION },
            stylized_string{ T_NAME2.m_name, fragT, str_displ_t ::POSITION } );

        auto vg = place_alignment_graph( pic, P, fragP, T, fragT,
                                         tikz_point{ CHAR_WIDTH, -CHAR_HEIGHT },
                                         tikz_point{ 0.0, 0.0 }, COLOR_BLACK.deemphasize( ), 6 );

        auto brpnt = breakpoint_slice( BP_P_T, fragP );
        place_alignment_on_coordinates( pic, vg, brpnt, true, true );
    }
} )

FILE_SIMPLE( p_f_wapm_challenge, {
    int _a;
    (void) _a;
    WITH_PICTURE( pic, { }, doc ) {
        std::string T  = "acabb";
        std::string P  = "cabba";
        auto        bp = compute_breakpoints( P, T, WILDCARD );
        auto        T_NAME
            = stylized_string{ T, "T", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS }
                  .add_wildcards( T, chr_displ_t::SHOW_ID_IF_WILDCARD );
        auto P_NAME
            = stylized_string{ P, "P", str_displ_t::SHOW_CHARACTERS | str_displ_t::SHOW_WILDCARDS };
        place_alignment_graph_label( pic, P_NAME, T_NAME );
        auto vg = place_alignment_graph( pic, P, fragmentco{ 0, P.size( ) }, T,
                                         fragmentco{ 0, T.size( ) } );
        place_alignment_on_coordinates( pic, vg, bp );
        doc.add_picture( pic );
        place_weighted_alignment_on_coordinates( pic, vg, bp );
        doc.add_picture( pic );
        cost_table w{ { { 'a', 0 }, 10 } };
        auto       bp2 = reweight( bp, w );
        auto       bp3 = compute_breakpoints( P, T, w, WILDCARD );
        place_weighted_alignment_on_coordinates( pic, vg, bp2 );
        doc.add_picture( pic );

        MAT_COL = COLOR_C1.deemphasize( );
        DEL_COL = COLOR_C3.deemphasize( );
        INS_COL = COLOR_C4.deemphasize( );
        SUB_COL = COLOR_C5.deemphasize( );
        SEP_COL = COLOR_C2.deemphasize( );
        place_weighted_alignment_on_coordinates( pic, vg, bp2 );
        MAT_COL = COLOR_C1;
        DEL_COL = COLOR_C3;
        INS_COL = COLOR_C4;
        SUB_COL = COLOR_C5;
        SEP_COL = COLOR_C2;
        place_weighted_alignment_on_coordinates( pic, vg, bp3 );
    }
} )

// ---------------------------------------------------------------------------------------------
//
//      main function
//
// ---------------------------------------------------------------------------------------------

MAIN( { } )
