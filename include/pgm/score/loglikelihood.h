#ifndef PGM_LOGLIKELIHOOD_H
#define PGM_LOGLIKELIHOOD_H

#include <pgm/score/score.h>
#include <pgm/frequency.h>

namespace pgm {

//! Log Likelihood scoring function
/*!
    Equation:
    \f{eqnarray*}{
        LL(B|T) &=& \sum_{i=1}^{n} \sum_{j=1}^{q_i} \sum_{k=1}^{r_i} N_{ijk} \log(\frac{N_{ijk}}{N_{ij}})
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
        pgm::LogLikelihood score(dataset);
        score(bn);
        score.score("A", {"B"});
    }

    \endcode
*/
class LogLikelihood : public Score
{
public:
    //! Default constructor
    /*!
        \param dataset Dataset
    */
    LogLikelihood(const Dataset &dataset);

    //! Compute decomposed score
    /*!
        \param child Child node
        \param parents Parents nodes
        \return decomposed score
    */
    virtual double score(const std::string &child, const std::vector<std::string> &parents);

    //! Compute decomposed score with existing frequency count
    /*!
        \param count Frequency count
        \param child Child node
        \param parents Parents nodes
        \return decomposed score
    */
    double score(const Frequency &count, const std::string &child, const std::vector<std::string> &parents);
};

}

#endif