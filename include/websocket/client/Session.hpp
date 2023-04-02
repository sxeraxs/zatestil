//
// Created by aalisher on 4/2/23.
//

#pragma once
#include <memory>

#include <websocket/websocket.hpp>

namespace ztstl::bot {
class BotApplication;
}// namespace ztstl::bot

namespace ztstl::websocket::client {

using BotApplicationPtr = bot::BotApplication*;

class Session : public std::enable_shared_from_this<Session> {
   public:
    using FlatBuffer = beast::flat_buffer;
    using Stream = beast::websocket::stream<beast::ssl_stream<beast::tcp_stream>>;

   public:
    Session(BotApplicationPtr application, Endpoint endpoint, Context& context, SslContext& sslContext);

    ~Session() noexcept;
    Session(Session&&) = delete;
    Session(Session const&) = delete;
    Session& operator=(Session&&) = delete;
    Session& operator=(Session const&) = delete;

   public:
    Result open() noexcept;

    Result close() noexcept;

    bool isOpen() const noexcept;

    void write(Message const& message) noexcept;

    Endpoint const& remoteEndpoint() const noexcept;

    void write(std::string_view const& data) noexcept;

   private:
    void startRead() noexcept;

    void onRead(Result const& result);

    void onWrite(Result const& result);

   private:
    Endpoint m_remoteEndpoint;
    Stream m_stream;
    Endpoint m_endpoint;
    FlatBuffer m_buffer;
    BotApplicationPtr m_application;
};

}// namespace ztstl::websocket::client
