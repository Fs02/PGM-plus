#ifndef PGM_BAYESNET_H
#define PGM_BAYESNET_H

#include <pgm/variable.h>
#include <pgm/dgraph.h>

namespace pgm {

class Dataset;

class Bayesnet
{
public:
    typedef const std::unordered_map<std::string, std::string> variables_map_type;

    Bayesnet();
    Bayesnet(const Dataset &dataset);

    std::string infer(const std::string &occurence, const variables_map_type &evidence);
    double query(const variables_map_type &evidence);

    bool add_node(const Variable &variable);
    bool add_node(const std::string &name, const std::vector<std::string> &states);
    bool rem_node(const std::string &name);

    bool add_arc(const std::string &parent, const std::string &child);
    bool rem_arc(const std::string &parent, const std::string &child);

    bool probability(const std::string &node, const std::string &state, 
        const variables_map_type &parent_states, double p);

    double probability(const std::string &node, const std::string &state, 
        const variables_map_type &parent_states);

    inline DGraph &graph() { return graph_; }
    inline const DGraph &graph() const { return graph_; }

private:
    std::unordered_map<std::size_t, std::size_t> id_map(const variables_map_type &str_vars);

    std::unordered_map<std::string, const std::size_t> nodes_;
    std::unordered_map<std::size_t, const Variable> variables_;
    std::unordered_map<std::size_t, std::vector<double>> probabilities_;
    DGraph graph_;
};

}

#endif
