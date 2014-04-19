#ifndef ECHO_RESPONDER_EH_HPP
#define ECHO_RESPONDER_EH_HPP

#include "EpollReactor.hpp"

class EchoResponderEH : public EpollReactor::EventHandler
{
	public:
		explicit EchoResponderEH(int);
		virtual ~EchoResponderEH();

	public:
		virtual void handleEvent(uint32_t);
};

#endif

