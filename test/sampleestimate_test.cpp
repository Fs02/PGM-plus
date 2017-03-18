#include <catch.hpp>
#include <pgm/bayesnet.h>
#include <pgm/dataset.h>
#include <pgm/sampleestimate.h>
#include "test_utility.h"

TEST_CASE( "sample estimate" ) {
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

    pgm::SampleEstimate estimate(0.0);
    estimate(bn, dataset);

    // check estimation
    SECTION( "correct estimation" ) {
        REQUIRE(equal(bn.probability("A", "T", {}), 0.5522));
        REQUIRE(equal(bn.probability("A", "F", {}), 0.4478));

        REQUIRE(equal(bn.probability("B", "T", {{"A", "T"}}), 0.0541));
        REQUIRE(equal(bn.probability("B", "F", {{"A", "T"}}), 0.9459));
        REQUIRE(equal(bn.probability("B", "T", {{"A", "F"}}), 0.3333));
        REQUIRE(equal(bn.probability("B", "F", {{"A", "F"}}), 0.6667));

        REQUIRE(equal(bn.probability("C", "T", {{"A", "T"}}), 0.5405));
        REQUIRE(equal(bn.probability("C", "F", {{"A", "T"}}), 0.4595));
        REQUIRE(equal(bn.probability("C", "T", {{"A", "F"}}), 0.5));
        REQUIRE(equal(bn.probability("C", "F", {{"A", "F"}}), 0.5));

        REQUIRE(std::isnan(bn.probability("D", "T", {{"B", "T"}, {"C", "T"}})));
        REQUIRE(std::isnan(bn.probability("D", "F", {{"B", "T"}, {"C", "T"}})));
        REQUIRE(equal(bn.probability("D", "T", {{"B", "T"}, {"C", "F"}}), 1.0));
        REQUIRE(equal(bn.probability("D", "F", {{"B", "T"}, {"C", "F"}}), 0.0));
        REQUIRE(equal(bn.probability("D", "T", {{"B", "F"}, {"C", "T"}}), 1.0));
        REQUIRE(equal(bn.probability("D", "F", {{"B", "F"}, {"C", "T"}}), 0.0));
        REQUIRE(equal(bn.probability("D", "T", {{"B", "F"}, {"C", "F"}}), 0.0));
        REQUIRE(equal(bn.probability("D", "F", {{"B", "F"}, {"C", "F"}}), 1.0));

        REQUIRE(equal(bn.probability("E", "T", {{"C", "T"}}), 1.0));
        REQUIRE(equal(bn.probability("E", "F", {{"C", "T"}}), 0.0));
        REQUIRE(equal(bn.probability("E", "T", {{"C", "F"}}), 0.3125));
        REQUIRE(equal(bn.probability("E", "F", {{"C", "F"}}), 0.6875));
    }
}