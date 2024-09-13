#include "CSVTable.h"

void CSVTable::setNames(std::vector<std::string> names) {
	if (!m_data.empty() && m_data[0].size() != names.size()) {
		std::cout << "warning: number of columns do not match number of names, can't set" << std::endl;
		return;
	}
	m_names = names;
}

void CSVTable::appendRow(std::vector<std::string> row) {
	m_data.push_back(row);
}

std::size_t CSVTable::getRowSize() const {
	if (!m_names.empty()) {
		return m_names.size();
	}
	else {
		return this->getColumns();
	}
}

std::size_t CSVTable::getColumns() const {
	if (m_data.empty()) {
		return 0;
	}
	else {
		return m_data[0].size();
	}
}

std::vector<std::string> CSVTable::getColumn(int ind) const {
	if (ind < 0 || ind >= this->getColumns()) {
		throw std::invalid_argument("index out of range");
	}
	std::vector<std::string> col;
	for (const auto& row : m_data) {
		col.push_back(row[ind]);
	}
	return col;
}

std::vector<std::string> CSVTable::getColumn(std::string col_name) const {
	if (m_names.empty()) {
		throw std::logic_error("unnamed columns");
	}
	int ind = 0;
	for (const auto& name : m_names) {
		if (col_name.compare(name) == 0) {
			return this->getColumn(ind);
		}
		ind++;
	}
	throw std::invalid_argument("no such column name");
}

const std::vector<std::string>& CSVTable::getRow(int ind) const {
	if (ind < 0 || ind >= m_data.size()) {
		throw std::invalid_argument("index out of range");
	}
	return m_data[ind];
}

std::string CSVTable::getValue(int row_ind, int col_ind) const {
	if (row_ind < 0 || row_ind >= m_data.size() || col_ind < 0 || col_ind >= this->getColumns()) {
		throw std::invalid_argument("index out of range");
	}
	return m_data[row_ind][col_ind];
}

std::string CSVTable::getValue(int row_ind, std::string col_name) const {
	if (m_names.empty()) {
		throw std::logic_error("unnamed columns");
	}
	int ind = 0;
	for (const auto& name : m_names) {
		if (col_name.compare(name) == 0) {
			return this->getValue(row_ind, ind);
		}
		ind++;
	}
	throw std::invalid_argument("no such column name");
}

void CSVTable::removeRow(int ind) {
	if (ind < 0 || ind >= m_data.size()) {
		throw std::invalid_argument("index out of range");
	}
	m_data.erase(m_data.begin() + ind);
}

void CSVTable::removeColumn(int ind) {
	if (ind < 0 || ind >= this->getColumns()) {
		throw std::invalid_argument("index out of range");
	}
	for (auto& row : m_data) {
		row.erase(row.begin() + ind);
	}
}

void CSVTable::removeColumn(std::string col_name) {
	if (m_names.empty()) {
		throw std::logic_error("unnamed columns");
	}
	int ind = 0;
	for (const auto& name : m_names) {
		if (col_name.compare(name) == 0) {
			removeColumn(ind);
			return;
		}
		ind++;
	}
	throw std::invalid_argument("no such column name");
}

std::ostream& operator<<(std::ostream& stream, const CSVTable& table) {
	if (!table.getNames().empty()) {
		for (auto& name : table.getNames()) {
			stream << name << ' ';
		}
		stream << '\n';
	}
	if (!table.getData().empty()) {
		for (auto& row : table.getData()) {
			for (auto& el : row) {
				stream << el << ' ';
			}
			stream << '\n';
		}
	}
	return stream;
}

