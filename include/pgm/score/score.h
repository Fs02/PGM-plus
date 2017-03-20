#ifndef PGM_SCORE_H
#define PGM_SCORE_H

#include <pgm/dataset.h>
#include <set>
#include <list>

namespace pgm {

class Bayesnet;

//! Abstract class for scoring function
class Score
{
public:
    //! Default constructor
    /*!
        \param dataset Dataset
    */
    Score(const Dataset &dataset);

    //! Compute score of a Bayesnet
    /*!
        \param bayesnet Bayesnet
        \return score
    */
    double operator ()(const Bayesnet &bayesnet);

    //! Compute decomposed score
    /*!
        \param child Child node
        \param parents Parents nodes
        \return decomposed score
    */
    virtual double score(const std::string &child, const std::vector<std::string> &parents) = 0;

    //! Get cache size
    /*!
        \return Number of score history to be cached
    */
    inline std::size_t cache_size() const { return cache_size_; }

    //! Set cache size
    /*!
        \param size Number of score history to be cached
    */
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