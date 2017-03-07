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
    Score(const Dataset &dataset, std::size_t cache_size = 10);

    double operator ()(const Bayesnet &bayesnet);
    virtual double score(const std::string &child, const std::vector<std::string> &parent) = 0;

protected:
    bool ret_cache(std::size_t child, const std::set<std::size_t> &parent, double &score);
    void put_cache(std::size_t child, const std::set<std::size_t> &parent, double score);

    const Dataset &dataset_;
    std::size_t cache_size_;
    std::unordered_map<std::size_t, std::list<std::pair<std::set<std::size_t>, double>>> cache_;
};

}

#endif