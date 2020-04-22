#include "src/C++/AUI_graph.h"
#include "src/C++/CTP_graph.h"
#include "src/C++/NEXI_graph.h"

void AUI() {
    std::cout << "==========================AUI==========================\n";
    AUI_graph net;
    // Insert vertexes
    net.load_nodes(AUI_NODES, "NDG");
    // Insert edges
    net.load_edges(AUI_EDGES, "NDG_ORIGINE",
                   "NDG_DESTINAZIONE");
}

void CTP() {
    std::cout << "==========================CTP==========================\n";
    CTP_graph net;
    // Insert vertexes
    net.load_nodes(CTP_NODES, "CD_NDG");
    // Insert edges
    net.load_edges(CTP_EDGES, "ID_NODO_ORIGINE",
                   "ID_NODO_DESTINAZIONE");
}

void NEXI() {
    std::cout << "==========================NEXI==========================\n";
    NEXI_graph net;
    // Insert vertexes
    net.load_nodes(NEXI_NODES, "ID_NODO");
    // Insert edges
    net.load_edges(NEXI_EDGES, "ID_NODO_ORIGINE",
                   "ID_NODO_DESTINAZIONE");
}
int main() {
    AUI();
    CTP();
    NEXI();
    return 0;
}