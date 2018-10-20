#include <application.h>

namespace rua1
{

application::application(int argc, char *argv[])
    : application_{argc, argv}
    , frmmain_{}
    , computer_{}
{
}

application::~application() = default;

auto application::exec() -> int
{
    // Should happen from file->open
    computer_ = std::make_unique<model::computer>(*this, "C:/Development/6502/mos6502/config.json");

    frmmain_.show();
    return QApplication::exec();
}

void application::add_mdi_child(QWidget *widget, Qt::WindowFlags flags) const noexcept
{
    frmmain_.add_mdi_child(widget, flags);
}

auto application::register_toggle_button(const std::string &text, const bool default_state,
                                         std::function<void(const bool)> on_toggle) -> view::sidebar_toggle_button *
{
    return frmmain_.register_toggle_button(text, default_state, std::move(on_toggle));
}

void application::remove_toggle_button(const view::sidebar_toggle_button *button)
{
    frmmain_.remove_toggle_button(button);
}

void application::remove_all_toggle_buttons()
{
    frmmain_.remove_all_toggle_buttons();
}

} // namespace rua1
