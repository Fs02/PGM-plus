#include <pgm/dgraph.h>
#include <algorithm>
#include <functional>


using namespace pgm;

DGraph::DGraph(bool acyclic)
    : vertices_(), adjacency_list_(), acyclic_(acyclic), max_adjacents_(0)
{}

std::size_t DGraph::add_vertex()
{
    std::size_t new_id = 0;
    if (!vertices_.empty())
        new_id = vertices_.back() + 1;

    vertices_.push_back(new_id);
    adjacency_list_[new_id] = std::vector<std::size_t>();
    return new_id;
}

bool DGraph::rem_vertex(std::size_t id)
{
    if (!valid_vertex(id))
        return false;

    for (auto adjacents : adjacency_list_)
        rem_adjacent(adjacents.first, id);

    adjacency_list_.erase(id);
    vertices_.erase(std::remove(vertices_.begin(), vertices_.end(), id), vertices_.end());

    return true;
}

bool DGraph::add_adjacent(std::size_t from, std::size_t to)
{
    if (!valid_vertex(from) || !valid_vertex(to))
        return false;

    std::vector<std::size_t> &adjacents = adjacency_list_.at(from);

    // max parents exceeded
    if (max_adjacents_ > 0 && adjacents.size() >= max_adjacents_)
        return false;

    // already adjacent
    auto it = std::find(adjacents.cbegin(), adjacents.cend(), to);
    if (it != adjacents.cend())
        return false;

    // check for acyclic
    if (acyclic_ && !valid_acyclic(from, to))
        return false;

    adjacents.push_back(to);
    return true;
}

bool DGraph::rem_adjacent(std::size_t from, std::size_t to)
{
    if (!valid_vertex(from) || !valid_vertex(to))
        return false;

    std::vector<std::size_t> &adjacents = adjacency_list_.at(from);

    // make sure vertex is adjacent
    auto it = std::find(adjacents.cbegin(), adjacents.cend(), to);
    if (it == adjacents.cend())
        return false;

    adjacents.erase(it);
    return true;
}

bool DGraph::rev_adjacent(std::size_t from, std::size_t to)
{
    if (!rem_adjacent(from, to))
        return false;

    if (add_adjacent(to, from))
        return true;

    adjacency_list_.at(from).push_back(to);
    return false;
}

bool DGraph::clear()
{
    vertices_.clear();
    adjacency_list_.clear();
    return true;
}

bool DGraph::clear_adjacents(std::size_t id)
{
    adjacency_list_.at(id).clear();
    return true;
}

bool DGraph::clear_all_adjacents()
{
    for (auto adjacents : adjacency_list_)
        adjacents.second.clear();
    
    return true;
}

bool DGraph::valid_vertex(std::size_t id) const
{
    return adjacency_list_.find(id) != adjacency_list_.end();
}

bool DGraph::valid_acyclic(std::size_t from, std::size_t to)
{
    if (from == to)
        return false;

    std::vector<std::size_t> &adjacents = adjacency_list_.at(from);
    adjacents.push_back(to);

    // make sure there's root nodes
    // ref: https://github.com/bnjmn/weka/blob/master/weka/src/main/java/weka/classifiers/bayes/net/search/SearchAlgorithm.java
    std::unordered_map<std::size_t, bool> checked;
    for (std::size_t i = 0; i < vertices_.size(); ++i)
    {
        bool found = false;
        for (auto v : vertices_)
        {
            if (!checked[v])
            {
                bool has_no_parents = true;
                for (auto adj : adjacency_list_.at(v))
                {
                    if (!checked[adj])
                        has_no_parents = false;
                }

                if (has_no_parents)
                {
                    checked[v] = true;
                    found = true;
                    break;
                }
            }
        }

        if (!found)
        {
            adjacents.pop_back();
            return false;
        }
    }

    adjacents.pop_back();
    return true;
}