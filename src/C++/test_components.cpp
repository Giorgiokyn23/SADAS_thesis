//
// Created by Gemma Martini on 22/04/2020.
//

#include "utils.h"



int main() {
    AUI_graph my_AUI = load_AUI();
    CTP_graph my_CTP = load_CTP();
    NEXI_graph my_NEXI = load_NEXI();
    std::cout << "=============AUI connected components=============\n";
    my_AUI.print_ccs(my_AUI.connected_components());
    std::cout << "=============AUI strongly connected components=============\n";
    my_AUI.print_ccs(my_AUI.strongly_connected_components());
    std::cout << "=============CTP strongly connected components=============\n";
    my_CTP.print_ccs(my_CTP.strongly_connected_components());
    std::cout << "=============NEXI strongly connected components=============\n";
    my_NEXI.print_ccs(my_NEXI.strongly_connected_components());
    my_NEXI.scc_size_freqs(my_NEXI.strongly_connected_components(),
            "../scc_sizes.csv");
    return 0;
}