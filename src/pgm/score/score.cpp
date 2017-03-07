#include <pgm/score/score.h>
#include <pgm/bayesnet.h>

using namespace pgm;

Score::Score(const Dataset &dataset)
    : dataset_(dataset), cache_size_(10)
{}

double Score::operator ()(const Bayesnet &bayesnet)
{
    double total = 0.0;
    for (auto vertex : bayesnet.graph().vertices())
    {
        auto adjacents = bayesnet.graph().adjacents(vertex);        

        // retrieve cache if available
        std::set<std::size_t> adjacents_set(adjacents.cbegin(), adjacents.cend());
        double node_score = 0.0;

        if (ret_cache(vertex, adjacents_set, node_score))
        {
            total += node_score;
            continue;
        }

        std::string child = bayesnet.variable(vertex).name();
        std::vector<std::string> parent;
        for (auto adj_id : adjacents)
            parent.push_back(bayesnet.variable(adj_id).name());
        
        // calculate node score
        node_score = score(child, parent);
        total += node_score;

        // cache node score
        put_cache(vertex, adjacents_set, node_score);
    }
    return total;
}

bool Score::ret_cache(std::size_t child, const std::set<std::size_t> &parent, double &score)
{
    for (auto item : cache_[child])
    {
        if (item.first == parent)
        {
            score = item.second;
            return true;
        }
    }
    return false;
}

void Score::put_cache(std::size_t child, const std::set<std::size_t> &parent, double score)
{
    std::list<std::pair<std::set<std::size_t>, double>> &items = cache_[child];
    items.push_front(std::make_pair(parent, score));
    if (items.size() > cache_size_)
        items.pop_back();
}
