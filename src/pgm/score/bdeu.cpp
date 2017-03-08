#include <pgm/score/bdeu.h>
#include <pgm/frequency.h>
#include <cmath>

using namespace pgm;

BDeu::BDeu(const Dataset &dataset, double aplha)
    : Score(dataset), alpha_(aplha)
{}

double BDeu::score(const std::string &child, const std::vector<std::string> &parents)
{
    auto var_child = dataset_.variable(child);
    std::vector<std::string> parents_child = parents;
    parents_child.push_back(child);

    Frequency count(dataset_, parents_child);
    auto pa_i = count.permutate(parents);

    double node_score = 0.0;
    std::size_t q_i = pa_i.size();
    std::size_t r_i = var_child.cardinality();
    for (std::size_t j = 0; j < q_i; ++j)
    {
        std::unordered_map<std::string, std::string> vars;
        for (std::size_t p = 0; p < pa_i[j].size(); ++p)
            vars[parents[p]] = pa_i[j][p];        

        double n_ij = count(vars);
        node_score +=
            std::lgamma(alpha_/double(q_i)) -
            std::lgamma((alpha_/double(q_i))+n_ij);

        for (auto k = 0; k < r_i; ++k)
        {
            std::string child_state = var_child(k);
            vars[child] = child_state;

            // double n_ijk = std::max(double(count(vars), std::numeric_limits<double>::epsilon());
            double n_ijk = double(count(vars));
            node_score +=
                std::lgamma((alpha_/double(r_i*q_i))+n_ijk) -
                std::lgamma(alpha_/double(r_i*q_i));
        }
    }
    return node_score;
}
