//
// Created by aalisher on 4/9/23.
//
#include "database/Database.hpp"

#include <mdbx.h++>
namespace ztstl::database {

template <Id id>
Database<id>::Database(std::filesystem::path const& path) {
    mdbx::env_managed::create_parameters createParams {};
    createParams.use_subdirectory = true;

    mdbx::env::operate_parameters opParams {};
    opParams.max_maps = 100;
    m_env = std::make_shared<mdbx::env_managed>(path, createParams, opParams, false);
    debug("open database {}", std::filesystem::canonical(std::filesystem::absolute(path)).string());
}

template <Id id>
Database<id>::~Database() {
    std::filesystem::path path = m_env->get_path();
    m_env->close();

    std::error_code errorCode {};
    path = std::filesystem::canonical(std::filesystem::absolute(path, errorCode), errorCode);
    debug("close database {}", path.string());
}

template <Id id>
Reader<id> Database<id>::makeReader() noexcept {
    return Reader<id>(*this);
}

template <Id id>
Writer<id> Database<id>::makeWriter() noexcept {
    return Writer<id>(*this);
}

}// namespace ztstl::database
