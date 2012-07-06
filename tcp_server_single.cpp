#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <SDKDDKVer.h>
	#include <Windows.h>
#endif

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <string>

using boost::thread;
using boost::format;
using namespace boost::posix_time;
namespace asio  = boost::asio;
using asio::ip::tcp;

int main(int argc, char* argv[])
{
	boost::system::error_code err;

	asio::io_service io_service;
	tcp::acceptor acceptor(io_service);

	tcp::endpoint endpoint(tcp::v4(), 12345);
	acceptor.open(endpoint.protocol());
	//acceptor.set_option(tcp::acceptor::reuse_address(true));
	acceptor.bind(endpoint, err);
	if (err) {
		std::cout << err.message() << std::endl;
		return 0;
	}
	acceptor.listen(5);

	while (true) {
		tcp::iostream s;
		acceptor.accept(*s.rdbuf());

		std::cout << "connect : peer=" << std::endl;

		// ŽóM
		std::string line;
		while (getline(s, line)) {
			std::cout << line << std::endl;
		}
		std::cout << "[connect end]" << std::endl;
	}

	return 0;	
}

