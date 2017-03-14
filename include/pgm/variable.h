#ifndef PGM_VARIABLE_H
#define PGM_VARIABLE_H

#include <vector>
#include <string>
#include <iostream>

namespace pgm {

//! Discrete Random Variable
/*!
    Represents a discrete variables
*/
class Variable
{
public:
    //! Construct with name
    /*!
        \param name Name of variable
    */
    explicit Variable(const std::string &name);

    //! Construct with name and states
    /*!
        \param name Name of variable
        \param states Initial states
    */
    Variable(const std::string &name, const std::vector<std::string> &states);

    //! Get state id
    /*!
        Will register state if not exists yet
        \param state State name
        \return State id
    */
    std::size_t operator() (const std::string &state);

    //! Get state id
    /*!
        \param state State name
        \return State id
    */
    std::size_t operator() (const std::string &state) const;

    //! Get state name
    /*!
        \param id State id
        \return State name
    */
    std::string operator() (std::size_t id);

    //! Get state name
    /*!
        \param id State id
        \return State name
    */
    std::string operator() (std::size_t id) const;

    //! Get variable name
    /*!
        \return Variable name
    */
    inline std::string name() const { return name_; }

    //! Get all states
    /*!
        \return All states
    */
    inline const std::vector<std::string> &states() const { return states_; }

    //! Get all states
    /*!
        \return All states
    */
    inline std::vector<std::string> states() { return states_; }

    //! Get number of states
    /*!
        \return Number of defined states
    */
    inline std::size_t cardinality() const { return states_.size(); }

    friend std::ostream &operator <<(std::ostream &os, const Variable &v);

private:
    std::string name_;
    std::vector<std::string> states_;
};

std::ostream &operator <<(std::ostream &os, const Variable &v);

}

#endif