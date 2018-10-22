#pragma once

#include <QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <widgets/disasm_widget.h>

namespace rua1::widgets_tests
{

class frmmain final : public QMainWindow
{
    Q_OBJECT

public:
    frmmain();
    ~frmmain() = default;

    frmmain(frmmain &&) noexcept = delete;
    auto operator=(frmmain &&) noexcept -> frmmain & = delete;

    frmmain(const frmmain &) noexcept = delete;
    auto operator=(const frmmain &) noexcept -> frmmain & = delete;

private:
    widgets::disasm_widget *disasm_widget_;
};

} // namespace rua1::widgets_tests
