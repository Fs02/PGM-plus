#include <pgm/utility.h>
#include <pgm/bayesnet.h>
#include <fstream>

bool pgm::write_dot(const Bayesnet &bayesnet, const std::string &path)
{
    std::ofstream out(path);
    out << "digraph {\n";

    // print out all labels
    for (auto v : bayesnet.graph().vertices())
    {
        out << "\t"<< v << " [label=\"" << bayesnet.variable(v).name() << "\"];\n";
    }
    out << "\n";

    // print out all labels as adjacency lists
    for (auto v : bayesnet.graph().vertices())
    {
        const auto adjacents = bayesnet.graph().adjacents(v);
        out << "\t"<< v << " -> {";
        for (std::size_t i = 0; i < adjacents.size(); ++i)
        {
            out << adjacents[i];
            if (i < adjacents.size() - 1) out << " ; ";
        }
        out << "} [dir=back]\n";
    }

    out << "}";
}
