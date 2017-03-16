#include <catch.hpp>
#include <pgm/dataset.h>
#include <pgm/frequency.h>

TEST_CASE( "frequency" ) {

    // prepare dataset
    pgm::Dataset dataset;
    dataset.push({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "yes"}}); 
    dataset.push({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "no"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "yes"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "yes"}});
    dataset.push({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "no"}});

    REQUIRE(dataset.size() == 5);

    SECTION( "with one variable" ) {
        pgm::Frequency count(dataset, {"outlook"});

        REQUIRE(count({}) == 5);
        REQUIRE(count({{"outlook", "sunny"}}) == 3);
        REQUIRE(count({{"outlook", "rain"}}) == 2);        
    }

    SECTION( "with two variables" ) {
        pgm::Frequency count(dataset, {"outlook", "temperature"});

        REQUIRE(count({}) == 5);
        REQUIRE(count({{"outlook", "sunny"}}) == 3);
        REQUIRE(count({{"outlook", "rain"}}) == 2);
        REQUIRE(count({{"temperature", "hot"}}) == 2);
        REQUIRE(count({{"temperature", "cool"}}) == 2);
        REQUIRE(count({{"temperature", "mild"}}) == 1);

        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "hot"}}) == 1);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "cool"}}) == 1);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "mild"}}) == 1);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "hot"}}) == 1);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "cool"}}) == 1);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "mild"}}) == 0);
    }

    SECTION( "with two variables" ) {
        pgm::Frequency count(dataset, {"outlook", "temperature", "play"});

        REQUIRE(count({}) == 5);
        REQUIRE(count({{"outlook", "sunny"}}) == 3);
        REQUIRE(count({{"outlook", "rain"}}) == 2);
        REQUIRE(count({{"temperature", "hot"}}) == 2);
        REQUIRE(count({{"temperature", "cool"}}) == 2);
        REQUIRE(count({{"temperature", "mild"}}) == 1);
        REQUIRE(count({{"play", "yes"}}) == 3);
        REQUIRE(count({{"play", "no"}}) == 2);

        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "hot"}}) == 1);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "cool"}}) == 1);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "mild"}}) == 1);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "hot"}}) == 1);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "cool"}}) == 1);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "mild"}}) == 0);

        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "yes"}}) == 1);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "no"}}) == 0);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "yes"}}) == 1);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "no"}}) == 0);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "yes"}}) == 1);
        REQUIRE(count({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "no"}}) == 0);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "yes"}}) == 0);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "no"}}) == 1);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "yes"}}) == 0);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "no"}}) == 1);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "mild"}, {"play", "yes"}}) == 0);
        REQUIRE(count({{"outlook", "rain"}, {"temperature", "mild"}, {"play", "no"}}) == 0);
    }
}