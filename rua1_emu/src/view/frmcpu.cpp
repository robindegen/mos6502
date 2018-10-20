#include <view/frmcpu.h>
#include <ui_frmcpu.h>
#include <QCloseEvent>

namespace rua1::view
{

frmcpu::frmcpu(std::function<void()> close_signal, QWidget *parent)
    : QDialog(parent)
    , ui_{std::make_unique<Ui::frmcpu>()}
    , close_signal_{std::move(close_signal)}
{
    ui_->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
}

frmcpu::~frmcpu() = default;

void frmcpu::closeEvent(QCloseEvent *event)
{
    close_signal_();
    event->accept();
}

} // namespace rua1::view
