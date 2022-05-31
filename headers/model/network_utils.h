#ifndef TANKS_NETWORK_UTILS_H
#define TANKS_NETWORK_UTILS_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

namespace Tanks::model {
using boost::asio::buffer;
using boost::asio::ip::tcp;
using transferIntType = std::int32_t;

template <typename T>
void sendInt(tcp::socket &socket, T value) {
    auto buff = static_cast<transferIntType>(value);

    boost::asio::write(
        socket, buffer(reinterpret_cast<const char *>(&buff), sizeof(buff)),
        boost::asio::transfer_exactly(sizeof(buff)));
}

[[nodiscard]] inline transferIntType receiveInt(tcp::socket &socket) {
    transferIntType buff = 0;
    auto res = boost::asio::read(
        socket, buffer(reinterpret_cast<char *>(&buff), sizeof(buff)));
    assert(res == sizeof(transferIntType));
    return buff;
}

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
