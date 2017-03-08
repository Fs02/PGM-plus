#include <pgm/score/loglikelihood.h>
#include <limits>
#include <cmath>

using namespace pgm;

LogLikelihood::LogLikelihood(const Dataset &dataset)
    : Score(dataset)
{}

double LogLikelihood::score(const std::string &child, const std::vector<std::string> &parents)
{
    std::vector<std::string> parents_child = parents;
    parents_child.push_back(child);

    Frequency count(dataset_, parents_child);
    
    return score(count, child, parents);
}

double LogLikelihood::score(const Frequency &count, const std::string &child, const std::vector<std::string> &parents)
{
    auto var_child = dataset_.variable(child);
    auto pa_i = count.permutate(parents);
    double epsilon = std::numeric_limits<double>::epsilon();

    double node_score = 0.0;
    std::size_t q_i = pa_i.size();
    std::size_t r_i = var_child.cardinality();
    for (std::size_t j = 0; j < q_i; ++j)
    {
        std::unordered_map<std::string, std::string> vars;
        for (std::size_t p = 0; p < pa_i[j].size(); ++p)
            vars[parents[p]] = pa_i[j][p];        

        double n_ij = std::max(double(count(vars)), epsilon);
        for (auto k = 0; k < r_i; ++k)
        {
            std::string child_state = var_child(k);
            vars[child] = child_state;

            double n_ijk = std::max(double(count(vars)), epsilon);
            node_score += n_ijk * std::log(n_ijk/n_ij);
        }
    }
    return node_score;
}