#ifndef PGM_AIC_H
#define PGM_AIC_H

#include <pgm/score/loglikelihood.h>

namespace pgm {

class Aic : public LogLikelihood
{
public:
    Aic(const Dataset &dataset);

    double score(const std::string &child, const std::vector<std::string> &parents);
};

}

#endif