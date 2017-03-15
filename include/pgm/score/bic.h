#ifndef PGM_BIC_H
#define PGM_BIC_H

#include <pgm/score/loglikelihood.h>

namespace pgm {

class Bic : public LogLikelihood
{
public:
    Bic(const Dataset &dataset);

    double score(const std::string &child, const std::vector<std::string> &parents);
};

}

#endif