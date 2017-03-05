#include <pgm/bayesnet.h>
#include <pgm/dataset.h>
#include <algorithm>

using namespace pgm;

Bayesnet::Bayesnet()
    : nodes_(), variables_(), graph_(true)
{}

Bayesnet::Bayesnet(const Dataset &dataset)
    : nodes_(), variables_(), graph_()
{
    for (auto variable : dataset.variables())
        add_node(variable);
}

std::string Bayesnet::infer(const std::string &occurence, const variables_map_type &evidence)
{}

double Bayesnet::query(const variables_map_type &evidence)
{}

bool Bayesnet::add_node(const Variable &variable)
{
    return add_node(variable.name(), variable.states());
}

bool Bayesnet::add_node(const std::string &name, const std::vector<std::string> &states)
{
    if (nodes_.find(name) != nodes_.end())
        return false;

    std::size_t id = graph_.add_vertex();
    nodes_.insert({name, id});
    variables_.insert({id, Variable(name, states)});
    probabilities_[id] = std::vector<double>();

    return true;
}

bool Bayesnet::rem_node(const std::string &name)
{
    if (nodes_.find(name) != nodes_.end())
        return false;

    std::size_t id = nodes_[name];
    nodes_.erase(name);
    variables_.erase(id);
    probabilities_.erase(id);
    graph_.rem_vertex(id);

    return true;
}

bool Bayesnet::add_arc(const std::string &parent, const std::string &child)
{
    auto p_it = nodes_.find(parent);
    auto c_it = nodes_.find(child);

    if (p_it == nodes_.end() || c_it == nodes_.end())
        return false;

    return graph_.add_adjacent(c_it->second, p_it->second);
}

bool Bayesnet::rem_arc(const std::string &parent, const std::string &child)
{
    auto p_it = nodes_.find(parent);
    auto c_it = nodes_.find(child);

    if (p_it == nodes_.end() || c_it == nodes_.end())
        return false;

    return graph_.rem_adjacent(c_it->second, p_it->second);    
}

bool Bayesnet::probability(const std::string &node, const std::string &state, 
    const variables_map_type &parent_states, double p)
{
    auto node_it = nodes_.find(node);
    if (node_it == nodes_.end())
        return false;
    
    const std::size_t node_id = node_it->second;
    std::vector<std::size_t> joint = graph_.adjacents(node_id);
    joint.push_back(node_id);

    auto vars = id_map(parent_states);
    vars[node_id] = variables_.at(node_id)(state);

    std::size_t stride = 1;
    std::size_t index = 0;
    for (auto it = joint.crbegin(); it != joint.crend(); ++it)
    {
        std::size_t value = 0;
        auto var = vars.find(*it);
        if (var != vars.end())
            value = var->second;
        else
            return false;

        std::size_t arity = variables_.at(*it).arity();
        if (value > arity)
            return false;

        index += value * stride;
        stride *= arity;
    }

    if (index >= probabilities_.at(node_id).size())
        probabilities_.at(node_id).resize(index + 1);

    probabilities_.at(node_id)[index] = p;

    return true;
}

double Bayesnet::probability(const std::string &node, const std::string &state, 
    const variables_map_type &parent_states)
{
    auto node_it = nodes_.find(node);
    if (node_it == nodes_.end())
        return false;
    
    const std::size_t node_id = node_it->second;
    std::vector<std::size_t> joint = graph_.adjacents(node_id);
    joint.push_back(node_id);

    auto vars = id_map(parent_states);
    vars[node_id] = variables_.at(node_id)(state);

    std::size_t stride = 1;
    std::size_t index = 0;
    for (auto it = joint.crbegin(); it != joint.crend(); ++it)
    {
        std::size_t value = 0;
        auto var = vars.find(*it);
        if (var != vars.end())
            value = var->second;
        else
            return 0.0;

        std::size_t arity = variables_.at(*it).arity();
        if (value > arity)
            return 0.0;

        index += value * stride;
        stride *= arity;
    }

    return probabilities_.at(node_id)[index];
}

std::unordered_map<std::size_t, std::size_t> Bayesnet::id_map(const variables_map_type &str_vars)
{
    std::unordered_map<std::size_t, std::size_t> vars;
    for (auto v : str_vars)
    {
        auto id = nodes_.at(v.first);
        vars[id] = variables_.at(id)(v.second);
    }
    return vars;
}
