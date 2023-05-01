//
// Created by aalisher on 4/2/23.
//

#include "Session.hpp"

#include <string>
#include <utility>

#include <log/log.hpp>
#include <util/OnScopeExit.hpp>
#include <util/serde.hpp>
#include <websocket/Message.hpp>
namespace ztstl::websocket::server {

Session::Session(std::shared_ptr<Server> server, TcpSocket&& socket, Context& context, SslContext& sslContext) :
    m_remoteEndpoint {socket.remote_endpoint()},
    m_stream {std::move(socket), sslContext},
    m_server {std::move(server)},
    m_wStrand {context},
    m_context {context} {}

void Session::open() noexcept {
    debug("open session session from {}", to_string(remoteEndpoint()));
    auto self = shared_from_this();
    asio::dispatch(m_stream.get_executor(), [self] {
        self->onOpen();
    });
}

void Session::onOpen() {
    beast::get_lowest_layer(m_stream).expires_after(std::chrono::seconds(30));
    auto self = shared_from_this();
    m_stream.next_layer().async_handshake(ssl::stream_base::server, [self](auto errorCode) {
        trace("handshaking with {}", to_string(self->remoteEndpoint()));
        auto result = Result {Result::Success};

        if (errorCode) {
            result.code = Result::Error;
            result.message = fmt::format("{} - {} on endpoint {}", errorCode.value(), errorCode.what(), to_string(self->m_remoteEndpoint));
        }
        self->onHandshake(result);
    });
}

void Session::onHandshake(Result const& result) {
    if (not result.isOk()) {
        error("handshake error {}", result.message);
        return;
    }
    trace("handshaking with {} is succeed", to_string(remoteEndpoint()));

    beast::get_lowest_layer(m_stream).expires_never();
    m_stream.read_message_max(std::numeric_limits<size_t>::max());
    m_stream.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::server));

    m_stream.set_option(beast::websocket::stream_base::decorator([](beast::websocket::response_type& res) {
        res.set(http::field::server, "websocket");
    }));

    trace("accepting from {}", to_string(remoteEndpoint()));

    auto self = shared_from_this();
    m_stream.async_accept([self](auto errorCode) {
        auto result = Result {Result::Success};
        if (errorCode) {
            result.code = Result ::Error;
            result.message = fmt::format("{} - {} on endpoint {}", errorCode.value(), errorCode.what(), to_string(self->m_remoteEndpoint));
        }
        self->onAccept(result);
    });
}

void Session::onAccept(Result const& result) {
    if (not result.isOk()) {
        error("accept error {}", result.message);
        return;
    }

    trace("accepting from {} is succeed", to_string(remoteEndpoint()));
    startRead();
}

void Session::onRead(Result const& result) {
    util::OnScopeExit _ {[&] {
        startRead();
    }};

    if (not result.isOk()) {
        error("read error {}", result.message);
        return;
    }

    auto self = shared_from_this();
    auto payload = m_rBuffer.data();
    m_context.post([self, payload] {
        auto message = serde::from_binary<Message>(static_cast<uint8_t*>(payload.data()), payload.size());
        self->m_server->onMessage(self, message);
    });
}

void Session::send(std::string const& data) noexcept {
    m_stream.binary(true);
    auto self = shared_from_this();
    boost::asio::post(m_wStrand, [self, data]() mutable {
        self->m_wBuffer.consume(self->m_wBuffer.size());
        asio::buffer_copy(self->m_wBuffer.prepare(data.size()), asio::buffer(data));
        self->m_wBuffer.commit(data.size());

        beast::error_code errorCode {};
        self->m_stream.write(self->m_wBuffer.data(), errorCode);
        auto result = Result {util::Result::Success};
        if (errorCode) {
            result.code = util::Result::Error;
            result.message = fmt::format("{} - {} on endpoint {}", errorCode.value(), errorCode.what(), to_string(self->m_remoteEndpoint));
        }

        self->onWrite(result);
    });
}

void Session::send(Message const& message) noexcept {
    trace("send message {} {}", message.id, message.data);
    send(serde::to_binary(message));
}

bool Session::isOpen() const noexcept {
    return m_stream.is_open();
}

Endpoint const& Session::remoteEndpoint() const noexcept {
    return m_remoteEndpoint;
}

void Session::startRead() noexcept {
    if (not m_stream.is_open()) {
        return;
    }
    trace("reading from {}", to_string(remoteEndpoint()));

    m_rBuffer.consume(m_rBuffer.size());
    auto self = shared_from_this();
    m_stream.async_read(m_rBuffer, [self](auto errorCode, auto nReadBytes) {
        if (errorCode == beast::websocket::error::closed) {
            return;
        }
        trace("read {} bytes from {}", nReadBytes, to_string(self->remoteEndpoint()));

        auto result = Result {Result::Success};
        if (errorCode) {
            result.code = Result::Error;
            result.message = fmt::format("{} - {} on endpoint {}", errorCode.value(), errorCode.what(), to_string(self->m_remoteEndpoint));
        }

        self->onRead(result);
    });
}

void Session::onWrite(const Result& result) {
    if (not result.isOk()) {
        error("write error {}", result.message);
        return;
    }
}

void Session::close() noexcept {
    debug("close session from {}", to_string(remoteEndpoint()));
    if (not isOpen()) {
        return;
    }

    ztstl::websocket::beast::error_code ec {};
    m_stream.close(ztstl::websocket::beast::websocket::normal, ec);
    if (ec) {
        error("session close error {} from {}", ec.what(), to_string(remoteEndpoint()));
    }
}

Session::~Session() noexcept {
    close();
}

}// namespace ztstl::websocket::server
