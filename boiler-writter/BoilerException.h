#ifndef BoilerException_h__
#define BoilerException_h__
#include <exception>
#include <string>
#include "BoilerExitCode.h"

class BoilerException : public std::exception
{
public:
	BoilerException(BoilerExitCode exitCode, const std::string& what);
	virtual const char* what() const throw();
	int getExitCode() const;

private:
	std::string m_what;
	BoilerExitCode m_exitCode;
};

#endif // BoilerException_h__
