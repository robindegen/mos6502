#include <view/frmmemory.h>
#include <ui_frmmemory.h>
#include <QCloseEvent>

namespace rua1::view
{

frmmemory::frmmemory(frmmemory_model_interface &model_interface, std::function<void()> close_signal, QWidget *parent)
    : QFrame(parent)
    , ui_{std::make_unique<Ui::frmmemory>()}
    , model_interface_{model_interface}
    , close_signal_{std::move(close_signal)}
{
    ui_->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui_->btn_hex_view, &QPushButton::pressed, [this]() { model_interface_.on_ui_btn_hex_view_clicked(); });
}

frmmemory::~frmmemory() = default;

void frmmemory::closeEvent(QCloseEvent *event)
{
    close_signal_();
    event->accept();
}

} // namespace rua1::view
