#include <catch.hpp>
#include <pgm/variable.h>

TEST_CASE( "variable" ) {
    SECTION( "with no default states" ) {
        pgm::Variable weather("weather");
        REQUIRE(weather("rainy") == 0);
        REQUIRE(weather("cloudy") == 1);
        REQUIRE(weather("sunny") == 2);
        REQUIRE(weather(0) == "rainy");
        REQUIRE(weather(1) == "cloudy");
        REQUIRE(weather(2) == "sunny");
    }

    SECTION( "with specified states" ) {
        pgm::Variable student("student", {"yes", "no"});
        REQUIRE(student("yes") == 0);
        REQUIRE(student("no") == 1);
        REQUIRE(student(0) == "yes");
        REQUIRE(student(1) == "no");
    }
}