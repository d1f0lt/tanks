#ifndef TANKS_NETWORK_UTILS_H
#define TANKS_NETWORK_UTILS_H

#include <boost/asio/ip/tcp.hpp>

namespace Tanks::model {
using boost::asio::buffer;
using boost::asio::ip::tcp;
template <typename T>
void sendInt(tcp::socket &socket, T a) {
    auto buff = static_cast<std::int32_t>(a);
    auto res = socket.write_some(
        buffer(reinterpret_cast<const char *>(&buff), sizeof(buff)));
    assert(res == sizeof(buff));
}

std::int32_t receiveInt(tcp::socket &socket);

}  // namespace Tanks::model

#endif  // TANKS_NETWORK_UTILS_H
