#include "model/network_utils.h"
#include <boost/asio/read.hpp>

namespace Tanks::model {
void safeShutdown(tcp::socket &socket) {
    try {
        socket.shutdown(tcp::socket::shutdown_both);
    } catch (boost::system::system_error &) {
    }
}
}  // namespace Tanks::model
