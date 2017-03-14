#ifndef PGM_GREEDYHILLCLIMBING_H
#define PGM_GREEDYHILLCLIMBING_H

#include <functional>

namespace pgm {

class Bayesnet;

// Reference:
// Learning Bayesian Network Structures
// Brandon Malone
class GreedyHillClimbing
{
public:
    typedef std::function<double(const Bayesnet&)> score_type;

    GreedyHillClimbing(std::size_t max_iter = 10000, unsigned seed = 1);

    double operator() (Bayesnet &bayesnet, const score_type &score);

    inline void init_as_naive_bayes(const std::string &class_node) { class_node_ = class_node; }
    void init_naive_bayes(Bayesnet &bayesnet);

    inline std::size_t max_iter() const { return max_iter_; }
    inline void max_iter(std::size_t n) { max_iter_ = n; }

    inline unsigned seed() const { return seed_; }
    inline void seed(double s) { seed_ = s; }

    inline double verbose() const { return verbose_; }
    inline void verbose(bool v) { verbose_ = v; }

private:
    std::size_t max_iter_;
    unsigned seed_;
    std::string class_node_;
    bool verbose_;
};

}

#endif