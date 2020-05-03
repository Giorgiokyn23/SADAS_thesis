//
// Created by Gemma Martini on 21/04/2020.
//
#pragma once

#include <map>
#include <array>
#include <queue>
#include <stack>
#include <unordered_set>

#include "read_utilities.hpp"

#ifndef SADAS_GDB_SADAS_GRAPH_H
#define SADAS_GDB_SADAS_GRAPH_H

// Redefinition of data structures functional to this graph implementation
template <typename id_type, typename node_t>
using id2nd_t = std::map<id_type, node_t>;

template <typename id_type>
using couple = std::pair<id_type, id_type>;

template <typename id_type, typename edge_t>
using id2edg_t = std::map<couple<id_type>, std::vector<edge_t>>;

template <typename id_type>
using adjl_t = std::map<id_type, std::unordered_set<id_type>>;

template <typename id_type, typename node_t, typename edge_t>
class SADAS_graph {
protected:
    id2nd_t<id_type, node_t> nodes_map; // id -> node_t
    id2edg_t<id_type, edge_t> edges_map;// (id, id) -> vector of edge_t
    adjl_t<id_type> g;// (id) -> set of ids
    adjl_t<id_type> unoriented_g;// (id) -> set of ids
    /**
     * PURE ABSTRACT METHOD for loading a line from the csv file containing
     * nodes into the nodes' map of the graph
     * @param line row of csv file
     */
    virtual void add_vtx(CSVIterator line) = 0;

    /**
     * PURE ABSTRACT METHOD for obtaining the keys for both origin and
     * destination nodes from a line of the edges' csv file
     * @param line row of csv file
     * @return the "couple" of id values
     */
    virtual std::array<id_type, 2> line2id(CSVIterator line) = 0;

    /**
     * PURE ABSTRACT METHOD for loading a line from the edges' file
     * containing edges into the edges' map of the graph
     * @param line row of csv file
     */
    virtual void line2edg(CSVIterator line) = 0;

    /**
     * Utility method for implementing orange query
     * @param from id of starting node
     * @param to id of destination node
     * @return a couple <distance, path> from from node towards to node
     */
    std::pair<long long int, std::vector<id_type>>
    orange_utils(id_type from, id_type to) {
        std::queue<id_type> q; // Queue of visited nodes of which exploring neighs
        std::map<id_type, long long int> visited; // Stores distances
        std::map <id_type, id_type> predecessor;
        q.push(from);
        visited[from] = 0;
        // No predecessor of source (left unset)
        bool brk = false;
        while (!q.empty() && !brk) {
            id_type curr_node = q.front();
            q.pop();
            // Visit all the unvisited neighbours of current node
            std::unordered_set<id_type> neighs = neighbours(curr_node);
            for (auto neigh = neighs.begin(); neigh != neighs.end(); ++neigh) {
                // If neigh has not been visited yet visit it
                if (visited.find((*neigh)) == visited.end()) {
                    visited[*neigh] = visited[curr_node] + 1;
                    predecessor[*neigh] = curr_node;
                    q.push(*neigh);
                    // If dest has been reached a shortest path was found
                    if (*neigh == to) {
                        brk = true;
                        break;
                    }
                }
            }
        }
        // If target was not reached
        if (visited.find(to) == visited.end()) {
            return std::make_pair(-1, std::vector<id_type>());
        }
        // Else store in path the shortest path
        std::vector<id_type> path;
        id_type crawl = to;
        path.push_back(crawl);
        while (predecessor.find(crawl) != predecessor.end()) {
            path.insert(path.begin(), predecessor[crawl]);
            crawl = predecessor[crawl];
        }
        return std::make_pair(visited[to], path);
    }

    /**
     * This method creates a new unoriented graph preserving the frontology
     * and stores the new adjacency list in the field unoriented_g of the class
     *
     */
    void make_unoriented() {
        for (auto node: g) {
            for (auto neigh = node.second.begin(); neigh != node.second.end();
                 ++neigh) {
                unoriented_g[node.first].insert(*neigh);
                unoriented_g[*neigh].insert(node.first);
            }
        }
        /*for (auto n: unoriented_g) {
            std::cout << "\nNeighs of " << n.first << ":\n";
            for (auto ng = n.second.begin(); ng != n.second.end(); ++ng) {
                std::cout << *ng << " ";
            }
        }*/
    }

    /**
     * This method finds the neighbours of a node in the undirected version
     * of the graph
     * @param id identifier of target node
     * @return the set of ids of (undirected) neighbours
     */
    std::unordered_set<id_type> u_neighbours(id_type id) {
        auto it = unoriented_g.find(id);
        if (it != unoriented_g.end())
            return it->second;
        return std::unordered_set<id_type>();
    }

    /**
     * This method implements the utility function for performing a recursive
     * DFS on the graph for the computation of connected components
     * @param node id of starting node
     * @param visited reference of a set of identifiers representing the
     *      visited nodes
     * @param cc reference of a set of identifiers representing the
     *      current connected component
     */
    void DFS_utils(id_type node, std::unordered_set<id_type> &visited,
            std::unordered_set<id_type> &cc) {
        // Mark the current node as visited and add it to set of ccs
        visited.insert(node);
        cc.insert(node);

        // Recur for all the vertices adjacent to this vertex
        std::unordered_set<id_type> neighs = u_neighbours(node);
        for (auto neigh = neighs.begin(); neigh != neighs.end(); ++neigh) {
            // If neigh has not been visited yet
            if (visited.find((*neigh)) == visited.end()) {
                DFS_utils(*neigh, visited, cc);
            }
        }
    }

    /**
     * This method implements the utility function for performing a recursive
     * DFS on the transposed graph for the computation of
     * strongly connected components
     * @param gt adjacency list of transposed graph
     * @param node id of starting node
     * @param visited  reference of a set of identifiers representing the
     *      visited nodes
     * @param scc reference of a set of identifiers representing the
     *      current strongly connected component
     */
    void DFS_utils_scc(adjl_t<id_type> gt, id_type node,
            std::unordered_set<id_type> &visited,
            std::unordered_set<id_type> &scc) {
        // Mark the current node as visited and add it to set of ccs
        visited.insert(node);
        scc.insert(node);

        // Recur for all the vertices adjacent to this vertex
        std::unordered_set<id_type> neighs = gt[node];
        for (auto neigh = neighs.begin(); neigh != neighs.end(); ++neigh) {
            // If neigh has not been visited yet
            if (visited.find((*neigh)) == visited.end()) {
                DFS_utils_scc(gt, *neigh, visited, scc);
            }
        }
    }

    /**
     * This method implements the utility function "fill order" of Kosaraju's
     * algorithm for computing the strongly connected components of a graph
     * @param node the starting node
     * @param a reference to the visited a set of visited nodes (ids)
     * @param s reference to the stack
     */
    void fill_order(id_type node, std::unordered_set<id_type> &visited,
            std::stack<id_type> &s) {
        visited.insert(node);

        // Recur for all the vertices adjacent to this vertex
        std::unordered_set<id_type> neighs = neighbours(node);
        for (auto neigh = neighs.begin(); neigh != neighs.end(); ++neigh) {
            // If neigh has not been visited yet
            if (visited.find((*neigh)) == visited.end()) {
                fill_order(*neigh, visited, s);
            }
        }
        s.push(node);
    }

    /**
     * This private method is used to obtain the adjacency list of the
     * transposed graph
     * @return the adjacency list of the transposed graph
     */
    adjl_t<id_type> get_transpose() {
        adjl_t<id_type> gt;
        // For each node

        for (auto n: g) {
            // For each node in the transpose graph, print neighs
            for (auto neigh = n.second.begin(); neigh != n.second.end(); ++neigh) {
                gt[*neigh].insert(n.first);
            }
        }
        /*for (auto n: gt) {
            // For each neighbour add reverse edge
            std::cout << "Vicini di " << n.first << ": ";
            for (auto neigh = n.second.begin(); neigh != n.second.end(); ++neigh) {
                std::cout << *neigh << " ";
            }
            std::cout << std::endl;
        }*/
        return gt;
    }


public:

    /**
     * This method provides the number of nodes in the network
     * @return the number of nodes
     */
    long long int num_nodes() {
        return nodes_map.size();
    }

    /**
     * This method provides the number of edges in the network
     * @return the number of edges
     */
    long long int num_edges() {
        long long int i = 0;
        for ( auto it = edges_map.begin(); it != edges_map.end(); ++it) {
            i += it->second.size();
        }
        return i;
    }

    /**
     * This method loads the nodes (contained in the path) into memory
     * @param path  of the file containing the nodes
     * @param id_name name of the key of the node
     */
    void load_nodes(std::string path, std::string id_name) {
        // Print number of rows in file
        std::cout << "Number of vertices in file: "<< num_lines(path) - 1
            <<"\n";
        std::ifstream nodes_file(path);
        //long long int line_no = 0;
        // For each line in nodes file add vertex
        for (CSVIterator loop(nodes_file); loop != CSVIterator(); ++loop) {
            //std::clog << "Line number: " << ++line_no << std::endl;
            // Discard header
            if ((*loop)[0] == id_name)
                continue;
            //Add file row to nodes data structure
            add_vtx(loop);
        }
        std::cout << "Number of vertices inserted: "
            << num_nodes() << "\n";
        return;
    }

    /**
     * This method loads into memory the edges contained in the path
     * OBS: a check of path between non existing nodes is performed and such
     * edges are not inserted
     *
     * @param path string containing the path to the edges' file
     * @param id_name_ori string representing the name of the field
     *      containing the origin node
     * @param id_name_dest string representing the name of the field
     *      containing the destination node
     */
    void load_edges(std::string path, std::string id_name_ori,
                        std::string id_name_dest) {
        std::cout << "Number of edges in file: " << num_lines(path) - 1 << "\n";
        std::ifstream edges_file(path);
        long long int line_no = 0;
        int num_inv_edges = 0; //counter for invalid edges
        for (CSVIterator loop(edges_file); loop != CSVIterator(); ++loop) {
            // Skip header
            if ((*loop)[0] == id_name_ori)
                continue;
            std::clog << "Line number: " << ++line_no << std::endl;
            std::array<id_type, 2> ori_dest = line2id(loop);
            auto ori = nodes_map.find(ori_dest[0]);
            auto dest = nodes_map.find(ori_dest[1]);
            // If edge is invalid go to next line
            if (ori == nodes_map.end() || dest == nodes_map.end()) {
                std::clog << "Invalid edge: " << ori_dest[0]
                    << " -> " << ori_dest[1] << "\n";
                num_inv_edges++;
                continue;
            }
            // If edge is valid insert it into adjacency list
            // If origin node does not already have a link to dest
            // insert dest in the adjacency list of ori
            if (g.find(ori_dest[0]) == g.end() ||
                g[ori_dest[0]].find(ori_dest[1]) == g[ori_dest[0]].end()) {
                g[ori_dest[0]].insert(ori_dest[1]);
            }
            // And add file row to edge data structure
            line2edg(loop);
        }
        std::cout << "Number of invalid edges: " << num_inv_edges << "\n";
        std::cout << "Number of edges inserted: " << num_edges() << "\n";
    }

    /**
     * This method provides the (outgoing) neighbours of a certain node
     * @param id the primary key representing such node
     * @return the vector of the primary keys identifying the neighbours
     */
    std::unordered_set<id_type> neighbours(id_type id) {
        auto it = g.find(id);
        if (it != g.end())
            return it->second;
        return std::unordered_set<id_type>();
    }

    /**
     * This method implements the blue query
     * -->visualization of a subset of the graph<--
     * Given a subject, extract the links up to n levels, i.e. those nodes
     * which are reachable percolating paths made of up to n edges
     * @param target the id of the node from which starting the search
     * @param n the maximum depth of the search
     * @return map from the key of the node to the distance from the target
     */
    std::map<id_type, int> blue(id_type target, int n) {
        std::queue<std::pair<id_type, int>> q; // Queue of <node, distance>
        std::map<id_type, int> visited; // Map from node to distance
        std::pair<id_type, int> my_couple;
        my_couple.first = target;
        my_couple.second = 0;
        q.push(my_couple);
        while(!q.empty()) {
            id_type curr_node = q.front().first;
            int curr_dist = q.front().second;
            std::clog << "node: " << curr_node << " dist: " << curr_dist
                << std::endl;
            q.pop();
            // If the current node is too far break
            if (curr_dist > n) break;
            // curr_node might have already been visited (put in q before visit)
            if (visited.find(curr_node) != visited.end())
                continue;
            // If the current node has not been visited visit it
            visited[curr_node] = curr_dist;
            // And add all the neighbours of current node to queue
            std::unordered_set<id_type> neighs = neighbours(curr_node);
            for (auto neigh = neighs.begin(); neigh != neighs.end(); ++neigh) {
                // If neigh has not been visited yet
                if (visited.find((*neigh)) == visited.end()) {
                    std::clog << "Curr_dist: " << curr_dist << " Neigh: "
                              << (*neigh) <<std::endl;
                    my_couple.first = (*neigh);
                    my_couple.second = curr_dist + 1;
                    q.push(my_couple);
                }
            }
        }
        return visited;
    }

    /**
     * This method implements orange query
     * -->connections between subjects<--
     * Given two subjects find the shortest path between them
     * Since the graph is oriented this method returns
     * @param t1 the first subject
     * @param t2 the second subject
     *
     * @return the array containing the two paths: from (to) t1 to (from) t2
     */
    std::array<std::pair<long long int, std::vector<id_type>>, 2> orange(
            id_type t1, id_type t2) {
        std::array<std::pair<long long int, std::vector<id_type>>, 2> ans;
        std::pair<long long int, std::vector<id_type>> path12;
        std::pair<long long int, std::vector<id_type>> path21;
        path12 = orange_utils(t1, t2);
        path21 = orange_utils(t2, t1);
        ans[0] = path12;
        ans[1] = path21;
        return ans;
    }

    /**
     * Method implementing green query
     * --> connections with suspects <<--
     * Given a subject and a set of nodes, extract those at a distance
     * smaller than n from it
     * @param target the id of the starting node
     * @param n the maximum distance
     * @param suspects set of suspects
     * @return a map from node id to distance
     */
    std::map<id_type, int> green(id_type target, int n,
            std::unordered_set<id_type> suspects) {
        std::queue<std::pair<id_type, int>> q; // Queue of <node, distance>
        std::map<id_type, int> visited; // Map from node to distance
        std::pair<id_type, int> my_couple;
        my_couple.first = target;
        my_couple.second = 0;
        q.push(my_couple);
        while(!q.empty()) {
            id_type curr_node = q.front().first;
            int curr_dist = q.front().second;
            std::clog << "node: " << curr_node << " dist: " << curr_dist
                      << std::endl;
            q.pop();
            // If the current node is too far break
            if (curr_dist > n) break;
            // curr_node might has already been visited (put in q before
            // being visited)
            if (visited.find(curr_node) != visited.end())
                continue;
            // If the current node has not been visited visit it
            visited[curr_node] = curr_dist;
            // And add all the neighbours of current node to queue
            std::unordered_set<id_type> neighs = neighbours(curr_node);
            for (auto neigh = neighs.begin(); neigh != neighs.end(); ++neigh) {
                // If neigh is not in the suspects list go to next neighbour
                if (suspects.find((*neigh)) == suspects.end())
                    continue;
                // If neigh has not been visited yet
                if (visited.find((*neigh)) == visited.end()) {
                    std::clog << "Curr_dist: " << curr_dist << " Neigh: "
                              << (*neigh) <<std::endl;
                    my_couple.first = (*neigh);
                    my_couple.second = curr_dist + 1;
                    q.push(my_couple);
                }
            }
        }
        return visited;
    }

    /**
     * This method prints all the (strongly) connected components given as input
     * @param ccs the vector of connected components
     */
    void print_ccs(std::vector<std::unordered_set<id_type>> ccs) {
        for (auto cc = ccs.begin(); cc != ccs.end(); ++cc) {
            std::cout << "New connected component:\n";
            for (auto node = cc->begin(); node != cc->end(); ++node) {
                std::cout << *node << " " << std::endl;
            }
        }
    }

    /**
     * This method computes the connected components of the graph, seen as an
     * undirected one
     * @return a vector of sets of node ids (the ccs)
     */
    std::vector<std::unordered_set<id_type>> connected_components() {
        //Make graph unoriented
        make_unoriented();
        // Create empty set for visited nodes
        std::unordered_set<id_type> visited;
        // Create vector of connected components
        std::vector<std::unordered_set<id_type>> ccs;
        for (auto n: unoriented_g) {
            std::clog << "Current node: " << n.first << std::endl;
            if (visited.find((n.first)) == visited.end()) {
                // Find its connected component
                std::unordered_set<id_type> cc;
                DFS_utils(n.first, visited, cc);
                std::clog << "Taglia componente connessa: "
                << cc.size() << std::endl;
                ccs.push_back(cc);
            }
        }
        return ccs;
    }

    /**
     * This method exports a csv file containing the sizes
     * of the strongly connected components given as input
     * @param sccs vector of strongly connected components
     * @param path path of the file where to store the frequencies
     */
    void scc_size_freqs (std::vector<std::unordered_set<id_type>> sccs,
            std::string path) {
        std::ofstream out_file;
        out_file.open (path);
        out_file << "Size" << std::endl;
        for (auto scc = sccs.begin(); scc != sccs.end(); ++scc) {
            out_file << (*scc).size() << std::endl;
        }

        out_file.close();
    }

    /**
     * This method computes the strongly connected components of the graph,
     * using Kosaraju's algorithm
     * @return a vector of sets of node ids (the sccs)
     */
    std::vector<std::unordered_set<id_type>> strongly_connected_components()
    {
        // Create empty set for visited nodes
        std::unordered_set<id_type> visited;
        // Create vector of strongly connected components
        std::vector<std::unordered_set<id_type>> sccs;

        std::stack<id_type> stack;

        // Fill vertices in stack according to their finishing times
        for (auto n: g) {
            if (visited.find(n.first) == visited.end()) {
                fill_order(n.first, visited, stack);
            }
        }

        // Create a reversed graph
        adjl_t<id_type> gt = get_transpose();

        // Clear the set of visited nodes for second DFS
        visited.clear();

        std::clog << "Printing stack: ";
        std::unordered_set<id_type> scc;
        // Now process all vertices in order defined by the stack
        while (stack.empty() == false){
            // Pop a vertex from stack
            id_type v = stack.top();
            std::clog << v << " ";
            stack.pop();

            // Print Strongly connected component of the popped vertex
            if (visited.find(v) == visited.end()) {
                DFS_utils_scc(gt, v, visited, scc);
                std::clog << "Taglia componente connessa: "
                          << scc.size() << std::endl;
                sccs.push_back(scc);
                scc.clear();
            }
        }
        return sccs;
    }
};




#endif //SADAS_GDB_SADAS_GRAPH_H
