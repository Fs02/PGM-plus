#ifndef PGM_SIMULATEDANNEALING_H
#define PGM_SIMULATEDANNEALING_H

#include <functional>

namespace pgm {

class Bayesnet;

// Reference:
// Bayesian Belief Networks: from Construction to Inference
// R.R. Bouckaert
class SimulatedAnnealing
{
public:
    typedef std::function<double(const Bayesnet&)> score_type;

    SimulatedAnnealing(std::size_t max_iter = 10000, double initial_temp = 10, double delta = 0.999, unsigned seed = 1);

    double operator() (Bayesnet &bayesnet, const score_type &score);

    inline void init_as_naive_bayes(const std::string &class_node) { class_node_ = class_node; }
    void init_naive_bayes(Bayesnet &bayesnet);

    inline std::size_t max_iter() const { return max_iter_; }
    inline void max_iter(std::size_t n) { max_iter_ = n; }

    inline double initial_temp() const { return initial_temp_; }
    inline void initial_temp(double temp) { initial_temp_ = temp; }

    inline double delta() const { return delta_; }
    inline void delta(double d) { delta_ = d; }

    inline unsigned seed() const { return seed_; }
    inline void seed(double s) { seed_ = s; }

    inline double verbose() const { return verbose_; }
    inline void verbose(bool v) { verbose_ = v; }

private:
    std::size_t max_iter_;
    double initial_temp_;
    double delta_;
    unsigned seed_;
    std::string class_node_;
    bool verbose_;
};

}

#endif