#include <catch.hpp>
#include <pgm/pgm.h>
#include "../test_utility.h"

TEST_CASE( "bdeu" ) {
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

    SECTION( "equivalent sample size 0.1" ) {
        pgm::Bdeu score(dataset, 0.1);
        double total = score(bn);
        REQUIRE(equal(score(bn), -168.1996));
        REQUIRE(equal(score.score("A", {}), -50.3111));
        REQUIRE(equal(score.score("B", {"A"}), -35.1355));
        REQUIRE(equal(score.score("C", {"A"}), -55.4156));
        REQUIRE(equal(score.score("D", {"B", "C"}), -2.2119));
        REQUIRE(equal(score.score("E", {"C"}), -25.1254));
    }

    SECTION( "equivalent sample size 100" ) {
        pgm::Bdeu score(dataset, 100);
        REQUIRE(equal(score(bn), -201.3609));
        REQUIRE(equal(score.score("A", {}), -46.5506));
        REQUIRE(equal(score.score("B", {"A"}), -39.8258));
        REQUIRE(equal(score.score("C", {"A"}), -46.9044));
        REQUIRE(equal(score.score("D", {"B", "C"}), -29.5329));
        REQUIRE(equal(score.score("E", {"C"}), -38.5469));
    }
}