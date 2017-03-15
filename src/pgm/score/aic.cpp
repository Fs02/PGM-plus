#include <pgm/score/aic.h>
#include <cmath>

using namespace pgm;

Aic::Aic(const Dataset &dataset)
    : LogLikelihood(dataset)
{}

double Aic::score(const std::string &child, const std::vector<std::string> &parents)
{
    std::vector<std::string> parents_child = parents;
    parents_child.push_back(child);

    Frequency count(dataset_, parents_child);

    const auto var_child = dataset_.variable(child);
    const auto pa_i = count.permutate(parents);

    const std::size_t q_i = pa_i.size();
    const std::size_t r_i = var_child.cardinality();

    return LogLikelihood::score(count, child, parents) 
        - (r_i - 1.0) * q_i;
}