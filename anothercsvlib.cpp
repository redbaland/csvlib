#include "anothercsvlib.h"
#include "CSVTable.h"
#include <fstream>
#include <sstream>

enum class ReadState { unquoted_value, quoted_value, double_quotes };

void anothercsvlib::read_csv(CSVTable& table, std::string filepath,
    bool header, char delim,
    char quotes) {


    std::ifstream f;
    f.open(filepath);

    if (!f.is_open()) {
        std::cout << "failed to open " << filepath << '\n';
        return;
    }


    if (quotes == delim) {
        throw std::logic_error("delimiter and quotes should be different");
    }

    bool size_known = false;
    std::size_t size = 0, count = 0;

    if (header) {
        std::string names;
        std::getline(f, names);
        std::stringstream s_names(names);
        std::vector<std::string> col_names;
        std::string name;
        for (; std::getline(s_names, name, delim);) {
            // Assumption that header line wouldn't contain
            // special characters and quoted names. It still could
            // but really shouldn't. So getline is used.
            col_names.push_back(std::move(name));
        }
        table.setNames(std::move(col_names));
        size_known = true;
        size = table.getRowSize();
    }

    if (!size_known && (delim == '\n' || quotes == '\n')) {
        // throw exception because we wouldn't know number of columns
        throw std::logic_error("can't read a table like this");
    }

    ReadState state = ReadState::unquoted_value;
    std::vector<std::string> row;
    std::string value;
    while (f.good()) {
        char c;
        if (f.get(c)) {

            if (c == delim) {
                switch (state) {
                case ReadState::unquoted_value: {
                    row.push_back(std::move(value));
                    count++;
                    if (size_known && count == size) { // for case when delimiter or quotes is \n
                        table.appendRow(std::move(row));
                        count = 0;
                    }
                    break;
                }
                case ReadState::quoted_value: {
                    // TODO: put special characters in string
                    value += c;
                    break;
                }
                case ReadState::double_quotes: {
                    row.push_back(std::move(value));
                    state = ReadState::unquoted_value;
                    count++;
                    if (size_known && count == size) { // for case when delimiter or quotes is \n
                        table.appendRow(std::move(row));
                        count = 0;
                    }
                    break;
                }
                }
            }

            else if (c == quotes) {
                switch (state) {
                case ReadState::unquoted_value: {
                    if (value.empty()) {
                        state = ReadState::quoted_value;
                        break;
                    }
                    else {
                        // throw exception if there are single quotes in the middle of
                        // value like ...,a"b,...
                        throw std::logic_error("value must be entirely in quotes");
                    }
                }
                case ReadState::quoted_value: {
                    state = ReadState::double_quotes;
                    break;
                }
                case ReadState::double_quotes: {
                    value += quotes;
                    // TODO: think about special case of quotes like '\t'
                    state = ReadState::quoted_value;
                    break;
                }
                }
            }

            else {
                switch (state) {
                case ReadState::unquoted_value: {
                    if (c == '\n') {
                        count++;
                        row.push_back(std::move(value));
                        if (size_known && count != size) {
                            throw std::invalid_argument("mismatched table dimensions");
                        }
                        table.appendRow(std::move(row));
                        count = 0;
                        if (!size_known) {
                            /*row.push_back(std::move(value));  // why did I do this??
                            table.appendRow(std::move(row));*/

                            size_known = true;
                            size = table.getRowSize();
                        }
                    }
                    else {
                        value += c;
                        // TODO: special characters should't be here
                    }
                    break;
                }
                case ReadState::quoted_value: {
                    value += c;
                    // TODO: write special chars to string with \ to escape them
                    break;
                }
                case ReadState::double_quotes: {
                    if (c == '\n') {
                        count++;
                        row.push_back(std::move(value));
                        state = ReadState::unquoted_value;
                        if (size_known && count != size) {
                            throw std::invalid_argument("mismatched table dimensions");
                        }
                        table.appendRow(std::move(row));
                        count = 0;
                        if (!size_known) {
                            //row.push_back(std::move(value)); // why did I do this??
                            //table.appendRow(std::move(row));

                            size_known = true;
                            size = table.getRowSize();
                        }
                    }
                    else {
                        // throw exception because ,""ab..., isn't valid
                        throw std::logic_error("entire value should be in quotes");
                    }
                    break;
                }
                }
            }

        }
    }

    if (!f.eof() && f.fail()) {
        std::cout << "something went wrong with a file " << filepath << '\n';
    }

    f.close();
}

void anothercsvlib::read_with(CSVTable& t, std::string path, char delim, char quotes) {
    anothercsvlib::read_csv(t, path, false, delim, quotes);
}
void anothercsvlib::read_with_del(CSVTable& t, std::string path, char delim) {
    anothercsvlib::read_csv(t, path, false, delim);
}
void anothercsvlib::read_with_quotes(CSVTable& t, std::string path, char quotes) {
    anothercsvlib::read_csv(t, path, false, ',', quotes);
}


static std::string wrap_in_quotes(std::string s, char delim, char quotes) {
    bool has_special_chars = false;
    std::string res;
    for (const auto& c : s) {
        res += c;
        if (c == quotes) {
            res += c;
            has_special_chars = true;
        }
        else if (c == delim) {
            has_special_chars = true;
        }

        // TODO: need another case where c is somehing like \n
    }
    if (has_special_chars) {
        res = quotes + res + quotes;
    }
    return res;
}


void anothercsvlib::write_csv(CSVTable& table, std::string filepath,
    bool header, char delim,
    char quotes) {
    std::ofstream f;
    f.open(filepath);

    if (!f.is_open()) {
        std::cout << "failed to open " << filepath << '\n';
        return;
    }

    if (quotes == delim) {
        throw std::logic_error("delimiter and quotes should be different");
    }

    if (f.good()) {
        if (header) {
            if (table.getNames().empty()) {
                std::cout << "warning: no names to write" << filepath << '\n';
            }
            else {
                for (std::size_t i = 0; i < table.getNames().size() - 1; i++) {
                    f << table.getNames()[i] << delim;
                }
                f << table.getNames()[table.getNames().size() - 1] << '\n';
            }
        }

        for (auto& row : table.getData()) {  // need to wrap in quotes
            for (std::size_t i = 0; i < row.size() - 1; i++) {
                f << wrap_in_quotes(row[i], delim, quotes) << delim;  // need to think about special delim characters
            }
            f << wrap_in_quotes(row[row.size() - 1], delim, quotes) << '\n';
        }

    }

    if (f.fail()) {
        std::cout << "something went wrong with a file " << filepath << '\n';
    }

    f.close();
}

void anothercsvlib::write_with(CSVTable& t, std::string path, char delim, char quotes) {
    anothercsvlib::write_csv(t, path, false, delim, quotes);
}
void anothercsvlib::write_with_del(CSVTable& t, std::string path, char delim) {
    anothercsvlib::write_csv(t, path, false, delim);
}
void anothercsvlib::write_with_quotes(CSVTable& t, std::string path, char quotes) {
    anothercsvlib::write_csv(t, path, false, ',', quotes);
}


void anothercsvlib::print_csv(const CSVTable& table) {
    std::cout << table;
}
