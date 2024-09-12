#include "anothercsvlib.h"
#include "CSVTable.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

enum class ReadState { unquoted_value, quoted_value, escaped_quotes };

void anothercsvlib::readcsv(CSVTable &table, const std::string filepath,
                            const bool header = false, const char delim = ',',
                            const char quotes = '"') {
  std::ifstream f;
  f.open(filepath);

  if (!f.is_open()) {
    std::cout << "failed to open " << filepath << '\n';
    return;
  }

  bool size_known = false;
  int size = 0, count = 0;

  if (header) {
    std::string names;
    std::getline(f, names);
    std::stringstream s_names(names);
    std::vector<std::string> col_names;
    for (std::string name; std::getline(s_names, name, delim);) {
      // Assumption that header line wouldn't contain
      // special characters and quoted names. It still could
      // but really shouldn't. So getline is used.
      col_names.push_back(name);
    }
    table.setNames(col_names);
    size_known = true;
    size = table.getRowSize();
  }

  if (!size_known && (delim == '\n' || quotes == '\n')) {
    // TODO: throw exception because can't read such table
  }

  ReadState state = ReadState::unquoted_value;
  std::vector<std::string> row;
  std::string value;
  while (f.good()) {
    char c;
    if (f.get(c)) {
      switch (c) {
      case delim:
        switch (state) {
        case ReadState::unquoted_value: {
          row.push_back(value);
          count++;
          if (size_known && count == size) {
            table.appendRow(row);
            count = 0;
          }
          break;
        }
        case ReadState::quoted_value: {
          // TODO: put special characters in string
          value += c;
          break;
        }
        case ReadState::escaped_quotes: {
          row.push_back(value);
          count++;
          if (size_known && count == size) {
            table.appendRow(row);
            count = 0;
          }
          break;
        }
        }
      case quotes:
        switch (state) {
        case ReadState::unquoted_value: {
          if (value.empty()) {
            state = ReadState::quoted_value;
            break;
          } else {
            // TODO: throw exception if there are single quotes in the middle of
            // value like ...,a"b,...
          }
        }
        case ReadState::quoted_value: {
          state = ReadState::escaped_quotes;
          break;
        }
        case ReadState::escaped_quotes: {
          value += quotes;
          // TODO: think about special case of quotes like '\t'
          state = ReadState::quoted_value;
          break;
        }
        }
      // case '\n': think about it later
      default:
        switch (state) {
        case ReadState::unquoted_value: {
          if (c == '\n') {
            if (!size_known) {
              row.push_back(std::move(value));
              table.appendRow(std::move(row));

              size_known = true;
              size = table.getRowSize();
            }
          } else {
            value += c;
            // TODO: again special characters should't be here
          }
          break;
        }
        case ReadState::quoted_value: {
          value += c;
          // TODO: special chars add with \
          break;
        }
        case ReadState::escaped_quotes: {
          if (c == '\n') {
            if (!size_known) {
              row.push_back(std::move(value));
              table.appendRow(std::move(row));

              size_known = true;
              size = table.getRowSize();
            }
          } else {
            // TODO: throw exception because ,""ab..., qoutes aren't escaped
          }
          break;
        }
        }
      }
    }
  }

  if (!f.eof() && f.fail()) {
    // TODO: throw exception
  }

  f.close();
}
