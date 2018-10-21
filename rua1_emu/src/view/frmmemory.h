#pragma once

#include <QtWidgets/QFrame>
#include <memory>
#include <functional>

namespace Ui
{
class frmmemory;
}

namespace rua1::view
{

class frmmemory_model_interface
{
public:
    frmmemory_model_interface(frmmemory_model_interface &&) noexcept = delete;
    auto operator=(frmmemory_model_interface &&) noexcept -> frmmemory_model_interface & = delete;

    frmmemory_model_interface(const frmmemory_model_interface &) noexcept = delete;
    auto operator=(const frmmemory_model_interface &) noexcept -> frmmemory_model_interface & = delete;

protected:
    frmmemory_model_interface() = default;
    virtual ~frmmemory_model_interface() = default;
};

class frmmemory final : public QFrame
{
    Q_OBJECT

public:
    explicit frmmemory(frmmemory_model_interface &model_interface, std::function<void()> close_signal,
                       QWidget *parent = nullptr);
    ~frmmemory();

    frmmemory(frmmemory &&) noexcept = delete;
    auto operator=(frmmemory &&) noexcept -> frmmemory & = delete;

    frmmemory(const frmmemory &) noexcept = delete;
    auto operator=(const frmmemory &) noexcept -> frmmemory & = delete;

private:
    void closeEvent(QCloseEvent *event) override;

    std::unique_ptr<Ui::frmmemory> ui_;
    frmmemory_model_interface &model_interface_;
    std::function<void()> close_signal_;
};

} // namespace rua1::view
