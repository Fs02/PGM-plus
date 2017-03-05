#ifndef PGM_DATASET_H
#define PGM_DATASET_H

#include <pgm/variable.h>
#include <unordered_map>

namespace pgm {

class Variable;

class Dataset
{
public:
    Dataset();

    std::string get(const std::string &name, std::size_t index) const;
    void set(const std::string &name, std::size_t index, const std::string &value);

    void push(const std::unordered_map<std::string, std::string> vars);

    std::size_t raw(const std::string &name, std::size_t index) const;

    std::vector<std::string> names() const;
    std::vector<Variable> variables() const;

    inline std::size_t size() const { return size_; }

private:
    typedef std::unordered_map<std::size_t, std::size_t> values_type;
    std::unordered_map<std::string, std::pair<Variable, values_type>> variables_;
    std::size_t size_;
};

}

#endif