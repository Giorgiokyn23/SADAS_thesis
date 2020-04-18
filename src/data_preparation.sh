#!/bin/bash
#title              :convert_all.sh
#description        :This script create new csv files sanitizing them
#author             :Gemma Martini
#date               :20200420
#version            :1.0
#usage              :bash convert_all.sh
#notes              :we assume that any csv file terminates with ";" 
#bash_version       :3.2.57(1)-release

AUI_origin_edges="../data/AUI/AUI_ARCHI_OFF.csv"
AUI_dest_edges="../data/my_datasets/AUI_edges.csv"

AUI_origin_nodes="../data/AUI/AUI_NODI_OFF.csv"
AUI_dest_nodes="../data/my_datasets/AUI_nodes.csv"

NEXI_origin_edges="../data/CARTE_NEXI/CARTE_ARCHI_OFF.CSV"
NEXI_dest_edges="../data/my_datasets/NEXI_edges.csv"

NEXI_origin_nodes="../data/CARTE_NEXI/CARTE_NODI_OFF.CSV"
NEXI_dest_nodes="../data/my_datasets/NEXI_nodes.csv"

CTP_origin_edges="../data/CTP/CTP_ARCHI.CSV"
CTP_dest_edges="../data/my_datasets/CTP_edges.csv"

CTP_origin_nodes="../data/CTP/CTP_NODI.CSV"
CTP_dest_nodes="../data/my_datasets/CTP_nodes.csv"

tmp="tmp.csv"

# function for removing the trailing ; in csv files
remove_trailing_semicolon () {
    echo "Removing trailing ;"
    cat $1 | sed 's/\(.*\);/\1/' > $2
}

# function for transforming floating point format from 1,2 to 1.2
refactor_floating_point () {
    echo "Replacing , with ."
    cat $1 | sed -r 's/[,]/./g' > $2
}


echo "-----------------Converting AUI nodes-----------------"
rm $AUI_dest_nodes
remove_trailing_semicolon $AUI_origin_nodes $AUI_dest_nodes

echo "-----------------Converting AUI edges-----------------"
rm $tmp
rm $AUI_dest_edges
remove_trailing_semicolon $AUI_origin_edges $tmp
refactor_floating_point $tmp $AUI_dest_edges

echo "-----------------Converting NEXI nodes-----------------"
rm $NEXI_dest_nodes
remove_trailing_semicolon $NEXI_origin_nodes $NEXI_dest_nodes

echo "-----------------Converting NEXI edges-----------------"
rm $tmp
rm $NEXI_dest_edges
remove_trailing_semicolon $NEXI_origin_edges $tmp
refactor_floating_point $tmp $NEXI_dest_edges

echo "-----------------Converting CTP nodes-----------------"
rm $CTP_dest_nodes
remove_trailing_semicolon $CTP_origin_nodes $CTP_dest_nodes

echo "-----------------Converting CTP edges-----------------"
rm $tmp
rm $CTP_dest_edges
remove_trailing_semicolon $CTP_origin_edges $CTP_dest_edges
#No floating point refactoring needed
