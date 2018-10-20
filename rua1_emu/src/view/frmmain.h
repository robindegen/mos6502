#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>
#include <vector>

namespace Ui
{
class frmmain;
}

namespace rua1::view
{

class frmmain final : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmmain();
    ~frmmain();

    frmmain(frmmain &&) noexcept = delete;
    auto operator=(frmmain &&) noexcept -> frmmain & = delete;

    frmmain(const frmmain &) noexcept = delete;
    auto operator=(const frmmain &) noexcept -> frmmain & = delete;

    void add_mdi_child(QWidget *widget, Qt::WindowFlags flags = Qt::WindowFlags{}) const noexcept;
    void remove_mdi_child(QWidget *widget) const noexcept;

private:
    std::unique_ptr<Ui::frmmain> ui_;
};

} // namespace rua1::view
