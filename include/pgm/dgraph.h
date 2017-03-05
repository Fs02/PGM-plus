#ifndef PGM_DGRAPH_H
#define PGM_DGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

namespace pgm {

//! Directed DGraph
class DGraph
{
public:
    DGraph(bool acyclic = false);

    /*
     * Vertex Operations
     */
    std::size_t add_vertex();
    bool rem_vertex(std::size_t id);

    inline std::vector<std::size_t> vertices()
        { return vertices_; }
    inline const std::vector<std::size_t> &vertices() const
        { return vertices_; }

    /*
     * Adjacents Operations
     */
    inline std::vector<std::size_t> adjacents(std::size_t vertex)
        { return adjacency_list_.at(vertex); }
    inline const std::vector<std::size_t> &adjacents(std::size_t vertex) const
        { return adjacency_list_.at(vertex); }

    bool add_adjacent(std::size_t from, std::size_t to);
    bool rem_adjacent(std::size_t from, std::size_t to);
    bool rev_adjacent(std::size_t from, std::size_t to);

    bool clear();
    bool clear_adjacents(std::size_t id);
    bool clear_all_adjacents();

    bool valid_vertex(std::size_t id) const;
    bool valid_acyclic(std::size_t from, std::size_t to) const;
    bool invalid_acyclic(std::size_t from, std::size_t to) const;

    /*
     * Properties
     */
    inline bool acyclic() const { return acyclic_; }
    inline void acyclic(bool acyclic) { acyclic_ = acyclic; }

    inline std::size_t max_adjacents() const { return max_adjacents_; }
    inline void max_adjacents(std::size_t max) { max_adjacents_ = max; }

private:
    std::vector<std::size_t> vertices_;
    std::unordered_map<std::size_t, std::vector<std::size_t>> adjacency_list_;
    bool acyclic_;
    std::size_t max_adjacents_;
};

}

#endif
