#ifndef AG2_GRAPHWIZ_H
#define AG2_GRAPHWIZ_H
#include <fstream>

void dumpToGraphviz(std::ostream & f, const Map & map) {
    f << "digraph D {" << std::endl;

    for (const Place & place : map.places) {
        f << '"' << place << '"' << "[shape=circle]" << std::endl;
    }

    for (const auto & [from, to, capacity] : map.connections) {
        f << '"' << from << '"' << " -> " << '"' << to << '"' << "[weight="<< capacity << "]"<< std::endl;
    }

    f << "}" << std::endl;
}

void dumpToGraphviz(const Map & map) {
    std::ofstream f ("graph.dot");
    dumpToGraphviz(f, map);
    f.close();
}

#endif //AG2_GRAPHWIZ_H
