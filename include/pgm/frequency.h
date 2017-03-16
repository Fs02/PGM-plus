#ifndef PGM_FREQUENCY_H
#define PGM_FREQUENCY_H

#include <unordered_map>
#include <pgm/variable.h>

namespace pgm {

class Dataset;

//! Utility to count occurence of data in dataset
/*!
    This is used internally in scoring function and sample estimator to count occurence
    of some data in dataset.

    usage example:
    \code

    #include <pgm/dataset.h>
    #include <pgm/frequency.h>

    int main()
    {
        // Assume the following dataset
        pgm::Dataset dataset;
        dataset.push({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "yes"}}); 
        dataset.push({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "no"}});
        dataset.push({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "yes"}});
        dataset.push({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "yes"}});
        dataset.push({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "no"}});

        // Counting occurences for two variables
        pgm::Frequency count(dataset, {"outlook", "temperature"});

        // Occurences
        count({}); // is 5
        count({{"outlook", "sunny"}}); // is 3
        count({{"outlook", "rain"}}); // is 2
        count({{"temperature", "hot"}}); // is 2
        count({{"temperature", "cool"}}); // is 2
        count({{"temperature", "mild"}}); // is 1

        count({{"outlook", "sunny"}, {"temperature", "hot"}}); // is 1
        count({{"outlook", "sunny"}, {"temperature", "cool"}}); // is 1
        count({{"outlook", "sunny"}, {"temperature", "mild"}}); // is 1
        count({{"outlook", "rain"}, {"temperature", "hot"}}); // is 1
        count({{"outlook", "rain"}, {"temperature", "cool"}}); // is 1
        count({{"outlook", "rain"}, {"temperature", "mild"}}); // is 0
    }

    \endcode

*/
class Frequency
{
public:
    //! Default constructor
    /*!
        \param dataset Dataset
        \param variables Variables to observe
    */
    Frequency(const Dataset &dataset, const std::vector<std::string> &variables);

    //! Get count of occurences
    /*!
        \param vars Unordered map of variable and it's expected value
        \return Number of times vars happening
    */
    std::size_t operator ()(const std::unordered_map<std::string, std::string> &vars) const;

    //! Permutate value of variables
    /*!
        \param variables Variables to permutate
        \return Permutation value of each variables
    */
    std::vector<std::vector<std::string>> permutate(const std::vector<std::string> &variables) const;

    //! Get variable by name
    /*!
        \param name Variable name
        \return Variable
    */
    inline const Variable &variable(const std::string &name) const { return variables_.at(index_.at(name)); }

    //! Get variable by name
    /*!
        \param name Variable name
        \return Variable
    */
    inline Variable variable(const std::string &name) { return variables_.at(index_.at(name)); }

private:
    std::unordered_map<std::string, std::size_t> index_;
    std::vector<Variable> variables_;
    std::vector<std::size_t> count_;
};

}

#endif