#include <pgm/frequency.h>
#include <pgm/dataset.h>
#include <set>

using namespace pgm;

Frequency::Frequency(const Dataset &dataset, const std::vector<std::string> variables)
{
    // assign variables and stores each arity
    {
        auto variables_set = std::set<std::string>(variables.begin(), variables.end());
        std::size_t index = 0;
        for (auto it = variables_set.begin(); it != variables_set.end(); ++it)
        {
            auto var = dataset.variable(*it);
            variables_.push_back(var.name());
            arity_.push_back(var.arity());
            index_[var.name()] = index;
            ++index;
        }
    }

    // generate permutation
    auto permutation = permutate(variables_);
    count_.resize(permutation.size(), 0);

    for (std::size_t row = 0; row < dataset.size(); ++row)
    {
        // initial value
        std::vector<bool> match;
        match.assign(permutation.size(), true);

        // scan datasets
        for (std::size_t i = 0; i < variables_.size(); ++i)
        {
            std::size_t value = dataset.raw(variables_[i], row);
            for (std::size_t j = 0; j < permutation.size(); ++j)
            {
                if (!match[j]) continue;
                match[j] = permutation[j][i] == value;                
            }
        }

        // count matching permutations
        for (std::size_t i = 0; i < permutation.size(); ++i)
        {
            if (match[i])
                count_[i] += 1;
        }
    }

}

std::size_t Frequency::operator ()(const std::unordered_map<std::string, std::size_t> &vars) const
{
    std::size_t stride = 1;
    std::size_t index = 0;
    for (auto it = variables_.rbegin(); it != variables_.rend(); ++it)
    {
        auto var_it = vars.find(*it);

        // missing? marginalize
        if (var_it == vars.end())
        {
            std::size_t sum = 0;
            auto new_vars = vars;
            for (std::size_t i = 0; i < arity(*it); ++i)
            {
                new_vars[*it] = i;
                sum += operator ()(new_vars);
            }
            return sum;
        }

        // find the index
        index += var_it->second * stride;
        stride *= arity(*it);
    }

    return count_[index];
}

std::vector<std::vector<std::size_t>> Frequency::permutate(const std::vector<std::string> variables)
{
    std::vector<std::vector<std::size_t>> permutation;
    permutation.push_back(std::vector<std::size_t>());
    for (std::size_t i = 0; i < variables.size(); ++i)
    {
        std::size_t arity_size = arity(variables[i]);

        std::size_t size = permutation.size();
        permutation.reserve(size * arity_size);

        // duplicates current permutation
        std::vector<std::vector<std::size_t>> new_perm;
        for (std::size_t k = 0; k < size; ++k)
        {
            for (std::size_t j = 0; j < arity_size; ++j)
            {
                new_perm.push_back(permutation[k]);
            }
        }
        permutation = new_perm;

        // fill
        for (std::size_t k = 0; k < size; ++k)
        {
            for (std::size_t j = 0; j < arity_size; ++j)
            {
                permutation[j + k*arity_size].push_back(j);
            }
        }
    }

    return permutation;
}
