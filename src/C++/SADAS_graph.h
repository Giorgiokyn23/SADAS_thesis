//
// Created by Gemma Martini on 21/04/2020.
//
#pragma once

#include <map>
#include <array>
#include <unordered_set>

#include "read_utilities.hpp"

#ifndef SADAS_GDB_SADAS_GRAPH_H
#define SADAS_GDB_SADAS_GRAPH_H

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
     * This method provides the (outgoing) neighbours of a certain node
     * @param id the primary key representing such node
     * @return the vector of the primary keys identifying the neighbours
     */
    std::unordered_set<id_type> neighs(id_type id) {
        auto it = g.find(id);
        if (it != g.end())
            return it->first;
        return std::unordered_set<id_type>();
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
};


#endif //SADAS_GDB_SADAS_GRAPH_H
