#include <pgm/bayesnet.h>
#include <pgm/dataset.h>
#include <functional>
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

std::string Bayesnet::infer(const std::string &occurence, const variables_map_type &evidence) const
{
    auto states = variables_.at(nodes_.at(occurence)).states();
    double max_prob = 0.0;
    std::string state = "";

    variables_map_type merged_evidence = evidence;
    for (auto s : states)
    {
        merged_evidence[occurence] = s;
        double p = query(merged_evidence);
        if (p >= max_prob)
        {
            max_prob = p;
            state = s;
        }
    }

    return state;
}

double Bayesnet::query(const variables_map_type &evidence) const
{
    typedef std::unordered_map<std::size_t, const Variable>::const_iterator iterator_type;
    std::function<double(iterator_type, const variables_map_type&)> calculate =
        [this, &calculate](iterator_type it, const variables_map_type &evidence) {
        double p = 1.0;
        while (it != variables_.cend())
        {
            auto curr_var_name = it->second.name();
            if (evidence.find(curr_var_name) == evidence.end())
            {
                double sum = 0.0;
                auto new_evidence = evidence;
                for (auto state : it->second.states())
                {
                    new_evidence[curr_var_name] = state;
                    sum += calculate(it, new_evidence);
                }
                return p * sum;
            }

            for (auto adjacent : graph_.adjacents(it->first))
            {
                auto var = variables_.at(adjacent);
                int a = 0;
                if (evidence.find(var.name()) == evidence.end())
                {
                    double sum = 0.0;
                    auto new_evidence = evidence;
                    for (auto state : var.states())
                    {
                        new_evidence[var.name()] = state;
                        sum += calculate(it, new_evidence);
                    }
                    return p * sum;
                }
            }

            p *= probability(curr_var_name, evidence.at(curr_var_name), evidence);
            ++it;
        }

        return p;
    };

    return calculate(variables_.cbegin(), evidence);
}

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
    const variables_map_type &parents_states, double p)
{
    auto node_it = nodes_.find(node);
    if (node_it == nodes_.end())
        return false;
    
    const std::size_t node_id = node_it->second;
    std::vector<std::size_t> joint = graph_.adjacents(node_id);
    joint.push_back(node_id);

    auto vars = id_map(parents_states);
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

        std::size_t cardinality = variables_.at(*it).cardinality();
        if (value > cardinality)
            return false;

        index += value * stride;
        stride *= cardinality;
    }

    if (index >= probabilities_.at(node_id).size())
        probabilities_.at(node_id).resize(index + 1);

    probabilities_.at(node_id)[index] = p;

    return true;
}

double Bayesnet::probability(const std::string &node, const std::string &state, 
    const variables_map_type &parents_states) const
{
    auto node_it = nodes_.find(node);
    if (node_it == nodes_.end())
        return false;
    
    const std::size_t node_id = node_it->second;
    std::vector<std::size_t> joint = graph_.adjacents(node_id);
    joint.push_back(node_id);

    auto vars = id_map(parents_states);
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

        std::size_t cardinality = variables_.at(*it).cardinality();
        if (value > cardinality)
            return 0.0;

        index += value * stride;
        stride *= cardinality;
    }

    return probabilities_.at(node_id)[index];
}

std::unordered_map<std::size_t, std::size_t> Bayesnet::id_map(const variables_map_type &str_vars) const
{
    std::unordered_map<std::size_t, std::size_t> vars;
    for (auto v : str_vars)
    {
        auto id = nodes_.at(v.first);
        vars[id] = variables_.at(id)(v.second);
    }
    return vars;
}
