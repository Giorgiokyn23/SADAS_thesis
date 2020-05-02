//
// Created by Gemma Martini on 24/04/2020.
//

#include "utils.h"


template <typename id_type>

void print_orange(std::array<std::pair<long long int,
        std::vector<id_type>>, 2> a, id_type t1, id_type t2) {
    std::cout << "=====From " << t1 << " to "
              << t2 << "=====\n";
    std::cout << a[0].first << ": ";
    for(auto t=a[0].second.begin(); t!=a[0].second.end(); ++t)
        std::cout << *t << " ";
    std::cout << std::endl;
    std::cout << "=====From " << t2 << " to "
              << t1 << "=====\n";
    std::cout << a[1].first << ": ";
    for(auto t=a[1].second.begin(); t!=a[1].second.end(); ++t)
        std::cout << *t << " ";
    std::cout << std::endl;
}

int main() {
    // Testing orange on AUI graph
    AUI_graph my_AUI = load_AUI();
    std::string target1 = "0000000000001259";
    std::string target2 = "0000000000001314";
    print_orange(my_AUI.orange(target1, target2), target1, target2);

    // Testing blue on CTP graph
    CTP_graph my_CTP = load_CTP();
    long long int t1 = 1;
    long long int t2 = 16;
    print_orange(my_CTP.orange(t1, t2), t1, t2);

    // Testing blue on NEXI graph
    NEXI_graph my_NEXI = load_NEXI();
    t1 = 26285;
    t2 = 3297;
    print_orange(my_NEXI.orange(t1, t2), t1, t2);
    return 0;
}