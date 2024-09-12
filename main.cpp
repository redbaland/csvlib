#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

// class CSVLine {
// private:
// std::vector<std::string> row;
// public:
// 	// TODO: how vectors passed by in initializer list?
//
// 	CSVLine(const std::string row_) : row{row_} {}
// };

class CSVTable {
private:
	std::vector<std::vector<std::string>> data;
	std::vector<std::string> names;
public:
	CSVTable(std::vector<std::vector<std::string>>& data_, std::vector<std::string>& names_) : data{data_}, names{names_} {}
	// TODO: copy and assign operators
};

// singletons reader and writer

class CSVReader {

};

class CSVWriter {

};
