#pragma once
#include <cstddef>
#include <string>

#include <string>
#include <vector>

class CSVTable {
private:
  std::vector<std::vector<std::string>> m_data;
  std::vector<std::string> m_names;

public:
  // TODO: how vectors passed in initializer list
  CSVTable(std::vector<std::vector<std::string>> &data,
           std::vector<std::string> &names)
      : m_data{data}, m_names{names} {}
  // TODO: copy and assign operators
  void setNames(std::vector<std::string>);
  void appendRow(std::vector<std::string>);
  std::size_t getRowSize();
};
