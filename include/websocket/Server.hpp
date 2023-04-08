//
// Created by aalisher on 4/2/23.
//
#pragma once
#include <memory>

#include <config/ServiceConfiguration.hpp>
#include <log/log.hpp>
#include <util/Result.hpp>

#include "Message.hpp"
#include "websocket.hpp"

namespace ztstl::websocket {
namespace server {
    class Session;
}

class Server : public std::enable_shared_from_this<Server>, log_as(websocket) {
   public:
    using Config = config::ServiceConfiguration;
    friend class server::Session;
    friend class util::Singleton<Server>;

   public:
    Server(Context& context, Config::Ptr config);
    ~Server() noexcept = default;
    Server(Server&&) = delete;
    Server(Server const&) = delete;
    Server& operator=(Server&&) = delete;
    Server& operator=(Server const&) = delete;

   public:
    void run();

   private:
    void startAccept();

    void onAccept(util::Result const& result, TcpSocket socket);

    void onMessage(std::shared_ptr<server::Session> const& session, Message const& message);

   private:
    Config::Ptr m_config;
    Context& m_context;
    Endpoint m_endpoint;
    TcpAcceptor m_acceptor;
    SslContext m_sslContext;
};

}// namespace ztstl::websocket
