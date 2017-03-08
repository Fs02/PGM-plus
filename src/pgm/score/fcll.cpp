#include <pgm/score/fcll.h>
#include <cmath>
#include <limits>

using namespace pgm;

Fcll::Fcll(const Dataset &dataset, const std::string &class_node)
    : LogLikelihood(dataset), class_node_(class_node)
{}

double Fcll::score(const std::string &child, const std::vector<std::string> &parents)
{
    // declare some const numbers
    constexpr double pi = 3.14159265358979323846;
    const double alpha = (std::pow(pi, 2) + 6.0)/24.0;
    const double beta = (std::pow(pi, 2) - 18.0)/24.0;
    const double lambda = std::pow(pi, 2)/6.0;
    const double epsilon = std::numeric_limits<double>::epsilon();

    const auto var_child = dataset_.variable(child);
    const auto var_class = dataset_.variable(class_node_);
    std::vector<std::string> parents_child_class = parents;
    parents_child_class.push_back(child);
    parents_child_class.push_back(class_node_);

    Frequency count(dataset_, parents_child_class);
    const auto pa_i = count.permutate(parents);

    // compute second term    
    double fcllcriterion = 0.0; // 2nd decomposable term of fcll
    const std::size_t q_i = pa_i.size();
    const std::size_t r_i = var_child.cardinality();
    const std::size_t s_i = var_class.cardinality();

    for (std::size_t j = 0; j < q_i; ++j)
    {
        // n_ij
        std::unordered_map<std::string, std::string> vars;
        for (std::size_t p = 0; p < pa_i[j].size(); ++p)
            vars[parents[p]] = pa_i[j][p];

        const double n_ij = std::max(double(count(vars)), epsilon);

        for (std::size_t k = 0; k < r_i; ++k)
        {
            // n_ijk
            const std::string child_state = var_child(k);
            vars[child] = child_state;
            const double n_ijk = std::max(double(count(vars)), epsilon);
    
            for (std::size_t c = 0; c < s_i; ++c)
            {
                // n_ijck
                const std::string class_state = var_class(c);
                vars[class_node_] = class_state;
                const double n_ijck = std::max(double(count(vars)), epsilon);

                // n_ijc
                vars.erase(child);
                const double n_ijc = std::max(double(count(vars)), epsilon);

                fcllcriterion += n_ijck * (std::log(n_ijck/n_ijk)-std::log(n_ij/n_ij));
            }
        }
    }

    // make value negative to allow the same behaviours as other scoring function
    return -((alpha + beta) * LogLikelihood::score(count, child, parents) - beta * lambda * fcllcriterion);
}
