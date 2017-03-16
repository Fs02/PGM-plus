#include <catch.hpp>
#include <pgm/dgraph.h>

TEST_CASE( "dgraph" ) {

    // prepare directed graph
    pgm::DGraph graph(true);
    auto v1 = graph.add_vertex();
    auto v2 = graph.add_vertex();
    auto v3 = graph.add_vertex();

    REQUIRE(graph.vertices().size() == 3);
    REQUIRE(graph.add_adjacent(v1, v2));
    REQUIRE(graph.add_adjacent(v2, v3));
    REQUIRE(graph.adjacents(v1).size() == 1);
    REQUIRE(graph.adjacents(v2).size() == 1);
    REQUIRE(graph.adjacents(v3).size() == 0);

    SECTION( "make sure all vertices are valid") {
        REQUIRE(graph.valid_vertex(v1));
        REQUIRE(graph.valid_vertex(v2));
        REQUIRE(graph.valid_vertex(v3));
    }

    SECTION( "adding invalid adjacets" ) {
        REQUIRE(!graph.add_adjacent(v3, v1));
        REQUIRE(!graph.add_adjacent(v2, v1));
        REQUIRE(!graph.add_adjacent(v3, v2));
        REQUIRE(graph.adjacents(v1).size() == 1);
        REQUIRE(graph.adjacents(v2).size() == 1);
        REQUIRE(graph.adjacents(v3).size() == 0);
    }

    SECTION( "reversing adjacents" ) {
        REQUIRE(graph.rev_adjacent(v1, v2));
        REQUIRE(graph.adjacents(v1).size() == 0);
        REQUIRE(graph.adjacents(v2).size() == 2);
        REQUIRE(graph.adjacents(v3).size() == 0);
    }

    SECTION( "removing adjacents" ) {
        REQUIRE(graph.rem_adjacent(v1, v2));
        REQUIRE(graph.adjacents(v1).size() == 0);
        REQUIRE(graph.adjacents(v2).size() == 1);
        REQUIRE(graph.adjacents(v3).size() == 0);
    }

    SECTION( "removing vertex" ) {
        REQUIRE(graph.rem_vertex(v3));
        REQUIRE(!graph.valid_vertex(v3));
        REQUIRE(graph.vertices().size() == 2);
        REQUIRE(graph.adjacents(v1).size() == 1);
        REQUIRE(graph.adjacents(v2).size() == 0);
    }
}