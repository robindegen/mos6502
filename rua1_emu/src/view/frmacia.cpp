#include <view/frmacia.h>
#include <ui_frmacia.h>
#include <QCloseEvent>

namespace rua1::view
{

frmacia::frmacia(std::function<void()> close_signal, QWidget *parent)
    : QFrame(parent)
    , ui_{std::make_unique<Ui::frmacia>()}
    , close_signal_{std::move(close_signal)}
{
    ui_->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

frmacia::~frmacia() = default;

void frmacia::closeEvent(QCloseEvent *event)
{
    close_signal_();
    event->accept();
}

} // namespace rua1::view
