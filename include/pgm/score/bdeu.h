#ifndef PGM_BDEU_H
#define PGM_BDEU_H

#include <pgm/score/score.h>

namespace pgm {
//! BDeu scoring function
/*!
    Equation:
    \f{eqnarray*}{
        BDeu(B|T) &=& \log(P(B))+\sum_{i=1}^{n}\sum_{j=1}^{q_i}(
        \log(\frac{\Gamma(\frac{N'}{q_i})}{\Gamma(N_{ij}+\frac{N'}{q_i})})
        +\sum_{k=1}^{r_i}
        \log(\frac{\Gamma(N_{ijk}+\frac{N'}{r_i q_i})}{\Gamma(\frac{N'}{r_i q_i})})
        )
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
        pgm::Bdeu score(dataset);
        score(bn);
        score.score("A", {"B"});
    }

    \endcode

    For more information see:

    Carvalho, A. M. Scoring functions for learning bayesian networks, 2009.

    http://www.lx.it.pt/~asmc/pub/talks/09-TA/ta_pres.pdf
*/
class Bdeu : public Score
{
public:
    //! Default constructor
    /*!
        \param dataset Dataset
        \param alpha Equivalent sample size
    */
    Bdeu(const Dataset &dataset, double alpha = 0.5);

    //! Compute decomposed score
    /*!
        \param child Child node
        \param parents Parents nodes
        \return decomposed score
    */
    double score(const std::string &child, const std::vector<std::string> &parents);

private:
    double alpha_;
};

}

#endif