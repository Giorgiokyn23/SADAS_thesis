//
// Created by Gemma Martini on 22/04/2020.
//

#include "SADAS_graph.h"

#ifndef SADAS_GDB_CTP_GRAPH_H
#define SADAS_GDB_CTP_GRAPH_H

struct CTP_node {
    long long int CD_NDG;
    std::string CD_FORMA_GIURIDICA;
};

struct CTP_edge {
    long long int ID_NODO_ORIGINE;
    long long int ID_NODO_DESTINAZIONE;
    std::string CODICE_LEGAME;
    std::string DS_LEGAME;
};

class CTP_graph : public SADAS_graph<long long int, CTP_node, CTP_edge> {
protected:

    /**
     * This method loads a line from the csv file containing nodes into the
     * nodes' map of the graph
     * @param line row of csv file
     */
    virtual void add_vtx(CSVIterator line) override {
        // std::clog << (*line).size() << "\n";
        long long int id = std::stoll((*line)[0]);
        CTP_node my_node;
        my_node.CD_NDG = id;
        my_node.CD_FORMA_GIURIDICA = (*line)[1];
        // Add (id, node) to map
        nodes_map[id] = my_node;
    }

    /** This method returns the keys of both origin and
    * destination nodes from a line of the edges' csv file
    * @param line row of csv file
    * @return the "couple" of id values
    */
    virtual std::array<long long int, 2> line2id(CSVIterator line) override {
        std::array<long long int, 2> from_to;
        from_to[0] = std::stoll((*line)[0]);
        from_to[1] = std::stoll((*line)[1]);
        return from_to;
    }

    /**
     * This method loads a line from the edges' file
     * into the edges' map of the graph
     * @param line row of csv file
     */
     virtual void line2edg(CSVIterator line) override {
        //std::clog << "Number of fields of line: " << (*line).size() << "\n";
        CTP_edge my_edge;
        long long int ori_id = std::stoll((*line)[0]);
        long long int dest_id = std::stoll((*line)[1]);
        // Fill fields of node ds
        my_edge.ID_NODO_ORIGINE = ori_id;
        my_edge.ID_NODO_DESTINAZIONE = dest_id;
        my_edge.CODICE_LEGAME = (*line)[2];
        my_edge.DS_LEGAME = (*line)[3];
        couple<long long int> my_edge_key;
        my_edge_key.first = ori_id;
        my_edge_key.second = ori_id;
        edges_map[my_edge_key].push_back(my_edge);
    }
};


#endif //SADAS_GDB_CTP_GRAPH_H
