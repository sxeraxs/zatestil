//
// Created by aalisher on 5/1/23.
//
#include <stdexcept>

#include <hash/Hash.hpp>
namespace ztstl::hash {
template <size_t size>
Hash<size>::Hash() noexcept {
    m_data.fill(0);
}

template <size_t size>
Hash<size>::Hash(const std::array<uint8_t, size>& data) noexcept : m_data {data} {}

template <size_t size>
Hash<size>::~Hash() noexcept {
    m_data.fill(0);
}

template <size_t size>
std::array<uint8_t, size> const& Hash<size>::constValue() const noexcept {
    return m_data;
}

template <size_t size>
std::array<uint8_t, size>& Hash<size>::value() noexcept {
    return m_data;
}

inline bool char_from_hex(const char c, uint8_t& value) {
    if (c >= '0' && c <= '9') {
        value = c - '0';
        return true;
    }

    if (c >= 'a' && c <= 'f') {
        value = c - 'a' + 10;
        return true;
    }

    if (c >= 'A' && c <= 'F') {
        value = c - 'A' + 10;
        return true;
    }

    return false;
}

inline uint8_t char_from_hex(char c) {
    uint8_t ret;
    bool res = char_from_hex(c, ret);
    if (!res)
        throw std::runtime_error("invalid hex character");
    return ret;
}

template <size_t size>
Hash<size> Hash<size>::parse(std::string const& hash) {
    if (hash.length() != size * 2) {
        throw std::runtime_error("invalid hash string length");
    }

    std::array<uint8_t, size> data {};
    uint8_t* ptr = data.data();
    for (std::size_t i = 0; i < size; ++i, ++ptr) {
        char hi = hash[2 * i];
        char lo = hash[2 * i + 1];
        *ptr = (char_from_hex(hi) << 4) + char_from_hex(lo);
    }

    return Hash<size> {data};
}

template <size_t size>
std::string Hash<size>::string() const noexcept {
    std::string buf;
    buf.resize(2 * size);
    const uint8_t* ptr = m_data.data();
    for (std::size_t i = 0; i < size; ++i, ++ptr) {
        std::sprintf(&buf[2 * i], "%02x", *ptr);
    }
    return buf;
}

}// namespace ztstl::hash
