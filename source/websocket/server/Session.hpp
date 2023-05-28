//
// Created by aalisher on 4/2/23.
//
#pragma once

#include <map>

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/thread/synchronized_value.hpp>
#include <log/log.hpp>
#include <util/Result.hpp>
#include <websocket/Server.hpp>
namespace ztstl::websocket::server {

namespace beast = boost::beast;
namespace http = beast::http;
using Result = util::Result;

class Session : public std::enable_shared_from_this<Session>, log_as(websocket) {
   public:
    using FlatBuffer = beast::flat_buffer;
    using Stream = beast::websocket::stream<beast::ssl_stream<beast::tcp_stream>>;

    using Handle = std::function<void(std::string const&)>;

   public:
    Session(std::shared_ptr<Server> server, TcpSocket&& socket, Context& context, SslContext& sslContext);

    ~Session() noexcept;
    Session(Session&&) = delete;
    Session(Session const&) = delete;
    Session& operator=(Session&&) = delete;
    Session& operator=(Session const&) = delete;

   public:
    void open() noexcept;

    void close() noexcept;

    bool isOpen() const noexcept;

    Endpoint const& remoteEndpoint() const noexcept;

    void send(std::string const& data) noexcept;

    bool unsetHandle(size_t handleId) noexcept;

    size_t setHandle(Handle const& handle) noexcept;

   private:
    void onOpen();

    void startRead() noexcept;

    void onRead(Result const& result);

    void onWrite(Result const& result);

    void onAccept(Result const& result);

    void onHandshake(Result const& result);

   private:
    Context& m_context;
    Endpoint m_remoteEndpoint;
    Stream m_stream;
    FlatBuffer m_wBuffer;
    FlatBuffer m_rBuffer;
    Context::strand m_wStrand;
    std::shared_ptr<Server> m_server;
    boost::synchronized_value<std::map<size_t, Handle>> m_handles {};
};

}// namespace ztstl::websocket::server
