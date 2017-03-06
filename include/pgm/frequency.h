#ifndef PGM_FREQUENCY_H
#define PGM_FREQUENCY_H

#include <string>
#include <vector>
#include <unordered_map>

namespace pgm {

class Dataset;

class Frequency
{
public:
    Frequency(const Dataset &dataset, const std::vector<std::string> variables);

    std::size_t operator ()(const std::unordered_map<std::string, std::size_t> &vars) const;

    std::vector<std::vector<std::size_t>> permutate(const std::vector<std::string> variables);

    inline std::size_t arity(const std::string &variable) const { return arity_[index_.at(variable)]; }
private:
    std::unordered_map<std::string, std::size_t> index_;
    std::vector<std::string> variables_;
    std::vector<std::size_t> arity_;
    std::vector<std::size_t> count_;
};

}

#endif