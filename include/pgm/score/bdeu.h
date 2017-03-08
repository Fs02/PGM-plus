#ifndef PGM_BDEU_H
#define PGM_BDEU_H

#include <pgm/score/score.h>

namespace pgm {

class Bdeu : public Score
{
public:
    Bdeu(const Dataset &dataset, double aplha = 0.5);

    double score(const std::string &child, const std::vector<std::string> &parents);

private:
    double alpha_;
};

}

#endif