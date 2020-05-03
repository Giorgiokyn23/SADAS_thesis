//
// Created by Gemma Martini on 22/04/2020.
//

#include "utils.h"



int main() {
    AUI_graph my_AUI = load_AUI();
    //CTP_graph my_CTP = load_CTP();
    //NEXI_graph my_NEXI = load_NEXI();
    //std::vector<std::unordered_set<std::string>> a
    my_AUI.print_ccs(my_AUI.connected_components());
    return 0;
}