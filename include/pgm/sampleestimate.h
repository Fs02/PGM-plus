#ifndef PGM_SAMPLEESTIMATE_H
#define PGM_SAMPLEESTIMATE_H

namespace pgm {

class Bayesnet;
class Dataset;

//! Estimate parameter of Bayesnet given Dataset
/*!
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
        pgm::SampleEstimate estimate(0.0);
        estimate(bn, dataset);
    }

    \endcode
*/
class SampleEstimate
{
public:
    //! Construct a Sample estimate
    /*!
        \param alpha Alpha = 0 will create a Maximum Likelihood Estimation
    */
    SampleEstimate(double alpha = 0.5);

    //! Estimate parameter of Bayesnet giben Dataset
    /*!
        \param bayesnet Bayesnet
        \param dataset Dataset
    */
    void operator ()(Bayesnet &bayesnet, const Dataset &dataset);

private:
    double alpha_;
};

}

#endif