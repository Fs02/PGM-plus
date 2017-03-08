#include <cassert>
#include <string>
#include <pgm/pgm.h>
#include <cmath>
#include <iostream>

// compare with 5 digits precision
template<typename T>
bool equal(T f1, T f2) { 
  return (std::abs(f1 - f2) <= 0.0001);
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
        pgm::Frequency count(dataset, {"outlook"});

        assert(count({}) == 5);
        assert(count({{"outlook", "sunny"}}) == 3);
        assert(count({{"outlook", "rain"}}) == 2);
    }

    {
        pgm::Frequency count(dataset, {"outlook", "temperature"});

        assert(count({}) == 5);
        assert(count({{"outlook", "sunny"}}) == 3);
        assert(count({{"outlook", "rain"}}) == 2);
        assert(count({{"temperature", "hot"}}) == 2);
        assert(count({{"temperature", "cool"}}) == 2);
        assert(count({{"temperature", "mild"}}) == 1);

        assert(count({{"outlook", "sunny"}, {"temperature", "hot"}}) == 1);
        assert(count({{"outlook", "sunny"}, {"temperature", "cool"}}) == 1);
        assert(count({{"outlook", "sunny"}, {"temperature", "mild"}}) == 1);
        assert(count({{"outlook", "rain"}, {"temperature", "hot"}}) == 1);
        assert(count({{"outlook", "rain"}, {"temperature", "cool"}}) == 1);
        assert(count({{"outlook", "rain"}, {"temperature", "mild"}}) == 0);
    }

    {
        pgm::Frequency count(dataset, {"outlook", "temperature", "play"});

        assert(count({}) == 5);
        assert(count({{"outlook", "sunny"}}) == 3);
        assert(count({{"outlook", "rain"}}) == 2);
        assert(count({{"temperature", "hot"}}) == 2);
        assert(count({{"temperature", "cool"}}) == 2);
        assert(count({{"temperature", "mild"}}) == 1);
        assert(count({{"play", "yes"}}) == 3);
        assert(count({{"play", "no"}}) == 2);

        assert(count({{"outlook", "sunny"}, {"temperature", "hot"}}) == 1);
        assert(count({{"outlook", "sunny"}, {"temperature", "cool"}}) == 1);
        assert(count({{"outlook", "sunny"}, {"temperature", "mild"}}) == 1);
        assert(count({{"outlook", "rain"}, {"temperature", "hot"}}) == 1);
        assert(count({{"outlook", "rain"}, {"temperature", "cool"}}) == 1);
        assert(count({{"outlook", "rain"}, {"temperature", "mild"}}) == 0);

        assert(count({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "yes"}}) == 1);
        assert(count({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "no"}}) == 0);
        assert(count({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "yes"}}) == 1);
        assert(count({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "no"}}) == 0);
        assert(count({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "yes"}}) == 1);
        assert(count({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "no"}}) == 0);
        assert(count({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "yes"}}) == 0);
        assert(count({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "no"}}) == 1);
        assert(count({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "yes"}}) == 0);
        assert(count({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "no"}}) == 1);
        assert(count({{"outlook", "rain"}, {"temperature", "mild"}, {"play", "yes"}}) == 0);
        assert(count({{"outlook", "rain"}, {"temperature", "mild"}, {"play", "no"}}) == 0);
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
    assert(!graph.add_adjacent(v2, v1));
    assert(!graph.add_adjacent(v3, v2));
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

void test_sample_estimate()
{
    // structure
    pgm::Bayesnet bn;
    assert(bn.add_node("A", {"F", "T"}));
    assert(bn.add_node("B", {"F", "T"}));
    assert(bn.add_node("C", {"F", "T"}));
    assert(bn.add_node("D", {"F", "T"}));
    assert(bn.add_node("E", {"F", "T"}));

    assert(bn.add_arc("A", "B"));
    assert(bn.add_arc("A", "C"));
    assert(bn.add_arc("B", "D"));
    assert(bn.add_arc("C", "D"));
    assert(bn.add_arc("C", "E"));

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
    assert(equal(bn.probability("A", "T", {}), 0.5522));
    assert(equal(bn.probability("A", "F", {}), 0.4478));

    assert(equal(bn.probability("B", "T", {{"A", "T"}}), 0.0541));
    assert(equal(bn.probability("B", "F", {{"A", "T"}}), 0.9459));
    assert(equal(bn.probability("B", "T", {{"A", "F"}}), 0.3333));
    assert(equal(bn.probability("B", "F", {{"A", "F"}}), 0.6667));

    assert(equal(bn.probability("C", "T", {{"A", "T"}}), 0.5405));
    assert(equal(bn.probability("C", "F", {{"A", "T"}}), 0.4595));
    assert(equal(bn.probability("C", "T", {{"A", "F"}}), 0.5));
    assert(equal(bn.probability("C", "F", {{"A", "F"}}), 0.5));

    assert(std::isnan(bn.probability("D", "T", {{"B", "T"}, {"C", "T"}})));
    assert(std::isnan(bn.probability("D", "F", {{"B", "T"}, {"C", "T"}})));
    assert(equal(bn.probability("D", "T", {{"B", "T"}, {"C", "F"}}), 1.0));
    assert(equal(bn.probability("D", "F", {{"B", "T"}, {"C", "F"}}), 0.0));
    assert(equal(bn.probability("D", "T", {{"B", "F"}, {"C", "T"}}), 1.0));
    assert(equal(bn.probability("D", "F", {{"B", "F"}, {"C", "T"}}), 0.0));
    assert(equal(bn.probability("D", "T", {{"B", "F"}, {"C", "F"}}), 0.0));
    assert(equal(bn.probability("D", "F", {{"B", "F"}, {"C", "F"}}), 1.0));

    assert(equal(bn.probability("E", "T", {{"C", "T"}}), 1.0));
    assert(equal(bn.probability("E", "F", {{"C", "T"}}), 0.0));
    assert(equal(bn.probability("E", "T", {{"C", "F"}}), 0.3125));
    assert(equal(bn.probability("E", "F", {{"C", "F"}}), 0.6875));
}

void test_bdeu()
{
    // structure
    pgm::Bayesnet bn;
    assert(bn.add_node("A", {"F", "T"}));
    assert(bn.add_node("B", {"F", "T"}));
    assert(bn.add_node("C", {"F", "T"}));
    assert(bn.add_node("D", {"F", "T"}));
    assert(bn.add_node("E", {"F", "T"}));

    assert(bn.add_arc("A", "B"));
    assert(bn.add_arc("A", "C"));
    assert(bn.add_arc("B", "D"));
    assert(bn.add_arc("C", "D"));
    assert(bn.add_arc("C", "E"));

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

    // equivalent sample size 0.1
    {
        pgm::BDeu score(dataset, 0.1);
        double total = score(bn);
        double a = score.score("A", {});
        double b = score.score("B", {"A"});
        double c = score.score("C", {"A"});
        double d = score.score("D", {"B", "C"});
        double e = score.score("E", {"C"});
        assert(equal(score(bn), -168.1996));
        assert(equal(score.score("A", {}), -50.3111));
        assert(equal(score.score("B", {"A"}), -35.1355));
        assert(equal(score.score("C", {"A"}), -55.4156));
        assert(equal(score.score("D", {"B", "C"}), -2.2119));
        assert(equal(score.score("E", {"C"}), -25.1254));
    }

    // equivalent sample size 100
    {
        pgm::BDeu score(dataset, 100);
        double total = score(bn);
        double a = score.score("A", {});
        double b = score.score("B", {"A"});
        double c = score.score("C", {"A"});
        double d = score.score("D", {"B", "C"});
        double e = score.score("E", {"C"});
        assert(equal(score(bn), -201.3609));
        assert(equal(score.score("A", {}), -46.5506));
        assert(equal(score.score("B", {"A"}), -39.8258));
        assert(equal(score.score("C", {"A"}), -46.9044));
        assert(equal(score.score("D", {"B", "C"}), -29.5329));
        assert(equal(score.score("E", {"C"}), -38.5469));
    }
}

void test_simulated_annealing()
{
    // bayesnet
    pgm::Bayesnet bn;
    #ifndef NDEBUG
    assert(bn.add_node("A", {"F", "T"}));
    assert(bn.add_node("B", {"F", "T"}));
    assert(bn.add_node("C", {"F", "T"}));
    assert(bn.add_node("D", {"F", "T"}));
    assert(bn.add_node("E", {"F", "T"}));
    #else
    bn.add_node("A", {"F", "T"});
    bn.add_node("B", {"F", "T"});
    bn.add_node("C", {"F", "T"});
    bn.add_node("D", {"F", "T"});
    bn.add_node("E", {"F", "T"});
    #endif

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

    pgm::SimulatedAnnealing annealing;
    annealing.verbose(true);
    pgm::BDeu score(dataset);
    pgm::SampleEstimate estimate;

    annealing(bn, score);
    estimate(bn, dataset);

    std::cout << bn;
}

int main()
{
    test_variable();
    test_dataset();
    test_frequency();
    test_dgraph();
    test_bayesnet();
    test_sample_estimate();
    test_bdeu();
    test_simulated_annealing();
    return 0;
}