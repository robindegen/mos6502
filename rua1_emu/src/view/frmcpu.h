#pragma once

#include <QtWidgets/QFrame>
#include <memory>
#include <functional>

namespace Ui
{
class frmcpu;
}

namespace rua1::view
{

class frmcpu_model_interface
{
public:
    frmcpu_model_interface(frmcpu_model_interface &&) noexcept = delete;
    auto operator=(frmcpu_model_interface &&) noexcept -> frmcpu_model_interface & = delete;

    frmcpu_model_interface(const frmcpu_model_interface &) noexcept = delete;
    auto operator=(const frmcpu_model_interface &) noexcept -> frmcpu_model_interface & = delete;

    virtual void on_ui_btn_reset_clicked() = 0;
    virtual void on_ui_btn_step_clicked() = 0;

    virtual void on_ui_hex_selected() = 0;
    virtual void on_ui_dec_selected() = 0;

protected:
    frmcpu_model_interface() = default;
    virtual ~frmcpu_model_interface() = default;
};

class frmcpu final : public QFrame
{
    Q_OBJECT

public:
    explicit frmcpu(frmcpu_model_interface &model_interface, std::function<void()> close_signal,
                    QWidget *parent = nullptr);
    ~frmcpu();

    frmcpu(frmcpu &&) noexcept = delete;
    auto operator=(frmcpu &&) noexcept -> frmcpu & = delete;

    frmcpu(const frmcpu &) noexcept = delete;
    auto operator=(const frmcpu &) noexcept -> frmcpu & = delete;

    void set_hex_view(const bool val) const;

    void set_pc_value(const std::uint16_t val) const;
    void set_sp_value(const std::uint8_t val) const;
    void set_a_value(const std::uint8_t val) const;
    void set_x_value(const std::uint8_t val) const;
    void set_y_value(const std::uint8_t val) const;

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmcpu> ui_;
    frmcpu_model_interface &model_interface_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
