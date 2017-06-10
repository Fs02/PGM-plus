# PGM-plus
A C++ Probabilistic Graphical Model Library

## Features
Scoring Function:
- Akaike Information Criterion (AIC)
- Bayesian Dirichlet equivalent uniform (BDeu)
- Bayesian Information Criterion (BIC)
- Factorized Conditional Log Likelihood (fCLL)
- Log Likelihood

Search Algorithm:
- Greedy Hill Climbing
- Simulated Annealing

Inference:
- Brute force

## Usage
The easiest way to use this library is to include and compile alongside your project.

## Example
```
#include <string>
#include <pgm/pgm.h>
#include <cmath>
#include <iostream>

int main()
{
    // bayesnet
    pgm::Bayesnet bn;
    bn.add_node("A", {"F", "T"});
    bn.add_node("B", {"F", "T"});
    bn.add_node("C", {"F", "T"});
    bn.add_node("D", {"F", "T"});
    bn.add_node("E", {"F", "T"});

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

    // Train with simulated annealing and factorized conditional log likelihood
    pgm::SimulatedAnnealing annealing;
    annealing.verbose(true);
    annealing.init_as_naive_bayes("A");
    pgm::Fcll score(dataset, "A");
    pgm::SampleEstimate estimate;

    annealing(bn, score);
    estimate(bn, dataset);

    std::cout << bn;

    // predict
    std::size_t correct = 0;
    for (std::size_t i = 0; i < dataset.size(); ++i)
    {
        auto row = dataset[i];
        if (row["A"] == bn.infer("A", row))
            ++correct;
    }
    std::cout << "Correct : " << correct << "/" << dataset.size() << "\n";
}
```
