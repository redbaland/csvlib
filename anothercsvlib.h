#pragma once

#include "CSVTable.h"

namespace anothercsvlib {
	void readcsv(CSVTable&,const std::string,const bool,const char,const char);	
	void writecsv();
}
