#ifndef PGM_DATASET_H
#define PGM_DATASET_H

#include <pgm/variable.h>
#include <unordered_map>

namespace pgm {

class Variable;

class Dataset
{
    struct Proxy
    {
        Dataset *dataset_;
        const std::string &name_;
        const std::size_t index_;

        Proxy(Dataset *dataset, const std::string &name, std::size_t index);
        void operator = (const std::string &value);
        operator std::string() const;

        bool operator ==(const std::string &other);
    };
    
public:
    Dataset();

    Proxy operator ()(const std::string &name, std::size_t index);
    std::unordered_map<std::string, std::string> operator [](std::size_t index);

    std::string get(const std::string &name, std::size_t index) const;
    void set(const std::string &name, std::size_t index, const std::string &value);

    std::size_t raw(const std::string &name, std::size_t index) const;

    void push(const std::unordered_map<std::string, std::string> vars);

    bool add_variable(const Variable &variable);
    bool add_variable(const std::string &name, const std::vector<std::string> &values);

    bool rem_variable(const std::string &name);

    std::vector<std::string> names() const;
    std::vector<Variable> variables() const;

    inline const Variable &variable(const std::string &name) const { return variables_.at(name).first; }
    inline Variable variable(const std::string &name) { return variables_.at(name).first; }

    inline std::size_t size() const { return size_; }

private:
    typedef std::unordered_map<std::size_t, std::size_t> values_type;
    std::unordered_map<std::string, std::pair<Variable, values_type>> variables_;
    std::size_t size_;
};

}

#endif