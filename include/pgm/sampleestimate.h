#ifndef PGM_SAMPLEESTIMATE_H
#define PGM_SAMPLEESTIMATE_H

namespace pgm {

class Bayesnet;
class Dataset;

class SampleEstimate
{
public:
    SampleEstimate(double alpha = 0.5);

    void operator ()(Bayesnet &bayesnet, const Dataset &dataset);

private:
    double alpha_;
};

}

#endif