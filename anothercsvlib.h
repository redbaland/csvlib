#pragma once

#include "CSVTable.h"

namespace anothercsvlib {
	// in read and write not all edge cases are fully implemented

	// read csv table from file with parameters
	void read_csv(CSVTable&, std::string path, bool header = false, char delim = ',', char quotes = '"');
	void read_with(CSVTable&, std::string path, char delim, char quotes);
	void read_with_del(CSVTable&, std::string path, char delim);
	void read_with_quotes(CSVTable&, std::string path, char quotes);

	// write csv table from file with parameters
	void write_csv(CSVTable&, std::string path, bool header = false, char delim = ',', char quotes = '"');
	void write_with(CSVTable&, std::string path, char delim, char quotes);
	void write_with_del(CSVTable&, std::string path, char delim);
	void write_with_quotes(CSVTable&, std::string path, char quotes);

	// print csv table to console
	void print_csv(const CSVTable&);
}
