# Copyright (c) 2012-2018 Robin Degen
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

set(RUA1_EMU_SOURCES
    src/main.cpp
    src/application.cpp
    src/application.h
    src/configuration.cpp
    src/configuration.h
)

source_group(main FILES ${RUA1_EMU_SOURCES})

###############################################################################

set(RUA1_EMU_MODEL_SOURCES
    src/model/acia_6551.cpp
    src/model/acia_6551.h
    src/model/component.h
    src/model/computer.cpp
    src/model/computer.h
    src/model/cpu.cpp
    src/model/cpu.h
    src/model/ram.cpp
    src/model/ram.h
    src/model/rom.cpp
    src/model/rom.h
    src/model/sidebar_toggleable.h
    src/model/via_6522.cpp
    src/model/via_6522.h
)

source_group(model FILES ${RUA1_EMU_MODEL_SOURCES})

###############################################################################

set(RUA1_EMU_VIEW_SOURCES
    src/view/frmacia.cpp
    src/view/frmvia.cpp
    src/view/frmmain.cpp
    src/view/frmcpu.cpp
    src/view/frmmemory.cpp
    src/view/sidebar_toggle_button.cpp
    src/view/imain_window.h
    src/view/ui_utilities.cpp
    src/view/ui_utilities.h
)

source_group(view FILES ${RUA1_EMU_VIEW_SOURCES})

set(RUA1_EMU_VIEW_MOC_HEADERS
    src/view/frmacia.h
    src/view/frmvia.h
    src/view/frmmain.h
    src/view/frmcpu.h
    src/view/frmmemory.h
    src/view/sidebar_toggle_button.h
)

source_group(view FILES ${RUA1_EMU_VIEW_MOC_HEADERS})

qt5_wrap_cpp(RUA1_EMU_VIEW_MOC_SOURCES
    ${RUA1_EMU_VIEW_MOC_HEADERS}
)

source_group(view\\generated FILES ${RUA1_EMU_VIEW_MOC_SOURCES})

set(RUA1_EMU_VIEW_UI_FILES
    src/view/ui/frmacia.ui
    src/view/ui/frmvia.ui
    src/view/ui/frmmain.ui
    src/view/ui/frmcpu.ui
    src/view/ui/frmmemory.ui
)

source_group(view\\ui FILES ${RUA1_EMU_VIEW_UI_FILES})

qt5_wrap_ui(RUA1_EMU_VIEW_UI_HEADERS
    ${RUA1_EMU_VIEW_UI_FILES}
)

source_group(view\\generated FILES ${RUA1_EMU_VIEW_UI_HEADERS})

###############################################################################
add_executable(rua1_emu
    ${RUA1_EMU_SOURCES}
    ${RUA1_EMU_MODEL_SOURCES}
    ${RUA1_EMU_VIEW_SOURCES}
    ${RUA1_EMU_VIEW_MOC_HEADERS}
    ${RUA1_EMU_VIEW_MOC_SOURCES}
    ${RUA1_EMU_VIEW_UI_HEADERS}
)

target_compile_definitions(rua1_emu PRIVATE QT_NO_KEYWORDS)

target_include_directories(rua1_emu
    PUBLIC
        src
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(rua1_emu
    aeon_common
    aeon_streams
    libmos6502
    json11
    Qt5::Core
    Qt5::Widgets
)

set_target_properties(
    rua1_emu PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
