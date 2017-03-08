#ifndef PGM_FREQUENCY_H
#define PGM_FREQUENCY_H

#include <unordered_map>
#include <pgm/variable.h>

namespace pgm {

class Dataset;

class Frequency
{
public:
    Frequency(const Dataset &dataset, const std::vector<std::string> &variables);

    std::size_t operator ()(const std::unordered_map<std::string, std::string> &vars) const;

    std::vector<std::vector<std::string>> permutate(const std::vector<std::string> &variables) const;

    inline const Variable &variable(const std::string &name) const { return variables_.at(index_.at(name)); }
    inline Variable variable(const std::string &name) { return variables_.at(index_.at(name)); }

private:
    std::unordered_map<std::string, std::size_t> index_;
    std::vector<Variable> variables_;
    std::vector<std::size_t> count_;
};

}

#endif