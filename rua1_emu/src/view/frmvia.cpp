#include <view/frmvia.h>
#include <ui_frmvia.h>
#include <QCloseEvent>

namespace rua1::view
{

frmvia::frmvia(frmvia_model_interface &model_interface, std::function<void()> close_signal, QWidget *parent)
    : QFrame(parent)
    , ui_{std::make_unique<Ui::frmvia>()}
    , model_interface_{model_interface}
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
