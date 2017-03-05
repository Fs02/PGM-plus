#include <cassert>
#include <string>
#include <pgm/pgm.h>

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
    // http://www.cs.ubc.ca/~murphyk/Bayes/bnintro.html
    pgm::Bayesnet bn;
    assert(bn.add_node("cloudy", {"F", "T"}));
    assert(bn.add_node("sprinkler", {"F", "T"}));
    assert(bn.add_node("rain", {"F", "T"}));
    assert(bn.add_node("wetgrass", {"F", "T"}));

    assert(bn.add_arc("cloudy", "sprinkler"));
    assert(bn.add_arc("cloudy", "rain"));
    assert(bn.add_arc("sprinkler", "wetgrass"));
    assert(bn.add_arc("rain", "wetgrass"));

    // assigns cpt
    assert(bn.probability("cloudy", "F", {}, 0.5));
    assert(bn.probability("cloudy", "T", {}, 0.5));

    assert(bn.probability("sprinkler", "F", {{"cloudy", "F"}}, 0.5));
    assert(bn.probability("sprinkler", "T", {{"cloudy", "F"}}, 0.5));
    assert(bn.probability("sprinkler", "F", {{"cloudy", "T"}}, 0.9));
    assert(bn.probability("sprinkler", "T", {{"cloudy", "T"}}, 0.1));

    assert(bn.probability("rain", "F", {{"cloudy", "F"}}, 0.8));
    assert(bn.probability("rain", "T", {{"cloudy", "F"}}, 0.2));
    assert(bn.probability("rain", "F", {{"cloudy", "T"}}, 0.2));
    assert(bn.probability("rain", "T", {{"cloudy", "T"}}, 0.8));

    assert(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "F"}}, 1.0));
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "F"}}, 0.0));
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "F"}}, 0.1));
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "F"}}, 0.9));
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "T"}}, 0.1));
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "T"}}, 0.9));
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "T"}}, 0.01));
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "T"}}, 0.99));

    // check assigned cpt
    assert(bn.probability("cloudy", "F", {}) == 0.5);
    assert(bn.probability("cloudy", "T", {}) == 0.5);

    assert(bn.probability("sprinkler", "F", {{"cloudy", "F"}}) == 0.5);
    assert(bn.probability("sprinkler", "T", {{"cloudy", "F"}}) == 0.5);
    assert(bn.probability("sprinkler", "F", {{"cloudy", "T"}}) == 0.9);
    assert(bn.probability("sprinkler", "T", {{"cloudy", "T"}}) == 0.1);

    assert(bn.probability("rain", "F", {{"cloudy", "F"}}) == 0.8);
    assert(bn.probability("rain", "T", {{"cloudy", "F"}}) == 0.2);
    assert(bn.probability("rain", "F", {{"cloudy", "T"}}) == 0.2);
    assert(bn.probability("rain", "T", {{"cloudy", "T"}}) == 0.8);

    assert(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "F"}}) == 1.0);
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "F"}}) == 0.0);
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "F"}}) == 0.1);
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "F"}}) == 0.9);
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "T"}}) == 0.1);
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "T"}}) == 0.9);
    assert(bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "T"}}) == 0.01);
    assert(bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "T"}}) == 0.99);
}

int main()
{
    test_variable();
    test_dataset();
    test_dgraph();
    test_bayesnet();
    return 0;
}