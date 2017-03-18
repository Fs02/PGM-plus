#include <catch.hpp>
#include <pgm/pgm.h>
#include "../test_utility.h"

TEST_CASE( "bic" ) {
    // structure
    pgm::Bayesnet bn;
    REQUIRE(bn.add_node("A", {"F", "T"}));
    REQUIRE(bn.add_node("B", {"F", "T"}));
    REQUIRE(bn.add_node("C", {"F", "T"}));
    REQUIRE(bn.add_node("D", {"F", "T"}));
    REQUIRE(bn.add_node("E", {"F", "T"}));

    REQUIRE(bn.add_arc("A", "B"));
    REQUIRE(bn.add_arc("A", "C"));
    REQUIRE(bn.add_arc("B", "D"));
    REQUIRE(bn.add_arc("C", "D"));
    REQUIRE(bn.add_arc("C", "E"));

    // dataset
    pgm::Dataset dataset;
    for (std::size_t i = 0; i < 20; ++i)
        dataset.push({{"A", "T"}, {"B", "F"}, {"C", "T"}, {"D", "T"}, {"E", "T"}});

    for (std::size_t i = 0; i < 15; ++i)
        dataset.push({{"A", "T"}, {"B", "F"}, {"C", "F"}, {"D", "F"}, {"E", "F"}});

    for (std::size_t i = 0; i < 10; ++i)
        dataset.push({{"A", "F"}, {"B", "T"}, {"C", "F"}, {"D", "T"}, {"E", "T"}});

    for (std::size_t i = 0; i < 15; ++i)
        dataset.push({{"A", "F"}, {"B", "F"}, {"C", "T"}, {"D", "T"}, {"E", "T"}});

    for (std::size_t i = 0; i < 5; ++i)
        dataset.push({{"A", "F"}, {"B", "F"}, {"C", "F"}, {"D", "F"}, {"E", "F"}});

    for (std::size_t i = 0; i < 2; ++i)
        dataset.push({{"A", "T"}, {"B", "T"}, {"C", "F"}, {"D", "T"}, {"E", "F"}});

    SECTION( "score output" ) {
        pgm::Bic score(dataset);
        REQUIRE(equal(score(bn), -162.27019));
        REQUIRE(equal(score.score("A", {}), -48.1769));
        REQUIRE(equal(score.score("B", {"A"}), -31.0806));
        REQUIRE(equal(score.score("C", {"A"}), -50.5238));
        REQUIRE(equal(score.score("D", {"B", "C"}), -8.40939));
        REQUIRE(equal(score.score("E", {"C"}), -24.0795));
    }
}