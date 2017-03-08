#ifndef PGM_FCLL_H
#define PGM_FCLL_H

#include <pgm/score/loglikelihood.h>

namespace pgm {

class Fcll : public LogLikelihood
{
public:
    Fcll(const Dataset &dataset, const std::string &class_node);

    double score(const std::string &child, const std::vector<std::string> &parents);

private:
    const std::string class_node_;
};

}

#endif