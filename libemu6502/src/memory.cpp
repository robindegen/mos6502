#include <emu6502/memory.h>
#include <aeon/streams/stream.h>
#include <stdexcept>

namespace emu6502
{

void memory::load(aeon::streams::stream &stream, const std::uint16_t offset)
{
    if (stream.size() + offset > std::size(data_))
        throw std::runtime_error{"Load failed. Data does not fit in memory."};

    stream.read(std::data(data_) + offset, stream.size());
}

memory::memory(const std::uint16_t offset, const std::uint16_t size)
    : offset_{offset}
    , data_(size)
{
}

void memory::write(const std::uint16_t address, const std::uint8_t value) noexcept
{
    const auto offset = static_cast<int>(address) - static_cast<int>(offset_);

    if (offset < 0)
        return;

    if (offset >= static_cast<int>(std::size(data_)))
        return;

    data_[offset] = value;
}

auto memory::read(const std::uint16_t address) noexcept -> std::tuple<bool, std::uint8_t>
{
    const auto offset = static_cast<int>(address) - static_cast<int>(offset_);

    if (offset < 0)
        return {false, static_cast<uint8_t>(0)};

    if (offset >= static_cast<int>(std::size(data_)))
        return {false, static_cast<uint8_t>(0)};

    return {true, data_.at(offset)};
}

} // namespace emu6502
