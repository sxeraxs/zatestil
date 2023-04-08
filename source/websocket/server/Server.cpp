#include "websocket/Server.hpp"

#include <log/log.hpp>
#include <util/OnScopeExit.hpp>

#include "Session.hpp"

namespace ztstl::websocket {
using namespace server;

Server::Server(Context& context, Config::Ptr config) :
    m_config {config},
    m_context {context},
    m_sslContext {ssl::context::tlsv12},
    m_acceptor(asio::make_strand(context)) {
    m_endpoint = Endpoint(asio::ip::address::from_string(m_config->get<config::Address>()), std::stoul(m_config->get<config::Port>()));

    m_sslContext.set_password_callback([this](auto, auto) {
        return m_config->get<config::SslPassword>();
    });

    m_sslContext.set_options(asio::ssl::context::default_workarounds | asio::ssl::context::no_sslv2 | asio::ssl::context::single_dh_use);

    auto certificate = m_config->get<config::SslCertificate>();
    m_sslContext.use_certificate_chain(asio::buffer(certificate.data(), certificate.size()));

    auto privateKey = m_config->get<config::SslPrivateKey>();
    m_sslContext.use_private_key(asio::buffer(privateKey.data(), privateKey.size()), asio::ssl::context::file_format::pem);

    auto dhParams = m_config->get<config::SslDhParams>();
    m_sslContext.use_tmp_dh(asio::buffer(dhParams.data(), dhParams.size()));

    beast::error_code ec {};
    m_acceptor.open(m_endpoint.protocol(), ec);
    if (ec) {
        error("websocket server open error {} - {}", ec.value(), ec.what());
        return;
    }

    m_acceptor.set_option(asio::socket_base::reuse_address(true), ec);
    if (ec) {
        error("websocket server set error {} - {}", ec.value(), ec.what());
        return;
    }

    m_acceptor.bind(m_endpoint, ec);
    if (ec) {
        error("websocket server bind error {} - {}", ec.value(), ec.what());
        return;
    }

    m_acceptor.listen(asio::socket_base::max_listen_connections, ec);
    if (ec) {
        error("websocket server listen error {} - {}", ec.value(), ec.what());
        return;
    }
}

void Server::startAccept() {
    trace("starting accept on {}", to_string(m_endpoint));
    auto self = shared_from_this();
    m_acceptor.async_accept(asio::make_strand(m_context), [self](auto errorCode, TcpSocket socket) {
        auto result = Result {util::Result::Success};
        if (errorCode) {
            result.code = util::Result::Error;
            result.message = fmt::format("{} - {} on endpoint {}", errorCode.value(), errorCode.what(), to_string(socket.remote_endpoint()));
        }
        self->onAccept(result, std::move(socket));
    });
}

void Server::run() {
    debug("listening on {}", to_string(m_endpoint));
    startAccept();
}

void Server::onAccept(const Result& result, TcpSocket socket) {
    util::OnScopeExit _ {[&] {
        startAccept();
    }};

    if (not result.isOk()) {
        error("websocket server accept error {}", result.message);
        return;
    }

    trace("accepted from {}", to_string(socket.remote_endpoint()));

    auto self = shared_from_this();
    auto session = std::make_shared<Session>(self, std::move(socket), m_context, m_sslContext);
    session->open();
}

void Server::onMessage(std::shared_ptr<Session> const& session, Message const& message) {
    debug("got message {} {} from {}", message.id, message.data, to_string(session->remoteEndpoint()));
    session->send(message);
}

}// namespace ztstl::websocket
