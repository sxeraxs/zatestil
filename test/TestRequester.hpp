//
// Created by xerax on 5/27/23.
//

#pragma once
#include "TestConfiguration.hpp"
#include "TestSession.hpp"

#include "../source/message/Requester.cpp"

namespace ztstl::message {
template class Requester<test::TestSession, test::TestConfiguration>;
}

namespace ztstl::test {
using TestRequester = ztstl::message::Requester<ztstl::test::TestSession, ztstl::test::TestConfiguration>;
}
