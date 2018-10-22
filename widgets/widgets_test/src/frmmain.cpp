#include <frmmain.h>

namespace rua1::widgets_tests
{

frmmain::frmmain()
    : QMainWindow{}
    , disasm_widget_{new widgets::disasm_widget{}}
{
    setCentralWidget(disasm_widget_);
}

} // namespace rua1::widgets_tests
