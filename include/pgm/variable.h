#ifndef PGM_VARIABLE_H
#define PGM_VARIABLE_H

#include <vector>
#include <string>
#include <iostream>

namespace pgm {

class Variable
{
public:
    explicit Variable(const std::string &name);
    Variable(const std::string &name, const std::vector<std::string> &states);

    std::size_t operator() (const std::string &state);
    std::size_t operator() (const std::string &state) const;

    std::string operator() (std::size_t id);
    std::string operator() (std::size_t id) const;

    inline std::string name() const { return name_; }
    inline const std::vector<std::string> &states() const { return states_; }
    inline std::vector<std::string> states() { return states_; }
    inline std::size_t cardinality() const { return states_.size(); }

    friend std::ostream &operator <<(std::ostream &os, const Variable &v);

private:
    std::string name_;
    std::vector<std::string> states_;
};

std::ostream &operator <<(std::ostream &os, const Variable &v);

}

#endif