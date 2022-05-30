#ifndef TANKS_NETWORK_UTILS_H
#define TANKS_NETWORK_UTILS_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>

namespace Tanks::model {
using boost::asio::buffer;
using boost::asio::ip::tcp;
template <typename T>
void sendInt(tcp::socket &socket, T a) {
    auto buff = static_cast<std::int32_t>(a);

    boost::asio::write(
        socket, buffer(reinterpret_cast<const char *>(&buff), sizeof(buff)),
        boost::asio::transfer_exactly(sizeof(buff)));
}

std::int32_t receiveInt(tcp::socket &socket);

template <typename... Args>
void sendMultipleInts(tcp::socket &socket, Args... args) {
    (sendInt(socket, args), ...);
}

template <typename... Args>
[[nodiscard]] std::tuple<Args...> receiveMultipleInts(tcp::socket &socket) {
    return {static_cast<Args>(receiveInt(socket))...};
}
}  // namespace Tanks::model

#endif  // TANKS_NETWORK_UTILS_H
