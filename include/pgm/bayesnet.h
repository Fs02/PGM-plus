#ifndef PGM_BAYESNET_H
#define PGM_BAYESNET_H

#include <pgm/variable.h>
#include <pgm/dgraph.h>
#include <iostream>

namespace pgm {

class Dataset;

class Bayesnet
{
public:
    typedef std::unordered_map<std::string, std::string> variables_map_type;

    Bayesnet();
    Bayesnet(const Dataset &dataset);

    std::string infer(const std::string &occurence, const variables_map_type &evidence) const;
    double query(const variables_map_type &evidence) const;

    bool add_node(const Variable &variable);
    bool add_node(const std::string &name, const std::vector<std::string> &states);
    bool rem_node(const std::string &name);

    bool add_arc(const std::string &parent, const std::string &child);
    bool rem_arc(const std::string &parent, const std::string &child);

    bool probability(const std::string &node, const std::string &state, 
        const variables_map_type &parents_states, double p);

    double probability(const std::string &node, const std::string &state, 
        const variables_map_type &parents_states) const;

    inline const Variable &variable(const std::string &name) const { return variables_.at(nodes_.at(name)); }
    inline Variable variable(const std::string &name) { return variables_.at(nodes_.at(name)); }

    inline const Variable &variable(std::size_t id) const { return variables_.at(id); }
    inline Variable variable(std::size_t id) { return variables_.at(id); }

    inline DGraph &graph() { return graph_; }
    inline const DGraph &graph() const { return graph_; }

    friend std::ostream &operator <<(std::ostream &os, const Bayesnet &bn);

private:
    std::unordered_map<std::size_t, std::size_t> id_map(const variables_map_type &str_vars) const;

    std::unordered_map<std::string, const std::size_t> nodes_;
    std::unordered_map<std::size_t, const Variable> variables_;
    std::unordered_map<std::size_t, std::vector<double>> probabilities_;
    DGraph graph_;
};

std::ostream &operator <<(std::ostream &os, const Bayesnet &bn);

}

#endif
