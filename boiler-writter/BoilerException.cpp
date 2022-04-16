#include "BoilerException.h"

BoilerException::BoilerException(BoilerExitCode exitCode, const std::string& what) : m_what(what)
{
	m_exitCode = exitCode;
}

int BoilerException::getExitCode() const
{
	return (int)m_exitCode;
}

const char* BoilerException::what() const throw()
{
	return m_what.c_str();
}
