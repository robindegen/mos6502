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

void application::remove_mdi_child(QWidget *widget) const noexcept
{
    frmmain_.remove_mdi_child(widget);
}

} // namespace rua1
