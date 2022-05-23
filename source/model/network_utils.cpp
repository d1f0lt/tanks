#include "model/network_utils.h"
#include <boost/asio/read.hpp>

namespace Tanks::model {
std::int32_t receiveInt(boost::asio::ip::tcp::socket &socket) {
    std::int32_t buff = 0;
    boost::asio::read(
        socket,
        boost::asio::buffer(reinterpret_cast<char *>(&buff), sizeof(buff)));
    return buff;
}
}  // namespace Tanks::model
