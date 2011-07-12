#ifndef TableParser_hxx
#define TableParser_hxx
#include <iosfwd>
#include <vector>

class TableParser
{
public:
	class IntToken
	{
	public:
		int _value;
		IntToken(TableParser * parser)
		{
			parser->addColumn(this);
		}
		bool read(std::istream & stream)
		{
			stream >> _value;
			return stream;
		}
		int operator() ()
		{
			return _value;
		}
	};
private:
	std::istream & _stream;
	std::string _errorMessage;
	unsigned _line;
	unsigned _column;
	std::vector<IntToken*> _columns;
public:
	TableParser(std::istream & stream)
		: _stream(stream)
		, _line(0)
		, _column(0)
	{
	}
	void addColumn(IntToken * column)
	{
		_columns.push_back(column);
	}
	bool feedLine()
	{
		std::string line;
		while (line=="" or line[0]=='#')
		{
			if (not _stream) return false; // No content read
			_line++;
			std::getline(_stream, line);
		}

		_column=1;
		std::istringstream lineStream(line);
		for (unsigned i=0; i<_columns.size(); i++)
		{
			if (not _columns[i]->read(lineStream))
				return addError("Expected an int");
			_column++;
		}

		lineStream >> std::ws;
		std::string remainingContent;
		std::getline(lineStream, remainingContent);
		if (not remainingContent.empty())
			return addError("Unexpected content at the end of the line");
		return true;
	}
	const std::string & errorMessage() const
	{
		return _errorMessage;
	}
	bool hasError() const
	{
		return _errorMessage!="";
	}
	bool addError(const std::string & message)
	{
		std::ostringstream os;
		os << "Error in line " << _line;
		if (_column<=_columns.size()) os << ", token " << _column;
		os << ": " << message << "\n";
		_errorMessage += os.str();
		return false;
	}
};

#endif//TableParser_hxx

