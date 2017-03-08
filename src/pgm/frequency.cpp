#include <pgm/frequency.h>
#include <pgm/dataset.h>
#include <set>

using namespace pgm;

Frequency::Frequency(const Dataset &dataset, const std::vector<std::string> &variables)
{
    // assign variables and stores each cardinality
    std::vector<std::string> variables_unique;
    {
        auto variables_set = std::set<std::string>(variables.begin(), variables.end());
        std::size_t index = 0;
        for (auto it = variables_set.begin(); it != variables_set.end(); ++it)
        {
            auto var = dataset.variable(*it);
            variables_.push_back(var);
            variables_unique.push_back(var.name());
            index_[var.name()] = index;
            ++index;
        }
    }

    // generate permutation
    auto permutation = permutate(variables_unique);
    count_.resize(permutation.size(), 0);

    for (std::size_t row = 0; row < dataset.size(); ++row)
    {
        // initial value
        std::vector<bool> match;
        match.assign(permutation.size(), true);

        // scan datasets
        for (std::size_t i = 0; i < variables_unique.size(); ++i)
        {
            auto value = dataset.get(variables_unique[i], row);
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

std::size_t Frequency::operator ()(const std::unordered_map<std::string, std::string> &vars) const
{
    std::size_t stride = 1;
    std::size_t index = 0;
    for (auto it = variables_.rbegin(); it != variables_.rend(); ++it)
    {
        auto var_it = vars.find(it->name());

        // missing? marginalize
        if (var_it == vars.end())
        {
            std::size_t sum = 0;
            auto new_vars = vars;
            for (std::size_t i = 0; i < it->cardinality(); ++i)
            {
                new_vars[it->name()] = (*it)(i);
                sum += operator ()(new_vars);
            }
            return sum;
        }

        // find the index
        index += (*it)(var_it->second) * stride;
        stride *= it->cardinality();
    }

    return count_[index];
}

std::vector<std::vector<std::string>> Frequency::permutate(const std::vector<std::string> &variables) const
{
    std::vector<std::vector<std::string>> permutation;
    permutation.push_back(std::vector<std::string>());
    for (std::size_t i = 0; i < variables.size(); ++i)
    {
        const auto var = variable(variables[i]);
        const std::size_t cardinality = var.cardinality();

        const std::size_t size = permutation.size();
        permutation.reserve(size * cardinality);

        // duplicates current permutation
        std::vector<std::vector<std::string>> new_perm;
        for (std::size_t k = 0; k < size; ++k)
        {
            for (std::size_t j = 0; j < cardinality; ++j)
            {
                new_perm.push_back(permutation[k]);
            }
        }
        permutation = new_perm;

        // fill
        for (std::size_t k = 0; k < size; ++k)
        {
            for (std::size_t j = 0; j < cardinality; ++j)
            {
                permutation[j + k*cardinality].push_back(var(j));
            }
        }
    }

    return permutation;
}
