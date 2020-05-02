//
// Created by Gemma Martini on 24/04/2020.
//

#include "utils.h"


template <typename id_type>
void print_green(std::map<id_type, int> a, id_type t) {
    for (auto i = a.begin(); i != a.end(); ++i) {
        std::cout << "Node: " << (*i).first << "; Distance: " << (*i).second << std::endl;
    }
}

int main() {
    // Testing orange on AUI graph
    AUI_graph my_AUI = load_AUI();
    std::string subj = "0000000000001259";
    int n = 2;
    std::unordered_set<std::string> suspects;
    suspects.insert("0000000000001259");
    suspects.insert("0000000000001261");
    suspects.insert("0000000000001262");
    suspects.insert("0000000000001314");
    std::map<std::string, int> ans = my_AUI.green(subj, n, suspects);
    print_green(ans, subj);

    // Testing blue on CTP graph
    CTP_graph my_CTP = load_CTP();
    long long int sub = 1;
    n = 2;
    std::unordered_set<long long int> susp;
    susp.insert(1);
    susp.insert(11);
    susp.insert(12);
    susp.insert(15);
    std::map<long long int, int> an = my_CTP.green(sub, n, susp);
    print_green(an, sub);

    // Testing blue on NEXI graph
    NEXI_graph my_NEXI = load_NEXI();
    sub = 26285;
    n = 2;
    std::unordered_set<long long int> susps;
    susps.insert(26285);
    susps.insert(3297);
    susps.insert(6924);
    susps.insert(2106);
    an = my_NEXI.green(sub, n, susps);
    print_green(an, sub);
    return 0;
}