#ifndef PGM_FCLL_H
#define PGM_FCLL_H

#include <pgm/score/loglikelihood.h>

namespace pgm {

//! Factorized Conditional Log Likelihood (fCLL) scoring function
/*!
    Equation:
    \f{eqnarray*}{
        \widehat{f}CLL (G|D)&=(\alpha + \beta )\widehat{LL}(B|D)-\beta \lambda \sum_{i=1}^{n} \sum_{j=1}^{q^*_i} \sum_{k=1}^{r_i} \sum_{c=0}^{1} N_{ijk}(\log(\frac{N_{ijck}}{N_{ij*k}})-\log(\frac{N_{ijc}}{N_{ij*}})) \\ 
        &-\beta \lambda \sum_{c=0}^{1}N_c \log(\frac{N_c}{N})-\beta N \rho 
    \f}

    Usage example:

    \code

    #include <pgm/pgm.h>

    int main()
    {
        // structure
        pgm::Bayesnet bn;
        bn.add_node("A", {"F", "T"});
        bn.add_node("B", {"F", "T"});
        bn.add_node("C", {"F", "T"});
        bn.add_node("D", {"F", "T"});
        bn.add_node("E", {"F", "T"});

        bn.add_arc("A", "B");
        bn.add_arc("A", "C");
        bn.add_arc("B", "D");
        bn.add_arc("C", "D");
        bn.add_arc("C", "E");

        // dataset
        pgm::Dataset dataset;
        for (std::size_t i = 0; i < 20; ++i)
            dataset.push({{"A", "T"}, {"B", "F"}, {"C", "T"}, {"D", "T"}, {"E", "T"}});

        for (std::size_t i = 0; i < 15; ++i)
            dataset.push({{"A", "T"}, {"B", "F"}, {"C", "F"}, {"D", "F"}, {"E", "F"}});

        for (std::size_t i = 0; i < 10; ++i)
            dataset.push({{"A", "F"}, {"B", "T"}, {"C", "F"}, {"D", "T"}, {"E", "T"}});

        for (std::size_t i = 0; i < 15; ++i)
            dataset.push({{"A", "F"}, {"B", "F"}, {"C", "T"}, {"D", "T"}, {"E", "T"}});

        for (std::size_t i = 0; i < 5; ++i)
            dataset.push({{"A", "F"}, {"B", "F"}, {"C", "F"}, {"D", "F"}, {"E", "F"}});

        for (std::size_t i = 0; i < 2; ++i)
            dataset.push({{"A", "T"}, {"B", "T"}, {"C", "F"}, {"D", "T"}, {"E", "F"}});

        // estimate parameters
        pgm::Fcll score(dataset, "A");
        score(bn);
        score.score("A", {"B"});
    }

    \endcode

    For more information see:

    Carvalho, A. M., Roos, T., Oliveira, A. L., and Myllymaki, P. 
    Discriminative learning of bayesian networks via factorized conditional 
    log-likelihood. J. Mach. Learn. Res. 12 (July 2011), 2181–2210.
*/
class Fcll : public LogLikelihood
{
public:
    //! Default constructor
    /*!
        \param dataset Dataset
        \param class_node Class Node
    */
    Fcll(const Dataset &dataset, const std::string &class_node);

    //! Compute decomposed score
    /*!
        \param child Child node
        \param parents Parents nodes
        \return decomposed score
    */
    double score(const std::string &child, const std::vector<std::string> &parents);

private:
    const std::string class_node_;
};

}

#endif