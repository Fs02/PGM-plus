#include <cassert>
#include <string>
#include <pgm/pgm.h>
#include <cmath>
#include <iostream>

void test_simulated_annealing()
{
    std::cout << "\nsimulated annealing\n";

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
    annealing.init_as_naive_bayes("A");
    pgm::Fcll score(dataset, "A");
    pgm::SampleEstimate estimate;

    annealing(bn, score);
    estimate(bn, dataset);

    std::cout << bn;

    std::size_t correct = 0;
    for (std::size_t i = 0; i < dataset.size(); ++i)
    {
        auto row = dataset[i];
        if (row["A"] == bn.infer("A", row))
            ++correct;
    }
    std::cout << "Correct : " << correct << "/" << dataset.size() << "\n";
    
    pgm::write_dot(bn, "test_bn.dot");
}

void test_greedy_hill_climbing()
{
    std::cout << "\ngreedy hill climbing\n";

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

    pgm::GreedyHillClimbing hillclimb;
    hillclimb.verbose(true);
    hillclimb.init_as_naive_bayes("A");
    pgm::Fcll score(dataset, "A");
    pgm::SampleEstimate estimate;

    hillclimb(bn, score);
    estimate(bn, dataset);

    std::cout << bn;

    std::size_t correct = 0;
    for (std::size_t i = 0; i < dataset.size(); ++i)
    {
        auto row = dataset[i];
        if (row["A"] == bn.infer("A", row))
            ++correct;
    }
    std::cout << "Correct : " << correct << "/" << dataset.size() << "\n";
}

int main()
{
    test_simulated_annealing();
    test_greedy_hill_climbing();
    return 0;
}