#include <model/cpu.h>

namespace rua1::model
{

cpu::cpu(const view::imain_window &main_window, mos6502::bus &bus)
    : main_window_{main_window}
    , cpu_{bus}
    , view_{new view::frmcpu{}}
{
    main_window_.add_mdi_child(view_);
}

cpu::~cpu()
{
    main_window_.remove_mdi_child(view_);
}

} // namespace rua1::model
