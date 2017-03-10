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
    auto node_it = nodes_.find(occurence);
    if (node_it == nodes_.end())
        return "";

    const auto node_id = node_it->second;
    const auto var = variables_.at(node_id);
    double max_prob = 0.0;
    std::string state = "";

    raw_variables_map_type raw_evidence = id_map(evidence);
    for (std::size_t i = 0; i < var.cardinality(); ++i)
    {
        raw_evidence[node_id] = i;
        const double p = query(raw_evidence);
        if (p >= max_prob)
        {
            max_prob = p;
            state = var(i);
        }
    }

    return state;
}

double Bayesnet::query(const variables_map_type &evidence) const
{
    return query(id_map(evidence));
}

bool Bayesnet::add_node(const Variable &variable)
{
    return add_node(variable.name(), variable.states());
}

bool Bayesnet::add_node(const std::string &name, const std::vector<std::string> &states)
{
    if (nodes_.find(name) != nodes_.end())
        return false;

    const std::size_t id = graph_.add_vertex();
    nodes_.insert({name, id});
    variables_.insert({id, Variable(name, states)});
    probabilities_[id] = std::vector<double>();

    return true;
}

bool Bayesnet::rem_node(const std::string &name)
{
    if (nodes_.find(name) != nodes_.end())
        return false;

    const std::size_t id = nodes_[name];
    nodes_.erase(name);
    variables_.erase(id);
    probabilities_.erase(id);
    graph_.rem_vertex(id);

    return true;
}

bool Bayesnet::add_arc(const std::string &parent, const std::string &child)
{
    const auto p_it = nodes_.find(parent);
    const auto c_it = nodes_.find(child);

    if (p_it == nodes_.end() || c_it == nodes_.end())
        return false;

    return graph_.add_adjacent(c_it->second, p_it->second);
}

bool Bayesnet::rem_arc(const std::string &parent, const std::string &child)
{
    const auto p_it = nodes_.find(parent);
    const auto c_it = nodes_.find(child);

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

        const std::size_t cardinality = variables_.at(*it).cardinality();
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
        return 0.0;
    
    const std::size_t node_id = node_it->second;
    const std::size_t state_id = variables_.at(node_id)(state);

    return probability(node_id, state_id, id_map(parents_states));
}

Bayesnet::raw_variables_map_type Bayesnet::id_map(const variables_map_type &str_vars) const
{
    raw_variables_map_type vars;
    vars.reserve(str_vars.size());
    for (auto v : str_vars)
    {
        auto id = nodes_.at(v.first);
        vars[id] = variables_.at(id)(v.second);
    }
    return vars;
}

double Bayesnet::query(const raw_variables_map_type &evidence) const
{
    auto mutable_evidence = evidence;

    typedef std::unordered_map<std::size_t, const Variable>::const_iterator iterator_type;
    std::function<double(iterator_type)> calculate =
        [this, &mutable_evidence, &calculate](iterator_type it) {
        double p = 1.0;
        while (it != variables_.cend())
        {
            auto curr_var_id = it->first;
            if (mutable_evidence.find(curr_var_id) == mutable_evidence.end())
            {
                double sum = 0.0;
                for (std::size_t i = 0; i < it->second.cardinality(); ++i)
                {
                    mutable_evidence[curr_var_id] = i;
                    sum += calculate(it);
                }
                mutable_evidence.erase(curr_var_id);
                return p * sum;
            }

            for (auto adjacent : graph_.adjacents(curr_var_id))
            {
                if (mutable_evidence.find(adjacent) == mutable_evidence.end())
                {
                    auto var = variables_.at(adjacent);
                    double sum = 0.0;
                    for (std::size_t i = 0; i < var.cardinality(); ++i)
                    {
                        mutable_evidence[adjacent] = i;
                        sum += calculate(it);
                    }
                    mutable_evidence.erase(adjacent);
                    return p * sum;
                }
            }

            p *= probability(curr_var_id, mutable_evidence.at(curr_var_id), mutable_evidence);
            ++it;
        }

        return p;
    };

    return calculate(variables_.cbegin());
}

double Bayesnet::probability(std::size_t node, std::size_t state, 
    const raw_variables_map_type &parents_states) const
{    
    std::vector<std::size_t> joint = graph_.adjacents(node);
    joint.push_back(node);

    std::size_t stride = 1;
    std::size_t index = 0;
    for (auto it = joint.crbegin(); it != joint.crend(); ++it)
    {
        std::size_t value = 0;
        if (*it == node)
        {
            value = state;
        }
        else
        {
            auto var = parents_states.find(*it);
            if (var != parents_states.end())
                value = var->second;
            else
            return 0.0;
        }

        const std::size_t cardinality = variables_.at(*it).cardinality();
        if (value > cardinality)
            return 0.0;

        index += value * stride;
        stride *= cardinality;
    }

    return probabilities_.at(node)[index];    
}


std::ostream &pgm::operator <<(std::ostream &os, const Bayesnet &bn)
{
    std::streamsize ss = std::cout.precision();
    os << "\nBayesian Network Model\n";
    os << "==> Structure\n";
    for (auto v : bn.graph().vertices())
    {
        const auto variable = bn.variable(v);
        os << variable.name() << " (" << variable.cardinality() << ") :";
        for (auto adj : bn.graph().adjacents(v))
        {
            os << " " << bn.variable(adj).name();
        }
        os << "\n";
    }
    os << "==> Probability Distribution\n";
    std::cout.precision(2);
    for (auto v : bn.graph().vertices())
    {
        os << bn.variable(v) << " :";
        for (auto p : bn.probabilities_.at(v))
        {
            os << " " << p;
        }
        os << "\n";
    }
    std::cout.precision (ss);

    return os;
}
