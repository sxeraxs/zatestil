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

namespace ztstl::websocket {

namespace asio = boost::asio;
namespace ssl = asio::ssl;
namespace beast = boost::beast;
using Tcp = asio::ip::tcp;
using Endpoint = Tcp::endpoint;
using TcpSocket = Tcp::socket;
using TcpResolver = Tcp::resolver;
using TcpAcceptor = Tcp::acceptor;
using SslContext = ssl::context;
using Context = boost::asio::io_context;
using Result = util::Result;

std::string to_string(Endpoint const& endpoint) noexcept;
}// namespace ztstl::websocket
