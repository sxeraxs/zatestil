#include "ServiceApplication.hpp"
#include "config/ServiceConfiguration.hpp"

#include "../run.hpp"

int main(int argc, char const** argv) {
    using ztstl::service::ServiceApplication;
    using ztstl::config::ServiceConfiguration;
    return ztstl::run<ServiceApplication, ServiceConfiguration>(argc, argv);
}
