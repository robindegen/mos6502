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

set(LIBWIDGETS_SOURCES
    src/disasm_widget.cpp
)

source_group(widgets FILES ${LIBWIDGETS_SOURCES})

set(LIBWIDGETS_MOC_HEADERS
    include/widgets/disasm_widget.h
)

source_group(widgets FILES ${LIBWIDGETS_MOC_HEADERS})

qt5_wrap_cpp(LIBWIDGETS_MOC_SOURCES
    ${LIBWIDGETS_MOC_HEADERS}
)

source_group(generated FILES ${LIBWIDGETS_MOC_SOURCES})

add_library(libwidgets STATIC
    ${LIBWIDGETS_SOURCES}
    ${LIBWIDGETS_MOC_HEADERS}
    ${LIBWIDGETS_MOC_SOURCES}
)

target_include_directories(libwidgets
    PUBLIC
        include
    PRIVATE
        src
)

target_link_libraries(libwidgets
    aeon_common
    Qt5::Core
    Qt5::Widgets
)

set_target_properties(
    libwidgets PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
