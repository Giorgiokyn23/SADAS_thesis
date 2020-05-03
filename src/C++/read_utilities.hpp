#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#ifdef SMALL
    const std::string AUI_NODES = "../../data/my_datasets/small/AUI_nodes.csv";
	const std::string NEXI_NODES = "../../data/my_datasets/small/NEXI_nodes.csv";
	const std::string CTP_NODES = "../../data/my_datasets/small/CTP_nodes.csv";

	const std::string AUI_EDGES = "../../data/my_datasets/small/AUI_edges.csv";
	const std::string NEXI_EDGES = "../../data/my_datasets/small/NEXI_edges.csv";
	const std::string CTP_EDGES = "../../data/my_datasets/small/CTP_edges.csv";

#elif BLUE
    const std::string AUI_NODES = "../../data/my_datasets/small/AUI_nodes_test_blue.csv";
	const std::string NEXI_NODES = "../../data/my_datasets/small/NEXI_nodes_test_blue.csv";
	const std::string CTP_NODES = "../../data/my_datasets/small/CTP_nodes_test_blue.csv";

	const std::string AUI_EDGES = "../../data/my_datasets/small/AUI_edges_test_blue.csv";
	const std::string NEXI_EDGES = "../../data/my_datasets/small/NEXI_edges_test_blue.csv";
	const std::string CTP_EDGES = "../../data/my_datasets/small/CTP_edges_test_blue.csv";

#elif ORANGE
    const std::string AUI_NODES = "../../data/my_datasets/small/AUI_nodes_test_orange.csv";
	const std::string NEXI_NODES = "../../data/my_datasets/small/NEXI_nodes_test_orange.csv";
	const std::string CTP_NODES = "../../data/my_datasets/small/CTP_nodes_test_orange.csv";

	const std::string AUI_EDGES = "../../data/my_datasets/small/AUI_edges_test_orange.csv";
	const std::string NEXI_EDGES = "../../data/my_datasets/small/NEXI_edges_test_orange.csv";
	const std::string CTP_EDGES = "../../data/my_datasets/small/CTP_edges_test_orange.csv";

#elif TESTCC
    const std::string AUI_NODES = "../../data/my_datasets/small/AUI_nodes_test_cc.csv";
	const std::string NEXI_NODES = "../../data/my_datasets/small/NEXI_nodes_test_scc.csv";
	const std::string CTP_NODES = "../../data/my_datasets/small/CTP_nodes_test_scc.csv";

	const std::string AUI_EDGES = "../../data/my_datasets/small/AUI_edges_test_cc.csv";
	const std::string NEXI_EDGES = "../../data/my_datasets/small/NEXI_edges_test_scc.csv";
	const std::string CTP_EDGES = "../../data/my_datasets/small/CTP_edges_test_scc.csv";

#else
    const std::string AUI_NODES = "../../data/my_datasets/AUI_nodes.csv";
    const std::string NEXI_NODES = "../../data/my_datasets/NEXI_nodes.csv";
    const std::string CTP_NODES = "../../data/my_datasets/CTP_nodes.csv";

    const std::string AUI_EDGES = "../../data/my_datasets/AUI_edges.csv";
    const std::string NEXI_EDGES = "../../data/my_datasets/NEXI_edges.csv";
    const std::string CTP_EDGES = "../../data/my_datasets/CTP_edges.csv";

#endif


long long int num_lines(std::string f) {
    long long int num_lines = 0;
    std::ifstream in(f);
    std::string unused;
    while (std::getline(in, unused))
        ++num_lines;
    return num_lines;
}


/**
 * Class representing a row of csv file (fields separated by comma)
 *
 */
class CSVRow {
public:
  std::string const &operator[](std::size_t index) const {
    return m_data[index];
  }
  std::size_t size() const { return m_data.size(); }
  void readNextRow(std::istream &str) {
    std::string line;
    std::getline(str, line);

    std::stringstream lineStream(line);
    std::string cell;

    m_data.clear();
    while (std::getline(lineStream, cell, ';')) {
      m_data.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty()) {
      // If there was a trailing comma then add an empty element.
      m_data.push_back("");
    }
  }

private:
  std::vector<std::string> m_data;
};

std::istream &operator>>(std::istream &str, CSVRow &data) {
  data.readNextRow(str);
  return str;
}

/**
 * Class representing an iterator over a csv file
 *
 */
class CSVIterator {
public:
  typedef std::input_iterator_tag iterator_category;
  typedef CSVRow value_type;
  typedef std::size_t difference_type;
  typedef CSVRow *pointer;
  typedef CSVRow &reference;

  CSVIterator(std::istream &str) : m_str(str.good() ? &str : NULL) {
    ++(*this);
  }
  CSVIterator() : m_str(NULL) {}

  // Pre Increment
  CSVIterator &operator++() {
    if (m_str) {
      if (!((*m_str) >> m_row)) {
        m_str = NULL;
      }
    }
    return *this;
  }
  // Post increment
  CSVIterator operator++(int) {
    CSVIterator tmp(*this);
    ++(*this);
    return tmp;
  }
  CSVRow const &operator*() const { return m_row; }
  CSVRow const *operator->() const { return &m_row; }

  bool operator==(CSVIterator const &rhs) {
    return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));
  }
  bool operator!=(CSVIterator const &rhs) { return !((*this) == rhs); }

private:
  std::istream *m_str;
  CSVRow m_row;
};

/**
 * Procedure for duplicate check in nodes csv file
 *
 * @param dataset (AUI, CTP, NEXI)
 */
void check_duplicates(std::string dataset);
