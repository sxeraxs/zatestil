//
// Created by aalisher on 5/1/23.
//
#include <filesystem>

#include <database/Table.hpp>
#include <storage/Storage.hpp>
namespace ztstl::storage {
namespace fs = std::filesystem;
Storage::Storage(ztstl::config::ServiceConfiguration* config) noexcept {
    fs::path root = config->get<config::DataDir>();
    info("open storage {}", root.string());
    m_root = root.string();
    if (not fs::exists(root)) {
        fs::create_directories(root);
    }

    m_main = MainDb::instance(root / "main");
    m_blocks = m_main->openTable<object::Block, Id::_0>("blocks");
    m_accounts = m_main->openTable<object::Account, Id::_1>("accounts");
}

Storage::~Storage() noexcept {
    info("close storage {}", m_root);
}

}// namespace ztstl::storage
