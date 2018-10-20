#pragma once

#include <mos6502/ibus_device.h>
#include <aeon/streams/stream_fwd.h>
#include <vector>

namespace mos6502
{

class memory : public ibus_device
{
public:
    virtual ~memory() = default;

    memory(memory &&) noexcept = delete;
    auto operator=(memory &&) noexcept -> memory & = delete;

    memory(const memory &) noexcept = delete;
    auto operator=(const memory &) noexcept -> memory & = delete;

    void load(aeon::streams::stream &stream, const std::uint16_t offset = 0);

protected:
    explicit memory(const std::uint16_t offset, const std::uint16_t size);

    void write(const std::uint16_t address, const std::uint8_t value) noexcept override;
    auto read(const std::uint16_t address) noexcept -> std::tuple<bool, std::uint8_t> override;

    std::uint16_t offset_;
    std::vector<std::uint8_t> data_;
};

} // namespace mos6502
