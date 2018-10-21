#include <view/ui_utilities.h>
#include <aeon/common/string.h>

namespace rua1::view::utilities
{

template <typename T>
static auto int_to_qstring(const T val, const bool hex) -> QString
{
    if (hex)
    {
        auto str = aeon::common::string::int_to_hex_string(val);
        aeon::common::string::to_upper(str);
        return QString::fromStdString(str);
    }

    return QString::number(static_cast<int>(val));
}

auto uint8_to_qstring(const std::uint8_t val, const bool hex) -> QString
{
    return int_to_qstring(val, hex);
}

auto uint16_to_qstring(const std::uint16_t val, const bool hex) -> QString
{
    return int_to_qstring(val, hex);
}

} // namespace rua1::view::utilities
