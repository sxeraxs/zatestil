//
// Created by aalisher on 4/2/23.
//
#pragma once

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <util/Result.hpp>
#include <websocket/Server.hpp>

namespace ztstl::websocket::server {

namespace beast = boost::beast;
namespace http = beast::http;
using Result = util::Result;

class Session : public std::enable_shared_from_this<Session> {
   public:
    using FlatBuffer = beast::flat_buffer;
    using Stream = beast::websocket::stream<beast::ssl_stream<beast::tcp_stream>>;

   public:
    Session(std::shared_ptr<Server> server, TcpSocket&& socket, SslContext& context);

    ~Session() noexcept;
    Session(Session&&) = delete;
    Session(Session const&) = delete;
    Session& operator=(Session&&) = delete;
    Session& operator=(Session const&) = delete;

   public:
    void open() noexcept;

    void close() noexcept;

    bool isOpen() const noexcept;

    void write(Message const& message) noexcept;

    Endpoint const& remoteEndpoint() const noexcept;

    void write(std::string_view const& data) noexcept;

   private:
    void onOpen();

    void startRead() noexcept;

    void onRead(Result const& result);

    void onWrite(Result const& result);

    void onAccept(Result const& result);

    void onHandshake(Result const& result);

   private:
    Endpoint m_remoteEndpoint;
    Stream m_stream;
    FlatBuffer m_buffer;
    std::shared_ptr<Server> m_server;
};

}// namespace ztstl::websocket::server
