//
// Created by Gemma Martini on 21/04/2020.
//
#include "SADAS_graph.h"

#ifndef SADAS_GDB_AUI_GRAPH_H
#define SADAS_GDB_AUI_GRAPH_H


struct AUI_node {
    std::string NDG;
    std::string PAESE_RESIDENZA;
    std::string COMUNE_RESIDENZA;
    std::string TIPO_PERSONA;
};

struct AUI_edge {
    std::string NDG_ORIGINE;
    std::string NDG_DESTINAZIONE;
    std::string TIPO_OPERAZIONE;
    std::string RAPPORTO;
    std::string TIPO_LEGAME;
    std::string DATA_OPERAZIONE;
    std::string CAUSALE_ANALITICA;
    std::string SEGNO;
    float IMPORTO;
    float IMPORTO_CONTANTE;
};

class AUI_graph : public SADAS_graph<std::string, AUI_node, AUI_edge> {
protected:
    /**
     * This method loads a line from the csv file containing nodes into the
     * nodes' map of the graph
     * @param line row of csv file
     */
    virtual void add_vtx(CSVIterator line) override {
        //std::clog << "Number of fields in line: " << (*line).size() << "\n";
        std::string id = (*line)[0];
        AUI_node my_node;
        // Fill fields of node ds
        my_node.NDG = id;
        my_node.PAESE_RESIDENZA = (*line)[1];
        my_node.COMUNE_RESIDENZA = (*line)[2];
        my_node.TIPO_PERSONA = (*line)[3];
        // Add (id, node) to map
        nodes_map[id] = my_node;
    }

    /** This method returns the keys of both origin and
    * destination nodes from a line of the edges' csv file
    * @param line row of csv file
    * @return the "couple" of id values
    */
    virtual std::array<std::string, 2> line2id(CSVIterator line) override {
        std::array<std::string, 2> from_to;
        from_to[0] = (*line)[0];
        from_to[1] = (*line)[1];
        return from_to;
    }

    /**
     * This method loads a line from the edges' file
     * into the edges' map of the graph
     * @param line row of csv file
     */
    virtual void line2edg(CSVIterator line) override {
        //std::clog << "Number of fields of line: " << (*line).size() << "\n";
        AUI_edge my_edge;
        std::string ori_id = (*line)[0];
        std::string dest_id = (*line)[1];
        // Fill fields of node ds
        my_edge.NDG_ORIGINE = ori_id;
        my_edge.NDG_DESTINAZIONE = dest_id;
        my_edge.TIPO_OPERAZIONE = (*line)[2];
        /*std::clog << "======Line======\n";
        std::clog << "TIPO_OPERAZIONE: " << (*line)[2] << "\n";
        std::clog << "RAPPORTO: " << (*line)[3] << "\n";
        std::clog << "TIPO_LEGAME: " << (*line)[4] << "\n";
        std::clog << "DATA_OPERAZIONE: " << (*line)[5] << "\n";
        std::clog << "CAUSALE_ANALITICA: " << (*line)[6] << "\n";
        std::clog << "SEGNO: " << (*line)[7] << "\n";
        std::clog << "IMPORTO: " << (*line)[8] << "\n";
        std::clog << "IMPORTO_CONTANTE: " << (*line)[9] << "\n";*/
        my_edge.RAPPORTO = (*line)[3];
        my_edge.TIPO_LEGAME = (*line)[4];
        my_edge.DATA_OPERAZIONE = (*line)[5];
        my_edge.CAUSALE_ANALITICA = (*line)[6];
        my_edge.SEGNO = (*line)[7];
        my_edge.IMPORTO = std::stof((*line)[8]);
        //std::clog << "--log--importo: " << my_edge.IMPORTO << "\n";
        my_edge.IMPORTO_CONTANTE = std::stof((*line)[9]);
        // Add (<id, id>, edge) to map
        couple<std::string> my_edge_key;
        my_edge_key.first = ori_id;
        my_edge_key.second = ori_id;
        edges_map[my_edge_key].push_back(my_edge);
    }
};

#endif //SADAS_GDB_AUI_GRAPH_H
