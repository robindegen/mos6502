#pragma once

#include <QtWidgets/QFrame>
#include <memory>
#include <functional>

namespace Ui
{
class frmvia;
}

namespace rua1::view
{

class frmvia_model_interface
{
public:
    frmvia_model_interface(frmvia_model_interface &&) noexcept = delete;
    auto operator=(frmvia_model_interface &&) noexcept -> frmvia_model_interface & = delete;

    frmvia_model_interface(const frmvia_model_interface &) noexcept = delete;
    auto operator=(const frmvia_model_interface &) noexcept -> frmvia_model_interface & = delete;

protected:
    frmvia_model_interface() = default;
    virtual ~frmvia_model_interface() = default;
};

class frmvia final : public QFrame
{
    Q_OBJECT

public:
    explicit frmvia(frmvia_model_interface &model_interface, std::function<void()> close_signal,
                    QWidget *parent = nullptr);
    ~frmvia();

    frmvia(frmvia &&) noexcept = delete;
    auto operator=(frmvia &&) noexcept -> frmvia & = delete;

    frmvia(const frmvia &) noexcept = delete;
    auto operator=(const frmvia &) noexcept -> frmvia & = delete;

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmvia> ui_;
    frmvia_model_interface &model_interface_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
