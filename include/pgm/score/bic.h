#ifndef PGM_BIC_H
#define PGM_BIC_H

#include <pgm/score/loglikelihood.h>

namespace pgm {

//! Bayesian Information Criterion (BIC) scoring function
/*!
    Equation:
    \f{eqnarray*}{
        BIC(B|T) &=& LL(B|T) - \frac{1}{2} \log(N) \sum_{i=1}^{n}(r_i-1)q_i
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
        pgm::Bic score(dataset);
        score(bn);
        score.score("A", {"B"});
    }

    \endcode

    For more information see:

    Carvalho, A. M. Scoring functions for learning bayesian networks, 2009.

    http://www.lx.it.pt/~asmc/pub/talks/09-TA/ta_pres.pdf
*/
class Bic : public LogLikelihood
{
public:
    //! Default constructor
    /*!
        \param dataset Dataset
    */
    Bic(const Dataset &dataset);

    //! Compute decomposed score
    /*!
        \param child Child node
        \param parents Parents nodes
        \return decomposed score
    */
    double score(const std::string &child, const std::vector<std::string> &parents);
};

}

#endif