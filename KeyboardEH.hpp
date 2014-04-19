#ifndef KEYBOARD_EH_HPP
#define KEYBOARD_EH_HPP

#include "EpollReactor.hpp"
#include <fstream>

class KeyboardEH : public EpollReactor::EventHandler
{
	public:
		KeyboardEH(const ::std::string&);
		virtual ~KeyboardEH();

	public:
		virtual void handleEvent(uint32_t);

	private:
		::std::ofstream m_file;
};

#endif

