//
// Created by Gemma Martini on 21/04/2020.
//
#pragma once

#include <map>
#include <array>
#include <queue>
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
//


template <typename id_type, typename node_t, typename edge_t>
class SADAS_graph {
protected:
    id2nd_t<id_type, node_t> nodes_map; // id -> node_t
    id2edg_t<id_type, edge_t> edges_map;// (id, id) -> vector of edge_t
    adjl_t<id_type> g;// (id) -> set of id
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

    void green() {


    }
};




#endif //SADAS_GDB_SADAS_GRAPH_H
