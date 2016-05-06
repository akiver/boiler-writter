#include "BoilerException.h"

BoilerException::BoilerException(const std::string& what)
	:m_what(what)
{

}

const char* BoilerException::what() const throw()
{
	return m_what.c_str();
}

