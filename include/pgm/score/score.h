#ifndef PGM_SCORE_H
#define PGM_SCORE_H

#include <pgm/dataset.h>
#include <set>
#include <list>

namespace pgm {

class Bayesnet;

class Score
{
public:
    Score(const Dataset &dataset);

    double operator ()(const Bayesnet &bayesnet);
    virtual double score(const std::string &child, const std::vector<std::string> &parents) = 0;

    inline std::size_t cache_size() const { return cache_size_; }
    inline void cache_size(std::size_t size) { cache_size_ = size; }
protected:
    bool ret_cache(std::size_t child, const std::set<std::size_t> &parents, double &score);
    void put_cache(std::size_t child, const std::set<std::size_t> &parents, double score);

    const Dataset &dataset_;
    std::size_t cache_size_;
    std::unordered_map<std::size_t, std::list<std::pair<std::set<std::size_t>, double>>> cache_;
};

}

#endif