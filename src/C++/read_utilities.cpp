#include "read_utilities.hpp"

/**
 * Procedure for duplicate check in nodes csv file
 *
 * @param dataset (AUI, CTP, NEXI)
 */
void check_duplicates(std::string dataset) {
  std::string path;
  std::string id;
  if (dataset == "AUI") {
    path = AUI_NODES;
    id = "NDG";
  } else if (dataset == "CTP") {
    path = CTP_NODES;
    id = "CD_NDG";
  } else if (dataset == "NEXI") {
    path = NEXI_NODES;
    id = "ID_NODO";
  } else {
    perror("Wrong dataset nickname");
    return;
  }
  std::ifstream file(path);
  std::map<std::string, std::string> IDs;
  //std::map<int, std::string> IDs;
  // empty
  int duplicates = 0;
  int num_str = 0;
  for (CSVIterator loop(file); loop != CSVIterator(); ++loop) {
    // Skip header
    if ((*loop)[0] == id)
      // std::cout << "header\n";
      continue;
    // If string is codice fiscale (aka contains letter)
    // do not insert in map
    auto str_val_id = (*loop)[0];
    if (std::any_of(std::begin(str_val_id), std::end(str_val_id), ::isalpha))
      num_str++;
    // continue;
    //auto val_id = std::atoi((*loop)[0].c_str());
    auto val_id = (*loop)[0];
    std::string fields = "";
    int num_fields = (*loop).size();
    for (int i = 0; i < num_fields; i++) {
      fields += (*loop)[i];
    }
    // If key not present insert it
    if (!IDs.count(val_id))
    	IDs[val_id] = fields;
    else {
      duplicates++;
      //Check if the duplicates are equal in all fields
      if (IDs[val_id] != fields) {
        std::cout << IDs.find(val_id)->second << "\n";
        std::cout << fields << "\n~~~~~~~~~~~~~~~~~~~~~~\n";
      }
    }
  }
  int size = IDs.size() + duplicates;
  std::cout << dataset << " contains " << duplicates << "/" << size
            << " duplicates\n";
  std::cout << "Number of string fields: " << num_str << "\n";
  return;
}

int main() {
  check_duplicates("AUI");
  //For NEXI and CTP change str to int above
  //check_duplicates("NEXI");
  //check_duplicates("CTP");

  return 0;
}
