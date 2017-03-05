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

bool DGraph::valid_acyclic(std::size_t from, std::size_t to) const
{
    return !invalid_acyclic(from, to);
}

bool DGraph::invalid_acyclic(std::size_t from, std::size_t to) const
{
    if (from == to)
        return true;

    std::vector<bool> visited(vertices_.size());
    std::vector<bool> recstack(vertices_.size());

    std::function<bool(std::size_t, std::vector<bool>, std::vector<bool>&)> is_cyclic =
            [this, &is_cyclic](std::size_t v, std::vector<bool> visited, std::vector<bool> &recstack)
    {
        if (!visited[v])
        {
            // mark current node as part of recursion stack
            visited[v] = true;
            recstack[v] = true;

            const auto adjacents = this->adjacents(v);
            for (auto w : adjacents)
            {
                if (!visited[w] && is_cyclic(w, visited, recstack))
                    return true;
                else if (recstack[w])
                    return true;
            }
        }
        recstack[v] = false;
        return false;
    };

    visited[from] = true;
    recstack[from] = true;

    return is_cyclic(to, visited, recstack);
}
