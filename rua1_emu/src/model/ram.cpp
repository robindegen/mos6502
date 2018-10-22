#include <model/ram.h>
#include <aeon/streams/file_stream.h>
#include <document/qhexdocument.h>
#include <qhexedit.h>

namespace rua1::model
{

ram::ram(view::imain_window &main_window, const config::ram_device_config &config)
    : sidebar_toggleable<view::frmmemory, view::frmmemory_model_interface>{*this, config.name(), main_window}
    , ram_{static_cast<std::uint16_t>(config.offset()), static_cast<std::uint16_t>(config.size())}
{
}

ram::~ram() = default;

auto ram::get_device() noexcept -> mos6502::ibus_device &
{
    return ram_;
}

void ram::on_view_created()
{
}

void ram::on_view_destroyed()
{
}

void ram::on_ui_btn_hex_view_clicked()
{
    auto document = QHexDocument::fromMemory(reinterpret_cast<const char *>(ram_.data()), ram_.size());
    auto hexedit = new QHexEdit{};
    hexedit->setDocument(document);
    add_mdi_child(hexedit);
    hexedit->show();
}

} // namespace rua1::model
