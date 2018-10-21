#pragma once

#include <QString>

namespace rua1::view::utilities
{

auto uint8_to_qstring(const std::uint8_t val, const bool hex) -> QString;
auto uint16_to_qstring(const std::uint16_t val, const bool hex) -> QString;

} // namespace rua1::view::utilities
