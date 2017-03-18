#include <catch.hpp>
#include <pgm/bayesnet.h>
#include "test_utility.h"

TEST_CASE( "bayesnet" ) {
    // sanity test
    pgm::Bayesnet bn;
    REQUIRE(bn.add_node("winter", {"F", "T"}));
    REQUIRE(bn.add_node("sprinkler", {"F", "T"}));
    REQUIRE(bn.add_node("rain", {"F", "T"}));
    REQUIRE(bn.add_node("wetgrass", {"F", "T"}));
    REQUIRE(bn.add_node("slippery", {"F", "T"}));

    REQUIRE(bn.add_arc("winter", "sprinkler"));
    REQUIRE(bn.add_arc("winter", "rain"));
    REQUIRE(bn.add_arc("sprinkler", "wetgrass"));
    REQUIRE(bn.add_arc("rain", "wetgrass"));
    REQUIRE(bn.add_arc("rain", "slippery"));

    // assigns cpt
    REQUIRE(bn.probability("winter", "T", {}, 0.6));
    REQUIRE(bn.probability("winter", "F", {}, 0.4));

    REQUIRE(bn.probability("sprinkler", "T", {{"winter", "T"}}, 0.2));
    REQUIRE(bn.probability("sprinkler", "F", {{"winter", "T"}}, 0.8));
    REQUIRE(bn.probability("sprinkler", "T", {{"winter", "F"}}, 0.75));
    REQUIRE(bn.probability("sprinkler", "F", {{"winter", "F"}}, 0.25));

    REQUIRE(bn.probability("rain", "T", {{"winter", "T"}}, 0.8));
    REQUIRE(bn.probability("rain", "F", {{"winter", "T"}}, 0.2));
    REQUIRE(bn.probability("rain", "T", {{"winter", "F"}}, 0.1));
    REQUIRE(bn.probability("rain", "F", {{"winter", "F"}}, 0.9));

    REQUIRE(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "T"}}, 0.95));
    REQUIRE(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "T"}}, 0.05));
    REQUIRE(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "F"}}, 0.9));
    REQUIRE(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "F"}}, 0.1));
    REQUIRE(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "T"}}, 0.8));
    REQUIRE(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "T"}}, 0.2));
    REQUIRE(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "F"}}, 0.0));
    REQUIRE(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "F"}}, 1.0));

    REQUIRE(bn.probability("slippery", "T", {{"rain", "T"}}, 0.7));
    REQUIRE(bn.probability("slippery", "F", {{"rain", "T"}}, 0.3));
    REQUIRE(bn.probability("slippery", "T", {{"rain", "F"}}, 0.0));
    REQUIRE(bn.probability("slippery", "F", {{"rain", "F"}}, 1.0));

    SECTION ( "assigning correct cpt" ) {
        REQUIRE(bn.probability("winter", "T", {}) == 0.6);
        REQUIRE(bn.probability("winter", "F", {}) == 0.4);

        REQUIRE(bn.probability("sprinkler", "T", {{"winter", "T"}}) == 0.2);
        REQUIRE(bn.probability("sprinkler", "F", {{"winter", "T"}}) == 0.8);
        REQUIRE(bn.probability("sprinkler", "T", {{"winter", "F"}}) == 0.75);
        REQUIRE(bn.probability("sprinkler", "F", {{"winter", "F"}}) == 0.25);

        REQUIRE(bn.probability("rain", "T", {{"winter", "T"}}) == 0.8);
        REQUIRE(bn.probability("rain", "F", {{"winter", "T"}}) == 0.2);
        REQUIRE(bn.probability("rain", "T", {{"winter", "F"}}) == 0.1);
        REQUIRE(bn.probability("rain", "F", {{"winter", "F"}}) == 0.9);

        REQUIRE(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "T"}}) == 0.95);
        REQUIRE(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "T"}}) == 0.05);
        REQUIRE(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "F"}}) == 0.9);
        REQUIRE(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "F"}}) == 0.1);
        REQUIRE(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "T"}}) == 0.8);
        REQUIRE(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "T"}}) == 0.2);
        REQUIRE(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "F"}}) == 0.0);
        REQUIRE(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "F"}}) == 1.0);

        REQUIRE(bn.probability("slippery", "T", {{"rain", "T"}}) == 0.7);
        REQUIRE(bn.probability("slippery", "F", {{"rain", "T"}}) == 0.3);
        REQUIRE(bn.probability("slippery", "T", {{"rain", "F"}}) == 0.0);
        REQUIRE(bn.probability("slippery", "F", {{"rain", "F"}}) == 1.0);
    }

    SECTION( "query" ) {
        REQUIRE(equal(bn.query({{"winter", "T"}}), 0.6));
        REQUIRE(equal(bn.query({{"sprinkler", "T"}}), 0.42));
        REQUIRE(equal(bn.query({{"rain", "T"}}), 0.52));
        REQUIRE(equal(bn.query({{"wetgrass", "T"}}), 0.6995));
        REQUIRE(equal(bn.query({{"slippery", "T"}}), 0.364));
        REQUIRE(equal(bn.query({{"winter", "T"}, {"sprinkler", "T"}, {"rain", "F"}, {"wetgrass", "T"}, {"slippery", "F"}}), 0.0216));
        REQUIRE(equal(bn.query({{"winter", "T"}, {"sprinkler", "T"}, {"rain", "F"}}), 0.024));
        REQUIRE(equal(bn.query({{"rain", "F"}}), 0.48));
        REQUIRE(equal(bn.query({{"rain", "F"}, {"wetgrass", "T"}, {"slippery", "F"}}), 0.2646));
    }

    SECTION ( "inference" ) {
        REQUIRE(bn.infer("winter", {}) == "T");
        REQUIRE(bn.infer("sprinkler", {}) == "F");
        REQUIRE(bn.infer("rain", {}) == "T");
        REQUIRE(bn.infer("wetgrass", {}) == "T");
        REQUIRE(bn.infer("slippery", {}) == "F");
    }
}