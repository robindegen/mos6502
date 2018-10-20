#include <view/frmmemory.h>
#include <ui_frmmemory.h>
#include <QCloseEvent>

namespace rua1::view
{

frmmemory::frmmemory(std::function<void()> close_signal, QWidget *parent)
    : QFrame(parent)
    , ui_{std::make_unique<Ui::frmmemory>()}
    , close_signal_{std::move(close_signal)}
{
    ui_->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

frmmemory::~frmmemory() = default;

void frmmemory::closeEvent(QCloseEvent *event)
{
    close_signal_();
    event->accept();
}

} // namespace rua1::view
