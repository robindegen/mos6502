#pragma once

#include <mos6502/ibus_device.h>
#include <mos6502/ic_register.h>

namespace mos6502
{

struct via_6522_settings
{
    std::uint16_t iorb_register_address_{};
    std::uint16_t iora_register_address_{};
    std::uint16_t ddrb_register_address_{};
    std::uint16_t ddra_register_address_{};
    std::uint16_t t1cl_register_address_{};
    std::uint16_t t1ch_register_address_{};
    std::uint16_t t1ll_register_address_{};
    std::uint16_t t1lh_register_address_{};
    std::uint16_t t2cl_register_address_{};
    std::uint16_t t2ch_register_address_{};
    std::uint16_t sr_register_address_{};
    std::uint16_t acr_register_address_{};
    std::uint16_t pcr_register_address_{};
    std::uint16_t ifr_register_address_{};
    std::uint16_t ier_register_address_{};
    std::uint16_t iora_no_handshake_register_address_{};
};

class via_6522 : public ibus_device
{
public:
    explicit via_6522(const via_6522_settings settings) noexcept;
    virtual ~via_6522() = default;

    via_6522(via_6522 &&) noexcept = delete;
    auto operator=(via_6522 &&) noexcept -> via_6522 & = delete;

    via_6522(const via_6522 &) noexcept = delete;
    auto operator=(const via_6522 &) noexcept -> via_6522 & = delete;

private:
    void write(const std::uint16_t address, const std::uint8_t value) noexcept override;
    auto read(const std::uint16_t address) noexcept -> std::tuple<bool, std::uint8_t> override;

    ic_register iorb_register_;
    ic_register iora_register_;
    ic_register ddrb_register_;
    ic_register ddra_register_;

    ic_register t1cl_register_;
    ic_register t1ch_register_;
    ic_register t1ll_register_;
    ic_register t1lh_register_;

    ic_register t2cl_register_;
    ic_register t2ch_register_;

    ic_register sr_register_;

    ic_register acr_register_;
    ic_register pcr_register_;
    ic_register ifr_register_;
    ic_register ier_register_;
    ic_register iora_no_handshake_register_;
};

} // namespace mos6502
