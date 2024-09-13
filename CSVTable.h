#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <exception>

class CSVTable {
private:
	std::vector<std::vector<std::string>> m_data; // one-dimensional would probably be faster
	std::vector<std::string> m_names; // hash set is probably a better variant beacause of searching and uniqueness

public:
	void setNames(std::vector<std::string>);
	void appendRow(std::vector<std::string>);

	std::size_t getRowSize() const;
	std::size_t getRows() const { return m_data.size(); }
	std::size_t getColumns() const;

	const std::vector<std::vector<std::string>>& getData() const { return m_data; }
	const std::vector<std::string>& getNames() const { return m_names; }

	// should probably make methods that wouldn't return string copies
	std::vector<std::string> getColumn(int) const;
	std::vector<std::string> getColumn(std::string) const;

	const std::vector<std::string>& getRow(int) const;

	std::string getValue(int, int) const;
	std::string getValue(int, std::string) const;

	void removeRow(int);
	void removeColumn(int);
	void removeColumn(std::string);

	friend std::ostream& operator<<(std::ostream&, const CSVTable&);
};
