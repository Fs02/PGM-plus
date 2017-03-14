#ifndef PGM_DGRAPH_H
#define PGM_DGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

namespace pgm {

//! Directed Graph 
/*!
    A Container that represents directed graph. Graph is stored using adjacency list format.
    Used internally in Bayesnet model with reversed arc.
 */
class DGraph
{
public:
    //! Default constructor
    /*!
        \param acyclic Constraint graph as acyclic graph
    */
    DGraph(bool acyclic = false);

    //! Add a new vertex
    /*!
        \return Id of the new vertex
     */
    std::size_t add_vertex();

    //! Remove a vertex
    /*!
        \param id Vertex id
        \return Return true if success
    */
    bool rem_vertex(std::size_t id);

    //! Get list of vertices id
    /*!
        \return Vector of vertices id
    */
    inline std::vector<std::size_t> vertices()
        { return vertices_; }

    //! Get list of vertices id
    /*!
        \return Vector of vertices id
    */
    inline const std::vector<std::size_t> &vertices() const
        { return vertices_; }

    //! Get adjacents list of a vertex
    /*!
        \param vertex Vertex id
        \return Vector of vertices id
    */
    inline std::vector<std::size_t> adjacents(std::size_t vertex)
        { return adjacency_list_.at(vertex); }

    //! Get adjacents list of a vertex
    /*!
        \param vertex Vertex id
        \return Vector of vertices id
    */        
    inline const std::vector<std::size_t> &adjacents(std::size_t vertex) const
        { return adjacency_list_.at(vertex); }

    //! Add an adjacent
    /*!
        Add a new adjacent to the graph, will perform acyclic check if it's acyclic graph
        \param from Origin vertex id
        \param to Destination vertex id
        \return Return true if operation is valid
    */
    bool add_adjacent(std::size_t from, std::size_t to);

    //! Remove an adjacent
    /*!
        Remove an adjacent to the graph, will perform acyclic check if it's acyclic graph
        \param from Origin vertex id
        \param to Destination vertex id
        \return Return true if operation is valid
    */
    bool rem_adjacent(std::size_t from, std::size_t to);

    //! Reverse an adjacent
    /*!
        Remove an adjacent arc, will perform acyclic check if it's acyclic graph
        \param from Origin vertex id
        \param to Destination vertex id
        \return Return true if operation is valid
    */
    bool rev_adjacent(std::size_t from, std::size_t to);

    //! Clear graph
    /*!
        Clear all vertex and adjacents
        \return Return true if success
    */

    bool clear();

    //! Clear all adjacents of a vertex
    /*!
        \param id Vertex id
        \return Return true if success
    */
    bool clear_adjacents(std::size_t id);

    //! Clear all adjacents
    /*!
        Clear all adjacents
        \return Return true if success
    */
    bool clear_all_adjacents();

    //! Check whether vertex id is a valid id
    /*!
        \param id Vertex id
        \return True if vertex is valid
    */
    bool valid_vertex(std::size_t id) const;

    //! Check whether adding a new edge resulting a valid acyclic graph
    /*!
        \param from Origin vertex id
        \param to Destination vertex id
        \return True if graph is acyclic
    */
    bool valid_acyclic(std::size_t from, std::size_t to);

    //! Get graph acyclic property
    /*!
        \return Return value of acyclic property
    */
    inline bool acyclic() const { return acyclic_; }

    //! Set graph acyclic property
    /*!
        \param acyclic Boolean of acyclic status
    */
    inline void acyclic(bool acyclic) { acyclic_ = acyclic; }

    //! Get maximum adjacents property
    /*!
        \return Number of maximum adjacents
    */
    inline std::size_t max_adjacents() const { return max_adjacents_; }

    //! Set maximum adjacents property
    /*!
        \param max Number of maximum adjacents
    */
    inline void max_adjacents(std::size_t max) { max_adjacents_ = max; }

private:
    std::vector<std::size_t> vertices_;
    std::unordered_map<std::size_t, std::vector<std::size_t>> adjacency_list_;
    bool acyclic_;
    std::size_t max_adjacents_;
};

}

#endif
