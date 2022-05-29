#include "model/network_utils.h"
#include <boost/asio/read.hpp>

namespace Tanks::model {
std::int32_t receiveInt(tcp::socket &socket) {
    std::int32_t buff = 0;
    auto res = boost::asio::read(
        socket, buffer(reinterpret_cast<char *>(&buff), sizeof(buff)));
    assert(res == 4);
    return buff;
}
}  // namespace Tanks::model
