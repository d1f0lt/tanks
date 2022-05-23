#ifndef TANKS_NETWORK_UTILS_H
#define TANKS_NETWORK_UTILS_H

#include <boost/asio/ip/tcp.hpp>

namespace Tanks::model {
template <typename T>
void sendInt(boost::asio::ip::tcp::socket &socket, T a) {
    auto tmp = static_cast<std::int32_t>(a);
    socket.write_some(
        boost::asio::buffer(reinterpret_cast<const char *>(&tmp), sizeof(T)));
}

std::int32_t receiveInt(boost::asio::ip::tcp::socket &socket);
}  // namespace Tanks::model

#endif  // TANKS_NETWORK_UTILS_H
