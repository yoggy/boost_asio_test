#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <SDKDDKVer.h>
	#include <Windows.h>
#endif

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>

using boost::thread;
using boost::format;
using namespace boost::posix_time;
namespace asio  = boost::asio;
using asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void session(socket_ptr sock)
{
	boost::system::error_code err;
	boost::array<char, 128> buf;

	try {
		while (true) {
			memset(&buf, 0, buf.size());
			size_t length = sock->read_some(asio::buffer(buf), err);
			if (err == boost::asio::error::eof)
				break;
			else if (err)
				throw boost::system::system_error(err);

			printf("recv: %s\n", &buf);
			asio::write(*sock, asio::buffer(buf, length));
		}
	}
	catch (std::exception& e) {
		std::cerr << "exception in session : " << e.what() << "\n";
	}
}

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


	try {
		while (true) {
			socket_ptr sock(new tcp::socket(io_service));
			acceptor.accept(*sock);
			boost::thread thread(boost::bind(session, sock));
		}
	}
	catch (std::exception& e) {
		std::cerr << "exception in main() : " << e.what() << "\n";
	}

	return 0;	
}

