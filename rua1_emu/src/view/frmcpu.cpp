#include <view/frmcpu.h>
#include <ui_frmcpu.h>

namespace rua1::view
{

frmcpu::frmcpu(QWidget *parent)
    : QDialog(parent)
    , ui_{std::make_unique<Ui::frmcpu>()}
{
    ui_->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
}

frmcpu::~frmcpu() = default;

} // namespace rua1::view
