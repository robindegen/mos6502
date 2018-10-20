#include <model/cpu.h>
#include <QtWidgets>
#include <cassert>

namespace rua1::model
{

cpu::cpu(view::imain_window &main_window, mos6502::bus &bus)
    : main_window_{main_window}
    , cpu_{bus}
    , view_{}
    , sidebar_button_{
          main_window.register_toggle_button("CPU", true, [this](auto checked) { on_sidebar_button_toggled(checked); })}
{
    create_view();
}

cpu::~cpu() = default;

void cpu::create_view()
{
    assert(!view_);
    view_ = new view::frmcpu{[this]() { on_view_closed(); }};
    main_window_.add_mdi_child(view_);
    view_->show();
}

void cpu::destroy_view()
{
    assert(view_);
    view_->parentWidget()->close();
    view_ = nullptr;
}

void cpu::on_sidebar_button_toggled(const bool checked)
{
    if (checked)
    {
        create_view();
    }
    else
    {
        destroy_view();
    }
}

void cpu::on_view_closed() const
{
    sidebar_button_->set_checked(false);
}

} // namespace rua1::model
