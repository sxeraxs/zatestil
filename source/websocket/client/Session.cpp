//
// Created by aalisher on 4/2/23.
//

#include <string>
#include <utility>

#include <application/bot/BotApplication.hpp>
#include <log/log.hpp>
#include <util/OnScopeExit.hpp>
#include <util/serde.hpp>
#include <websocket/Message.hpp>
#include <websocket/client/Session.hpp>

namespace ztstl::websocket::client {

Session::Session(BotApplicationPtr application, Endpoint endpoint, Context& context, SslContext& sslContext) :
    m_endpoint {std::move(endpoint)},
    m_context {context},
    m_application {application},
    m_wStrand {context},
    m_stream {asio::make_strand(context), sslContext} {};

Result Session::open() noexcept {
    auto& lowestLayer = beast::get_lowest_layer(m_stream);
    lowestLayer.expires_after(std::chrono::seconds(30));

    beast::error_code ec {};
    lowestLayer.connect(m_endpoint, ec);
    if (ec) {
        return {util::Result::Error, fmt::format("websocket client session open error {}", ec.what())};
    }

    lowestLayer.expires_after(std::chrono::seconds(30));

    auto& nextLayer = m_stream.next_layer();
    nextLayer.handshake(ssl::stream_base::client, ec);
    if (ec) {
        return {util::Result::Error, fmt::format("websocket client session handshake error {}", ec.what())};
    }

    lowestLayer.expires_never();
    m_stream.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::client));

    m_stream.set_option(beast::websocket::stream_base::decorator([](auto& req) {
        req.set(beast::http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-async-ssl");
    }));

    m_stream.handshake(m_endpoint.address().to_string(), "/", ec);
    if (ec) {
        return {util::Result::Error, fmt::format("websocket client session perform handshake error {}", ec.what())};
    }

    startRead();
    return {util::Result::Success};
}

Result Session::close() noexcept {
    beast::error_code ec {};
    m_stream.close(beast::websocket::close_code::normal, ec);
    if (ec) {
        return {util::Result::Error, fmt::format("websocket client session close error {}", ec.what())};
    }

    return {util::Result::Success};
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
        self->m_application->onMessage(self, message);
    });
}

void Session::send(std::string const& data) noexcept {
    m_stream.binary(true);
    auto self = shared_from_this();
    boost::asio::post(m_wStrand, [self, data]() mutable {
        self->m_wBuffer = data;
        beast::error_code errorCode {};
        self->m_stream.write(asio::buffer(self->m_wBuffer), errorCode);
        auto result = Result {util::Result::Success};
        if (errorCode) {
            result.code = util::Result::Error;
            result.message = fmt::format("{} - {} on endpoint {}", errorCode.value(), errorCode.what(), to_string(self->m_remoteEndpoint));
        }

        self->onWrite(result);
    });
}

void Session::onWrite(const Result& result) {
    if (not result.isOk()) {
        error("write error {}", result.message);
        return;
    }
}

void Session::send(Message const& message) noexcept {
    trace("send message {} {}", message.id, message.data);
    send(serde::to_binary(message));
}

Endpoint const& Session::remoteEndpoint() const noexcept {
    return m_endpoint;
}

bool Session::isOpen() const noexcept {
    return m_stream.is_open();
}

Session::~Session() noexcept {
    close();
}

}// namespace ztstl::websocket::client
