#include <view/frmcpu.h>
#include <view/ui_utilities.h>
#include <ui_frmcpu.h>
#include <QCloseEvent>

namespace rua1::view
{

frmcpu::frmcpu(frmcpu_model_interface &model_interface, std::function<void()> close_signal, QWidget *parent)
    : QFrame(parent)
    , ui_{std::make_unique<Ui::frmcpu>()}
    , model_interface_{model_interface}
    , close_signal_{std::move(close_signal)}
{
    ui_->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui_->rdo_hex, &QRadioButton::clicked, [this](const auto toggled) {
        if (toggled)
            model_interface_.on_ui_hex_selected();
    });
    connect(ui_->rdo_dec, &QRadioButton::clicked, [this](const auto toggled) {
        if (toggled)
            model_interface_.on_ui_dec_selected();
    });

    connect(ui_->btn_reset, &QPushButton::pressed, [this]() { model_interface_.on_ui_btn_reset_clicked(); });
    connect(ui_->btn_step, &QPushButton::pressed, [this]() { model_interface_.on_ui_btn_step_clicked(); });
}

frmcpu::~frmcpu() = default;

void frmcpu::set_hex_view(const bool val) const
{
    ui_->rdo_hex->setChecked(val);
    ui_->rdo_dec->setChecked(!val);
}

void frmcpu::set_pc_value(const std::uint16_t val) const
{
    ui_->lbl_register_pc->setText(utilities::uint16_to_qstring(val, ui_->rdo_hex->isChecked()));
}

void frmcpu::set_sp_value(const std::uint8_t val) const
{
    ui_->lbl_register_sp->setText(utilities::uint8_to_qstring(val, ui_->rdo_hex->isChecked()));
}

void frmcpu::set_a_value(const std::uint8_t val) const
{
    ui_->lbl_register_a->setText(utilities::uint8_to_qstring(val, ui_->rdo_hex->isChecked()));
}

void frmcpu::set_x_value(const std::uint8_t val) const
{
    ui_->lbl_register_x->setText(utilities::uint8_to_qstring(val, ui_->rdo_hex->isChecked()));
}

void frmcpu::set_y_value(const std::uint8_t val) const
{
    ui_->lbl_register_y->setText(utilities::uint8_to_qstring(val, ui_->rdo_hex->isChecked()));
}

void frmcpu::closeEvent(QCloseEvent *event)
{
    close_signal_();
    event->accept();
}

} // namespace rua1::view
