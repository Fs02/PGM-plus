#ifndef PGM_BAYESNET_H
#define PGM_BAYESNET_H

#include <pgm/variable.h>
#include <pgm/dgraph.h>
#include <iostream>

namespace pgm {

class Dataset;

//! Bayesian Network Model
/*! 
    Used to create a Bayesian Network Model.

    Usage example:

    \dot
    digraph example {
        winter -> sprinkler
        winter -> rain
        sprinkler -> wetgrass
        rain -> wetgrass
        rain -> slippery
    }
    \enddot
    
    \code

    #include <pgm/bayesnet.h>

    int main()
    {
        // construct structure
        pgm::Bayesnet bn;
        bn.add_node("winter", {"F", "T"});
        bn.add_node("sprinkler", {"F", "T"});
        bn.add_node("rain", {"F", "T"});
        bn.add_node("wetgrass", {"F", "T"});
        bn.add_node("slippery", {"F", "T"});

        bn.add_arc("winter", "sprinkler");
        bn.add_arc("winter", "rain");
        bn.add_arc("sprinkler", "wetgrass");
        bn.add_arc("rain", "wetgrass");
        bn.add_arc("rain", "slippery");

        // assigns cpt
        bn.probability("winter", "T", {}, 0.6);
        bn.probability("winter", "F", {}, 0.4);

        bn.probability("sprinkler", "T", {{"winter", "T"}}, 0.2);
        bn.probability("sprinkler", "F", {{"winter", "T"}}, 0.8);
        bn.probability("sprinkler", "T", {{"winter", "F"}}, 0.75);
        bn.probability("sprinkler", "F", {{"winter", "F"}}, 0.25);

        bn.probability("rain", "T", {{"winter", "T"}}, 0.8);
        bn.probability("rain", "F", {{"winter", "T"}}, 0.2);
        bn.probability("rain", "T", {{"winter", "F"}}, 0.1);
        bn.probability("rain", "F", {{"winter", "F"}}, 0.9);

        bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "T"}}, 0.95);
        bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "T"}}, 0.05);
        bn.probability("wetgrass", "T", {{"sprinkler", "T"}, {"rain", "F"}}, 0.9);
        bn.probability("wetgrass", "F", {{"sprinkler", "T"}, {"rain", "F"}}, 0.1);
        bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "T"}}, 0.8);
        bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "T"}}, 0.2);
        bn.probability("wetgrass", "T", {{"sprinkler", "F"}, {"rain", "F"}}, 0.0);
        bn.probability("wetgrass", "F", {{"sprinkler", "F"}, {"rain", "F"}}, 1.0);

        bn.probability("slippery", "T", {{"rain", "T"}}, 0.7);
        bn.probability("slippery", "F", {{"rain", "T"}}, 0.3);
        bn.probability("slippery", "T", {{"rain", "F"}}, 0.0);
        bn.probability("slippery", "F", {{"rain", "F"}}, 1.0);

        // query probability
        bn.query({{"slippery", "T"}}); // 0.364
        bn.query({{"winter", "T"}, {"sprinkler", "T"}, {"rain", "F"}, {"wetgrass", "T"}, {"slippery", "F"}}); // 0.0216

        // inference
        bn.infer("sprinkler", {}) // F
        bn.infer("rain", {}); // T
    }

    \endcode
*/
class Bayesnet
{
public:
    typedef std::unordered_map<std::string, std::string> variables_map_type;

    //! Default constructor
    /*!
        Construct empty bayesnet
    */
    Bayesnet();

    //! Construct with dataset
    /*!
        Construct arc-less bayesnet with variables based on provided dataset
        \param dataset Dataset
    */
    Bayesnet(const Dataset &dataset);

    //! Infer the most probable occurence given evidence
    /*!
        \param occurence Occurence to predict
        \param evidence Known evidence
        \return Most probable state of occurence
    */
    std::string infer(const std::string &occurence, const variables_map_type &evidence) const;

    //! Get probability of some evidence occuring
    /*!
        \param evidence Evidence
        \return probability of the evidence
    */
    double query(const variables_map_type &evidence) const;

    //! Add node
    /*!
        \param variable Variable of new node
    */
    bool add_node(const Variable &variable);

    //! Add node
    /*!
        \param name Name of new node
        \param states States of new node
        \return True if success
    */    
    bool add_node(const std::string &name, const std::vector<std::string> &states);

    //! Remove node
    /*!
        \param name Name of new node
        \return True if success
    */    
    bool rem_node(const std::string &name);

    //! Add arc between nodes
    /*!
        \param parent Parent node
        \param child Child node
        \return True if a valid arc
    */    
    bool add_arc(const std::string &parent, const std::string &child);

    //! Remove arc between nodes
    /*!
        \param parent Parent node
        \param child Child node
        \return True if success, false if does not exist
    */
    bool rem_arc(const std::string &parent, const std::string &child);

    //! Set a probability to a node
    /*!
        \param node Target mode
        \param state State of the target node
        \param parents_states Parent states of target node
        \param p Probability
        \return True if success
    */    
    bool probability(const std::string &node, const std::string &state, 
        const variables_map_type &parents_states, double p);

    //! Get a probability of a node
    /*!
        \param node Target mode
        \param state State of the target node
        \param parents_states Parent states of target node
        \return Probability
    */
    double probability(const std::string &node, const std::string &state, 
        const variables_map_type &parents_states) const;

    //! Get a network variable
    /*!
        \param name Name of variable
        \return Variable
    */
    inline const Variable &variable(const std::string &name) const { return variables_.at(nodes_.at(name)); }

    //! Get a network variable
    /*!
        \param name Name of variable
        \return Variable
    */
    inline Variable variable(const std::string &name) { return variables_.at(nodes_.at(name)); }

    //! Get a network variable
    /*!
        \param id Id of variable
        \return Variable
    */
    inline const Variable &variable(std::size_t id) const { return variables_.at(id); }

    //! Get a network variable
    /*!
        \param id Id of variable
        \return Variable
    */
    inline Variable variable(std::size_t id) { return variables_.at(id); }

    //! Get directed graph
    /*!
        \return DGraph
    */
    inline DGraph &graph() { return graph_; }

    //! Get directed graph
    /*!
        \return DGraph
    */
    inline const DGraph &graph() const { return graph_; }

    friend std::ostream &operator <<(std::ostream &os, const Bayesnet &bn);

private:
    typedef std::unordered_map<std::size_t, std::size_t> raw_variables_map_type;

    raw_variables_map_type id_map(const variables_map_type &str_vars) const;

    double query(const raw_variables_map_type &evidence) const;
    double probability(std::size_t node, std::size_t state, 
        const raw_variables_map_type &parents_states) const;

    std::unordered_map<std::string, const std::size_t> nodes_;
    std::unordered_map<std::size_t, const Variable> variables_;
    std::unordered_map<std::size_t, std::vector<double>> probabilities_;
    DGraph graph_;
};

std::ostream &operator <<(std::ostream &os, const Bayesnet &bn);

}

#endif
