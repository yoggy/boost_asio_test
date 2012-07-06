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
	tcp::socket socket(io_service);

	tcp::resolver resolver(io_service);
    tcp::resolver::query query("127.0.0.1", "12345");
	tcp::resolver::iterator it = resolver.resolve(query);
	tcp::resolver::iterator end;

	err = boost::asio::error::host_not_found;
	while(it != end) {
		socket.connect(*it, err);
		if (err) break;
		it++;
	}
	if (err) {
		std::cout << "connection failed..." << err.message() << std::endl;
		return 0;
	}

	std::cout << "connect success" << std::endl;

	std::string msg;
	for (int i = 0; i < 10; ++i) {
		msg = (format("count=%d ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\r\n") % i).str();

		size_t len = socket.write_some(asio::buffer(msg.c_str(), msg.size()), err);
		if (err) {
			std::cout << "send failed..." << err.message() << std::endl;
			return 0;
		}

		thread::sleep(boost::get_system_time() + milliseconds(1000));
	}

	socket.close();

	return 0;
}

