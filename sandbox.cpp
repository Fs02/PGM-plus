#include <cassert>
#include <string>
#include <pgm/pgm.h>
#include <cmath>

// compare with 5 digits precision
template<typename T>
bool equal(T f1, T f2) { 
  return (std::abs(f1 - f2) <= 0.000001);
}

void test_variable()
{
    // with unspecified states
    pgm::Variable weather("weather");
    assert(weather("rainy") == 0);
    assert(weather("cloudy") == 1);
    assert(weather("sunny") == 2);
    assert(weather(0) == "rainy");
    assert(weather(1) == "cloudy");
    assert(weather(2) == "sunny");

    // with specified states
    pgm::Variable student("student", {"yes", "no"});
    assert(student("yes") == 0);
    assert(student("no") == 1);
    assert(student(0) == "yes");
    assert(student(1) == "no");
}

void test_dataset()
{
    pgm::Dataset dataset;
    dataset.push({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "yes"}}); 
    dataset.push({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "no"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "yes"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "yes"}});
    dataset.push({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "no"}});

    int a = dataset.size();
    assert(dataset.size() == 5);
    assert(dataset.names().size() == 3);
    assert(dataset.variables().size() == 3);
    assert(dataset.get("outlook", 0) == "sunny");
    assert(dataset.get("play", 4) == "no");
    assert(dataset.raw("temperature", 3) == 2);
}

void test_frequency()
{
    pgm::Dataset dataset;
    dataset.push({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "yes"}}); 
    dataset.push({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "no"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "yes"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "yes"}});
    dataset.push({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "no"}});

    {
        auto outlook = dataset.variable("outlook");
        pgm::Frequency count(dataset, {"outlook"});

        assert(count({}) == 5);
        assert(count({{"outlook", outlook("sunny")}}) == 3);
        assert(count({{"outlook", outlook("rain")}}) == 2);
    }

    {
        auto outlook = dataset.variable("outlook");
        auto temperature = dataset.variable("temperature");
        pgm::Frequency count(dataset, {"outlook", "temperature"});

        assert(count({}) == 5);
        assert(count({{"outlook", outlook("sunny")}}) == 3);
        assert(count({{"outlook", outlook("rain")}}) == 2);
        assert(count({{"temperature", temperature("hot")}}) == 2);
        assert(count({{"temperature", temperature("cool")}}) == 2);
        assert(count({{"temperature", temperature("mild")}}) == 1);

        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("hot")}}) == 1);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("cool")}}) == 1);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("mild")}}) == 1);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("hot")}}) == 1);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("cool")}}) == 1);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("mild")}}) == 0);
    }

    {
        auto outlook = dataset.variable("outlook");
        auto temperature = dataset.variable("temperature");
        auto play = dataset.variable("play");
        pgm::Frequency count(dataset, {"outlook", "temperature", "play"});

        assert(count({}) == 5);
        assert(count({{"outlook", outlook("sunny")}}) == 3);
        assert(count({{"outlook", outlook("rain")}}) == 2);
        assert(count({{"temperature", temperature("hot")}}) == 2);
        assert(count({{"temperature", temperature("cool")}}) == 2);
        assert(count({{"temperature", temperature("mild")}}) == 1);
        assert(count({{"play", play("yes")}}) == 3);
        assert(count({{"play", play("no")}}) == 2);

        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("hot")}}) == 1);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("cool")}}) == 1);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("mild")}}) == 1);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("hot")}}) == 1);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("cool")}}) == 1);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("mild")}}) == 0);

        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("hot")}, {"play", play("yes")}}) == 1);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("hot")}, {"play", play("no")}}) == 0);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("cool")}, {"play", play("yes")}}) == 1);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("cool")}, {"play", play("no")}}) == 0);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("mild")}, {"play", play("yes")}}) == 1);
        assert(count({{"outlook", outlook("sunny")}, {"temperature", temperature("mild")}, {"play", play("no")}}) == 0);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("hot")}, {"play", play("yes")}}) == 0);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("hot")}, {"play", play("no")}}) == 1);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("cool")}, {"play", play("yes")}}) == 0);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("cool")}, {"play", play("no")}}) == 1);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("mild")}, {"play", play("yes")}}) == 0);
        assert(count({{"outlook", outlook("rain")}, {"temperature", temperature("mild")}, {"play", play("no")}}) == 0);
    }
}

void test_dgraph()
{
    pgm::DGraph graph(true);
    auto v1 = graph.add_vertex();
    auto v2 = graph.add_vertex();
    auto v3 = graph.add_vertex();

    assert(graph.valid_vertex(v1));
    assert(graph.valid_vertex(v2));
    assert(graph.valid_vertex(v3));

    assert(graph.add_adjacent(v1, v2));
    assert(graph.add_adjacent(v2, v3));
    assert(!graph.add_adjacent(v3, v1));
    assert(graph.adjacents(v1).size() == 1);
    assert(graph.adjacents(v2).size() == 1);
    assert(graph.adjacents(v3).size() == 0);

    assert(graph.rev_adjacent(v1, v2));
    assert(graph.adjacents(v1).size() == 0);
    assert(graph.adjacents(v2).size() == 2);
    assert(graph.adjacents(v3).size() == 0);

    assert(graph.rem_vertex(v3));
    assert(!graph.valid_vertex(v3));
    assert(graph.adjacents(v1).size() == 0);
    assert(graph.adjacents(v2).size() == 1);
}

void test_bayesnet()
{
    // sanity test
    // taken from my course assignment
    pgm::Bayesnet bn;
    assert(bn.add_node("winter", {"F", "T"}));
    assert(bn.add_node("sprinkler", {"F", "T"}));
    assert(bn.add_node("rain", {"F", "T"}));
    assert(bn.add_node("wetgrass", {"F", "T"}));
    assert(bn.add_node("slippery", {"F", "T"}));

    assert(bn.add_arc("winter", "sprinkler"));
    assert(bn.add_arc("winter", "rain"));
    assert(bn.add_arc("sprinkler", "wetgrass"));
    assert(bn.add_arc("rain", "wetgrass"));
    assert(bn.add_arc("rain", "slippery"));

    // assigns cpt
    assert(bn.probability("winter", "T", {}, 0.6));
    assert(bn.probability("winter", "F", {}, 0.4));

    assert(bn.probability("sprinkler", "T", {{"winter", "T"}}, 0.2));
    assert(bn.probability("sprinkler", "F", {{"winter", "T"}}, 0.8));
    assert(bn.probability("sprinkler", "T", {{"winter", "F"}}, 0.75));
    assert(bn.probability("sprinkler", "F", {{"winter", "F"}}, 0.25));

    assert(bn.probability("rain", "T", {{"winter", "T"}}, 0.8));
    assert(bn.probability("rain", "F", {{"winter", "T"}}, 0.2));
    assert(bn.probability("rain", "T", {{"winter", "F"}}, 0.1));
    assert(bn.probability("rain", "F", {{"winter", "F"}}, 0.9));

    assert(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "T"}}, 0.95));
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "T"}}, 0.05));
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "F"}}, 0.9));
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "F"}}, 0.1));
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "T"}}, 0.8));
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "T"}}, 0.2));
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "F"}}, 0.0));
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "F"}}, 1.0));

    assert(bn.probability("slippery", "T", {{"rain", "T"}}, 0.7));
    assert(bn.probability("slippery", "F", {{"rain", "T"}}, 0.3));
    assert(bn.probability("slippery", "T", {{"rain", "F"}}, 0.0));
    assert(bn.probability("slippery", "F", {{"rain", "F"}}, 1.0));

    // check assigned cpt
    assert(bn.probability("winter", "T", {}) == 0.6);
    assert(bn.probability("winter", "F", {}) == 0.4);

    assert(bn.probability("sprinkler", "T", {{"winter", "T"}}) == 0.2);
    assert(bn.probability("sprinkler", "F", {{"winter", "T"}}) == 0.8);
    assert(bn.probability("sprinkler", "T", {{"winter", "F"}}) == 0.75);
    assert(bn.probability("sprinkler", "F", {{"winter", "F"}}) == 0.25);

    assert(bn.probability("rain", "T", {{"winter", "T"}}) == 0.8);
    assert(bn.probability("rain", "F", {{"winter", "T"}}) == 0.2);
    assert(bn.probability("rain", "T", {{"winter", "F"}}) == 0.1);
    assert(bn.probability("rain", "F", {{"winter", "F"}}) == 0.9);

    assert(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "T"}}) == 0.95);
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "T"}}) == 0.05);
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "F"}}) == 0.9);
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "F"}}) == 0.1);
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "T"}}) == 0.8);
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "T"}}) == 0.2);
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "F"}}) == 0.0);
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "F"}}) == 1.0);

    assert(bn.probability("slippery", "T", {{"rain", "T"}}) == 0.7);
    assert(bn.probability("slippery", "F", {{"rain", "T"}}) == 0.3);
    assert(bn.probability("slippery", "T", {{"rain", "F"}}) == 0.0);
    assert(bn.probability("slippery", "F", {{"rain", "F"}}) == 1.0);

    // queries
    // compared with http://aispace.org/bayes/
    assert(equal(bn.query({{"winter", "T"}}), 0.6));
    assert(equal(bn.query({{"sprinkler", "T"}}), 0.42));
    assert(equal(bn.query({{"rain", "T"}}), 0.52));
    assert(equal(bn.query({{"wetgrass", "T"}}), 0.6995));
    assert(equal(bn.query({{"slippery", "T"}}), 0.364));
    assert(equal(bn.query({{"winter", "T"}, {"sprinkler", "T"}, {"rain", "F"}, {"wetgrass", "T"}, {"slippery", "F"}}), 0.0216));
    assert(equal(bn.query({{"winter", "T"}, {"sprinkler", "T"}, {"rain", "F"}}), 0.024));
    assert(equal(bn.query({{"rain", "F"}}), 0.48));
    assert(equal(bn.query({{"rain", "F"}, {"wetgrass", "T"}, {"slippery", "F"}}), 0.2646));

    // infer
    assert(bn.infer("winter", {}) == "T");
    assert(bn.infer("sprinkler", {}) == "F");
    assert(bn.infer("rain", {}) == "T");
    assert(bn.infer("wetgrass", {}) == "T");
    assert(bn.infer("slippery", {}) == "F");
}

int main()
{
    test_variable();
    test_dataset();
    test_frequency();
    test_dgraph();
    test_bayesnet();
    return 0;
}