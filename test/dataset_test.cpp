#include <catch.hpp>
#include <pgm/dataset.h>

TEST_CASE( "dataset" ) {
    pgm::Dataset dataset;
    dataset.push({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "yes"}}); 
    dataset.push({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "no"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "yes"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "yes"}});
    dataset.push({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "no"}});

    REQUIRE(dataset.size() == 5);
    REQUIRE(dataset.names().size() == 3);
    REQUIRE(dataset.variables().size() == 3);

    SECTION( "getting a row" ) {
        auto row_3 = dataset[3];
        REQUIRE(row_3["outlook"] == "sunny");
        REQUIRE(row_3["temperature"] == "mild");
        REQUIRE(row_3["play"] == "yes");
    }

    SECTION( "getting a value" ) {
        REQUIRE(dataset("outlook", 1) == "rain");
        REQUIRE(dataset("outlook", 0) == "sunny");
        REQUIRE(dataset("play", 4) == "no");
        REQUIRE(dataset.raw("temperature", 3) == 2);
    }

    SECTION( "updating a value" ) {
        dataset("play", 4) = "yes";
        REQUIRE(dataset("play", 4) == "yes");
    }

    SECTION( "removing a variable" ) {
        REQUIRE(dataset.rem_variable("play"));
        REQUIRE(!dataset.rem_variable("player"));
    }
}