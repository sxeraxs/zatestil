//
// Created by aalisher on 4/2/23.
//

#pragma once
#include <map>
#include <memory>

#include <boost/thread/synchronized_value.hpp>
#include <log/log.hpp>
#include <websocket/websocket.hpp>
namespace ztstl::bot {
class BotApplication;
}// namespace ztstl::bot

namespace ztstl::websocket::client {

using BotApplicationPtr = bot::BotApplication*;

class Session : public std::enable_shared_from_this<Session>, log_as(websocket) {
   public:
    using FlatBuffer = beast::flat_buffer;
    using Stream = beast::websocket::stream<beast::ssl_stream<beast::tcp_stream>>;

    using Handle = std::function<void(std::string const&)>;

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

    Endpoint const& remoteEndpoint() const noexcept;

    void send(std::string const& data) noexcept;

    bool unsetHandle(size_t handleId) noexcept;

    size_t setHandle(Handle const& handle) noexcept;

   private:
    void startRead() noexcept;

    void onRead(Result const& result);

    void onWrite(Result const& result);

   private:
    Context& m_context;
    Endpoint m_remoteEndpoint;
    Stream m_stream;
    Endpoint m_endpoint;
    FlatBuffer m_rBuffer;
    std::string m_wBuffer;
    Context::strand m_wStrand;
    BotApplicationPtr m_application;
    boost::synchronized_value<std::map<size_t, Handle>> m_handles {};
};

}// namespace ztstl::websocket::client
