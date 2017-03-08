#include <pgm/search/simulatedannealing.h>
#include <pgm/bayesnet.h>
#include <random>
#include <iostream>

using namespace pgm;

SimulatedAnnealing::SimulatedAnnealing(std::size_t max_iter, double initial_temp, double delta, unsigned seed)
    : max_iter_(max_iter), initial_temp_(initial_temp), delta_(delta), seed_(seed), verbose_(false)
{}

double SimulatedAnnealing::operator() (Bayesnet &bayesnet, const score_type &score)
{
    std::mt19937 random(seed_);
    std::uniform_int_distribution<std::size_t> vertex_dist(0, bayesnet.graph().vertices().size() - 1);
    std::uniform_real_distribution<double> acceptance_dist(0.0, 1.0);

    bayesnet.graph().clear_all_adjacents();
    double current_score = score(bayesnet);    
    double best_score = current_score;
    Bayesnet best_bayesnet = bayesnet;

    double temp = initial_temp_;
    for (std::size_t iter = 0; iter < max_iter_; ++iter)
    {
        DGraph &graph = bayesnet.graph();
        bool is_operated = false;
        while (!is_operated)
        {
            auto child = graph.vertices()[vertex_dist(random)];
            auto parent = graph.vertices()[vertex_dist(random)];
            while (child == parent)
                parent = graph.vertices()[vertex_dist(random)];

            if (graph.rem_adjacent(child, parent))
            {
                // try to remove adjacent
                is_operated = true;
                const double new_score = score(bayesnet);
                const double delta_score = new_score - current_score;

                if ((temp * acceptance_dist(random) + 1e-100) < delta_score)
                    current_score = new_score;
                else
                    graph.add_adjacent(child, parent);
            }
            else if (graph.add_adjacent(child, parent))
            {
                // try to add adjacent
                is_operated = true;
                const double new_score = score(bayesnet);
                const double delta_score = new_score - current_score;
 
                if ((temp * acceptance_dist(random) + 1e-100) < delta_score)
                    current_score = new_score;
                else
                    graph.rem_adjacent(child, parent);                
            }
        }

        if (current_score > best_score)
        {
            best_bayesnet = bayesnet;
            best_score = current_score;

            if (verbose_)
            {
                std::cout << "Iter: " << iter << " Temp: " << temp << " Score: " << best_score << "\n";
            }
        }

        temp *= delta_;
    }
    bayesnet = best_bayesnet;
    return best_score;
}
