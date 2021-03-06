

#include "Log.h"
#include <iostream>
#include <sstream>

using namespace std;

template< class T >
void WriteGeneric(const T& data, double time)
{
	cout << time << ": " << data << endl;
	clog << time << ": " << data << endl;
}

Log& Log::Instance()
{
	static Log instance;
	return instance;
}

Log::Log()
{
	m_timer.Start();

	m_file.open("GameLog.txt");

	unsigned int counter = 2;
	while(!m_file.is_open())
	{
		std::stringstream stream;
		stream << counter++;

		m_file.open("GameLog_" + stream.str() + ".txt");
	}

	// redirect clog to m_file
	clog.rdbuf(m_file.rdbuf());
}

Log::~Log()
{
	if(m_file.is_open())
	{
		m_file.close();
	}
}

void Log::Write(const std::string& str)
{
	WriteGeneric(str,m_timer.GetTime());
}

void Log::Write(const char* str)
{
	WriteGeneric(str,m_timer.GetTime());
}

void Log::Write(unsigned int i)
{
	WriteGeneric(i,m_timer.GetTime());
}

void Log::Write(unsigned long i)
{
	WriteGeneric(i,m_timer.GetTime());
}

void Log::Write(int i)
{
	WriteGeneric(i,m_timer.GetTime());
}
void Log::Write(float f)
{
	WriteGeneric(f,m_timer.GetTime());
}
void Log::Write(double d)
{
	WriteGeneric(d,m_timer.GetTime());
}
void Log::Write(char c)
{
	WriteGeneric(c,m_timer.GetTime());
}
void Log::Write(bool b)
{
	WriteGeneric(b,m_timer.GetTime());
}
