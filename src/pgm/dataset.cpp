#include <pgm/dataset.h>
#include <cassert>

using namespace pgm;

Dataset::Proxy::Proxy(Dataset *dataset, const std::string &name, std::size_t index)
    : dataset_(dataset), name_(name), index_(index)
{}

void Dataset::Proxy::operator = (const std::string &value)
{
    dataset_->set(name_, index_, value);
}

Dataset::Proxy::operator std::string() const
{
    return dataset_->get(name_, index_);
}

bool Dataset::Proxy::operator ==(const std::string &other)
{
    return dataset_->get(name_, index_) == other;
}

Dataset::Dataset()
    : variables_(), size_(0)
{}

Dataset::Proxy Dataset::operator ()(const std::string &name, std::size_t index)
{
    return Proxy(this, name, index);
}

std::unordered_map<std::string, std::string> Dataset::operator [](std::size_t index)
{
    std::unordered_map<std::string, std::string> result;
    result.reserve(variables_.size());
    for (auto var: variables_)
    {
        auto val = var.second.second.find(index);
        if (val != var.second.second.end())
            result[var.first] = var.second.first(val->second);
        else
            result[var.first] = var.second.first(0);
    }
    return result;
}

std::string Dataset::get(const std::string &name, std::size_t index) const
{
    auto var = variables_.find(name);
    assert(var != variables_.end());

    auto val = var->second.second.find(index);
    if (val != var->second.second.end())
        return var->second.first(val->second);

    return var->second.first(0);
}

void Dataset::set(const std::string &name, std::size_t index, const std::string &value)
{
    auto var = variables_.find(name);
    if (var == variables_.end()) {
        variables_.insert(std::make_pair(name, std::make_pair(Variable(name), values_type())));
        var = variables_.find(name);
    }

    std::size_t id = var->second.first(value);
    var->second.second[index] = id;
    size_ = std::max(size_, index + 1);
}

std::size_t Dataset::raw(const std::string &name, std::size_t index) const
{
    auto var = variables_.find(name);
    assert(var != variables_.end());

    auto val = var->second.second.find(index);
    if (val != var->second.second.end())
        return val->second;

    return 0;
}

void Dataset::push(const std::unordered_map<std::string, std::string> &vars)
{
    std::size_t index = size_;
    for (auto var : vars)
        set(var.first, index, var.second);
}

bool Dataset::add_variable(const Variable &variable)
{
    return add_variable(variable.name(), variable.states());
}

bool Dataset::add_variable(const std::string &name, const std::vector<std::string> &values)
{
    auto var = variables_.find(name);
    if (var == variables_.end()) {
        variables_.insert(std::make_pair(name, std::make_pair(Variable(name, values), values_type())));
        var = variables_.find(name);
        return true;
    }
    return false;
}

bool Dataset::rem_variable(const std::string &name)
{
    return variables_.erase(name) != 0;
}

std::vector<std::string> Dataset::names() const
{
    std::vector<std::string> result;
    for (auto var : variables_)
        result.push_back(var.first);

    return result;
}

std::vector<Variable> Dataset::variables() const
{
    std::vector<Variable> result;
    for (auto var : variables_)
        result.push_back(var.second.first);

    return result;
}