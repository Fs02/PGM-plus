#include <pgm/score/bdeu.h>
#include <pgm/frequency.h>
#include <cmath>

using namespace pgm;

Bdeu::Bdeu(const Dataset &dataset, double alpha)
    : Score(dataset), alpha_(alpha)
{}

double Bdeu::score(const std::string &child, const std::vector<std::string> &parents)
{
    const auto var_child = dataset_.variable(child);
    std::vector<std::string> parents_child = parents;
    parents_child.push_back(child);

    Frequency count(dataset_, parents_child);
    const auto pa_i = count.permutate(parents);

    double node_score = 0.0;
    const std::size_t q_i = pa_i.size();
    const std::size_t r_i = var_child.cardinality();
    for (std::size_t j = 0; j < q_i; ++j)
    {
        std::unordered_map<std::string, std::string> vars;
        for (std::size_t p = 0; p < pa_i[j].size(); ++p)
            vars[parents[p]] = pa_i[j][p];        

        const double n_ij = count(vars);
        node_score +=
            std::lgamma(alpha_/double(q_i)) -
            std::lgamma((alpha_/double(q_i))+n_ij);

        for (auto k = 0; k < r_i; ++k)
        {
            const std::string child_state = var_child(k);
            vars[child] = child_state;

            const double n_ijk = count(vars);
            node_score +=
                std::lgamma((alpha_/double(r_i*q_i))+n_ijk) -
                std::lgamma(alpha_/double(r_i*q_i));
        }
    }
    return node_score;
}
