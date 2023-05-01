//
// Created by aalisher on 5/1/23.
//

#pragma once
#include <sodium.h>

#include "Hash.hpp"

namespace ztstl::hash {
class Hasher {
   public:
    ~Hasher() noexcept;
    Hasher() noexcept;

    Hasher(Hasher const&) = delete;
    Hasher(Hasher&&) noexcept = delete;
    Hasher& operator=(Hasher const&) = delete;
    Hasher& operator=(Hasher&&) noexcept = delete;

   public:
    void update(const uint8_t* in, size_t length);

    void update(const std::vector<uint8_t>& in);

    void update(const std::string& str);

    void update(uint8_t in);

    template <typename T>
    void update(T const& v) {
        update(reinterpret_cast<const uint8_t*>(&v), sizeof(T));
    }

    template <std::size_t size>
    void update(Hash<size>& hash) {
        update(hash->data(), size);
    }

    template <std::size_t size>
    Hash<size> hash() {
        Hash<size> h {};
        crypto_hash_sha256_final(&m_state, h.value().data());
        return h;
    }

   private:
    crypto_hash_sha256_state m_state {};
};

}// namespace ztstl::hash
