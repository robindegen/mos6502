#include <view/frmmain.h>
#include <configuration.h>
#include <ui_frmmain.h>
#include <QPushButton>

namespace rua1::view
{

frmmain::frmmain()
    : ui_{std::make_unique<Ui::frmmain>()}
{
    ui_->setupUi(this);

    ui_->mdi_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui_->mdi_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

frmmain::~frmmain() = default;

void frmmain::add_mdi_child(QWidget *widget, Qt::WindowFlags flags) const noexcept
{
    ui_->mdi_area->addSubWindow(widget, flags);
}

void frmmain::remove_mdi_child(QWidget *widget) const noexcept
{
    ui_->mdi_area->removeSubWindow(widget);
}

} // namespace rua1::view
