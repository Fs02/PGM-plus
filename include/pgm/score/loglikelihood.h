#ifndef PGM_LOGLIKELIHOOD_H
#define PGM_LOGLIKELIHOOD_H

#include <pgm/score/score.h>
#include <pgm/frequency.h>

namespace pgm {

class LogLikelihood : public Score
{
public:
    LogLikelihood(const Dataset &dataset);

    virtual double score(const std::string &child, const std::vector<std::string> &parents);
    double score(const Frequency &count, const std::string &child, const std::vector<std::string> &parents);
};

}

#endif