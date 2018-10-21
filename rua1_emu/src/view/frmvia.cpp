#include <view/frmvia.h>
#include <ui_frmvia.h>
#include <QCloseEvent>

namespace rua1::view
{

frmvia::frmvia(std::function<void()> close_signal, QWidget *parent)
    : QFrame(parent)
    , ui_{std::make_unique<Ui::frmvia>()}
    , close_signal_{std::move(close_signal)}
{
    ui_->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

frmvia::~frmvia() = default;

void frmvia::closeEvent(QCloseEvent *event)
{
    close_signal_();
    event->accept();
}

} // namespace rua1::view
