//
// Created by Gemma Martini on 24/04/2020.
//

#include "utils.h"

template <typename id_type>
void print_blue(std::map<id_type, int> b) {
    for (auto i = b.begin(); i != b.end(); ++i) {
        std::cout << "Node: " << (*i).first << "; Distance: " << (*i).second << std::endl;
    }
}

int main() {
    // Testing blue on AUI graph
    AUI_graph my_AUI = load_AUI();
    std::string target = "0000000000000240";
    int n = 1;
    std::cout << "The nodes that are reachable from " << target
              << " in a radius of " << n << " are:\n";
    print_blue(my_AUI.blue(target, n));

    // Testing blue on CTP graph
    CTP_graph my_CTP = load_CTP();
    long long int t = 1;
    n = 4;
    std::cout << "The nodes that are reachable from " << t
              << " in a radius of " << n << " are:\n";
    print_blue(my_CTP.blue(t, n));

    // Testing blue on NEXI graph
    NEXI_graph my_NEXI = load_NEXI();
    t = 26285;
    n = 2;
    std::cout << "The nodes that are reachable from " << t
              << " in a radius of " << n << " are:\n";
    print_blue(my_NEXI.blue(t, n));
    return 0;
}