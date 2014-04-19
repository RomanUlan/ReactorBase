#include "KeyboardEH.hpp"
#include <string.h>
#include <sys/epoll.h>
#include <stdexcept>
#include <boost/algorithm/string.hpp>

KeyboardEH::KeyboardEH(const ::std::string& p_fileName)
	: EventHandler(0), m_file(p_fileName.c_str(), ::std::ofstream::out)
{
	if (!m_file.is_open())
	{
		throw ::std::runtime_error("Cannot open file");
	}
}

KeyboardEH::~KeyboardEH()
{
	m_file.close();
}

void KeyboardEH::handleEvent(uint32_t p_event)
{
	if (p_event & EPOLLIN)
	{
		char toRead[1024];
		::memset(toRead, 0, 1024);

		ssize_t r = read(0, toRead, 1024);
		if (r < 0)
		{
			throw ::std::runtime_error("Cannot read from keyb fd");
		}

		m_file << toRead;

		if (::boost::istarts_with(toRead, "exit"))
		{
			throw ::std::runtime_error("exit");
		}
	}
	else
	{
		throw ::std::runtime_error("Bad event for keyb fd");
	}
}

