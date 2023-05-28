//
// Created by aalisher on 5/1/23.
//

#pragma once
#include <array>
#include <string>

#include "serde/serde.hpp"
namespace ztstl::hash {

template <size_t size>
class Hash {
   public:
    Hash() noexcept;
    ~Hash() noexcept;
    Hash(Hash const&) = default;
    Hash(Hash&&) noexcept = default;
    Hash& operator=(Hash const&) = default;
    Hash& operator=(Hash&&) noexcept = default;

   private:
    explicit Hash(std::array<uint8_t, size> const& data) noexcept;

   public:
    serializable_as(Hash, m_data);

    static Hash parse(std::string const& hash);

    std::array<uint8_t, size>& value() noexcept;
    std::array<uint8_t, size>const& constValue() const noexcept;

    [[nodiscard]] std::string string() const noexcept;

   private:
    std::array<uint8_t, size> m_data;
};

}// namespace ztstl::hash
