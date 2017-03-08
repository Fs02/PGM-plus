#include <pgm/variable.h>
#include <algorithm>
#include <cassert>

using namespace pgm;

Variable::Variable(const std::string &name)
    : name_(name), states_()
{}

Variable::Variable(const std::string &name, const std::vector<std::string> &states)
    : name_(name), states_(states)
{}

std::size_t Variable::operator() (const std::string &state)
{
    auto it = std::find(states_.cbegin(), states_.cend(), state);
    if (it == states_.cend())
    {
        states_.push_back(state);
        return states_.size() - 1;
    }

    return std::distance(states_.cbegin(), it);    
}

std::size_t Variable::operator() (const std::string &state) const
{
    auto it = std::find(states_.cbegin(), states_.cend(), state);
    assert(it != states_.cend());

    return std::distance(states_.cbegin(), it);
}

std::string Variable::operator() (std::size_t id)
{
    return states_[id];
}

std::string Variable::operator() (std::size_t id) const
{
    assert(id < states_.size());
    return states_[id];
}

std::ostream &pgm::operator <<(std::ostream &os, const Variable &v)
{
    os << v.name_ << " {";
    for (std::size_t i = 0; i < v.states_.size(); ++i)
    {
        if (i != 0) os << ",";
        os << v.states_[i];
    }
    os << "}";
    return os;
}
