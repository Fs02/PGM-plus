#ifndef PGM_UTILITY_H
#define PGM_UTILITY_H

#include <string>

namespace pgm {

class Bayesnet;

bool write_dot(const Bayesnet &bayesnet, const std::string &path);

}

#endif