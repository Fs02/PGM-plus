#include <pgm/search/greedyhillclimbing.h>
#include <pgm/bayesnet.h>
#include <random>
#include <iostream>

using namespace pgm;

GreedyHillClimbing::GreedyHillClimbing(std::size_t max_iter, unsigned seed)
    : max_iter_(max_iter), seed_(seed), verbose_(false)
{}

double GreedyHillClimbing::operator() (Bayesnet &bayesnet, const score_type &score)
{
    std::mt19937 random(seed_);
    std::uniform_int_distribution<std::size_t> operator_dist(0, 2);
    std::uniform_int_distribution<std::size_t> vertex_dist(0, bayesnet.graph().vertices().size() - 1);
    std::uniform_real_distribution<double> acceptance_dist(0.0, 1.0);

    bayesnet.graph().clear_all_adjacents();
    init_naive_bayes(bayesnet);
    double current_score = score(bayesnet);
    double best_score = current_score;
    const double initial_score = current_score;
    Bayesnet best_bayesnet = bayesnet;

    if (verbose_)
        std::cout << "Initial score: " << current_score << "\n";

    for (std::size_t iter = 0; iter < max_iter_; ++iter)
    {
        DGraph &graph = bayesnet.graph();
        bool is_operated = false;
        while (!is_operated)
        {
            const auto operator_id = operator_dist(random);
            if (operator_id == 0)
            {
                // add edge
                const auto child = graph.vertices()[vertex_dist(random)];
                const auto parent = graph.vertices()[vertex_dist(random)];
                is_operated = graph.add_adjacent(child, parent);
            }
            else if (operator_id == 1)
            {
                // remove edge
                const auto child = graph.vertices()[vertex_dist(random)];
                const auto parents = graph.adjacents(child);

                if (parents.empty())
                    continue;

                std::uniform_int_distribution<std::size_t> parents_dist(0, parents.size() - 1);
                const auto parent = parents[parents_dist(random)];
                is_operated = graph.rem_adjacent(child, parent);
            }
            else if (operator_id == 2)
            {
                // reverse edge
                const auto child = graph.vertices()[vertex_dist(random)];
                const auto parents = graph.adjacents(child);

                if (parents.empty())
                    continue;

                std::uniform_int_distribution<std::size_t> parents_dist(0, parents.size() - 1);
                const auto parent = parents[parents_dist(random)];
                is_operated = graph.rev_adjacent(child, parent);
            }
        }

        current_score = score(bayesnet);

        if (current_score > best_score)
        {
            best_bayesnet = bayesnet;
            best_score = current_score;

            if (verbose_)
                std::cout << "Iter: " << iter << " Score: " << best_score << "\n";
        }
    }

    if (verbose_)
        std::cout << "Optimized by : " << best_score - initial_score << " points\n";

    bayesnet = best_bayesnet;
    return best_score;
}

void GreedyHillClimbing::init_naive_bayes(Bayesnet &bayesnet)
{
    if (class_node_.empty())
        return;

    bool acyclic = bayesnet.graph().acyclic();
    bayesnet.graph().acyclic(false);

    const auto vertices = bayesnet.graph().vertices();
    for (auto v : vertices)
    {
        auto const name = bayesnet.variable(v).name();
        if (class_node_ != name)
           bayesnet.add_arc(class_node_, name);
    }
    bayesnet.graph().acyclic(acyclic);
}