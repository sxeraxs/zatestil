//
// Created by aalisher on 5/1/23.
//
#include <hash/Hasher.hpp>
namespace ztstl::hash {
Hasher::Hasher() noexcept {
    crypto_hash_sha256_init(&m_state);
}

Hasher::~Hasher() noexcept {}

void Hasher::update(const uint8_t* in, size_t length)
{
    crypto_hash_sha256_update(&m_state, in, length);
}

void Hasher::update(const std::vector<uint8_t>& in)
{
    crypto_hash_sha256_update(&m_state, in.data(), in.size());
}

void Hasher::update(const std::string& str)
{
    crypto_hash_sha256_update(&m_state, reinterpret_cast<const unsigned char*>(str.c_str()), str.length());
}

void Hasher::update(uint8_t in)
{
    crypto_hash_sha256_update(&m_state, &in, 1);
}

}// namespace ztstl::hash
