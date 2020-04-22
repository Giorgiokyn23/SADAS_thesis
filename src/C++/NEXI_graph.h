//
// Created by Gemma Martini on 22/04/2020.
//
#include "SADAS_graph.h"


#ifndef SADAS_GDB_NEXI_GRAPH_H
#define SADAS_GDB_NEXI_GRAPH_H

struct NEXI_node {
    long long int ID_NODO;
    std::string TIPO_NODO;
    std::string LUOGO_DI_NASCITA;
    std::string ETA;
    std::string DATA_INIZIO_VALIDITA;
    std::string DATA_FINE_VALIDITA;
    std::string STATO_CARTA;
    std::string CO_SICC;
    std::string MCC_ESERCENTE;
    std::string LOCALITA_ESERCENTE;
    std::string NAZIONE_ESERCENTE;
};

struct NEXI_edge {
    long long int ID_NODO_ORIGINE;
    long long int ID_NODO_DESTINAZIONE;
    std::string TIPO_OPERAZIONE;
    std::string TIMESTAMP_OPERAZIONE;
    std::string OPERAZIONI_INTERNET;
    double IMPORTO;
};

class NEXI_graph : public SADAS_graph<long long int, NEXI_node, NEXI_edge> {
protected:

    /**
     * This method loads a line from the csv file containing nodes into the
     * nodes' map of the graph
     * @param line row of csv file
     */
    virtual void add_vtx(CSVIterator line) override {
        // std::cout << (*line).size() << "\n";
        long long int id = std::stoll((*line)[0]);
        NEXI_node my_node;
        my_node.ID_NODO = id;
        my_node.ID_NODO = std::stoll((*line)[0]);
        my_node.TIPO_NODO = (*line)[1];
        my_node.LUOGO_DI_NASCITA = (*line)[2];
        my_node.ETA = (*line)[3];
        my_node.DATA_INIZIO_VALIDITA = (*line)[4];
        my_node.DATA_FINE_VALIDITA = (*line)[5];
        my_node.STATO_CARTA = (*line)[6];
        my_node.CO_SICC = (*line)[7];
        my_node.MCC_ESERCENTE = (*line)[8];
        my_node.LOCALITA_ESERCENTE = (*line)[9];
        my_node.NAZIONE_ESERCENTE = (*line)[10];
        // Add (id, node) to map
        nodes_map[id] = my_node;
        return;
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
        NEXI_edge my_edge;
        long long int ori_id = std::stoll((*line)[0]);
        long long int dest_id = std::stoll((*line)[1]);
        // Fill fields of node ds
        my_edge.ID_NODO_ORIGINE = ori_id;
        my_edge.ID_NODO_DESTINAZIONE = dest_id;
        // std::clog << (*line)[3] << "\n";
        my_edge.TIPO_OPERAZIONE = (*line)[2];
        my_edge.TIMESTAMP_OPERAZIONE = (*line)[3];
        my_edge.OPERAZIONI_INTERNET = (*line)[4];
        my_edge.IMPORTO = std::stod((*line)[5]);
        couple<long long int> my_edge_key;
        my_edge_key.first = ori_id;
        my_edge_key.second = ori_id;
        edges_map[my_edge_key].push_back(my_edge);
    }
};


#endif //SADAS_GDB_NEXI_GRAPH_H
