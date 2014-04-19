#include "KeyboardEH.hpp"
#include "AcceptorEH.hpp"
#include <stdexcept>

int main(int, char**)
{
	try
	{
		EpollReactor reactor;
		EpollReactor::EventHandler::Ptr ehKeyb(new KeyboardEH("log.txt"));
		EpollReactor::EventHandler::Ptr ehAcceptor(new AcceptorEH(5050, reactor));

		reactor.registerHandler(ehKeyb);
		reactor.registerHandler(ehAcceptor);

		reactor.eventLoop();
	}
	catch (const ::std::runtime_error& rte)
	{
		::std::cout << "Runtime exception: " << rte.what() << ::std::endl;
	}
	catch (const ::std::exception& e)
	{
		::std::cout << "STD exception: " << e.what() << ::std::endl;
	}

	return 0;
}

