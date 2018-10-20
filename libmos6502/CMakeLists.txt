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

set(LIBMOS6502_SOURCES
    src/acia_6551.cpp
    include/mos6502/acia_6551.h
    src/bus.cpp
    include/mos6502/bus.h
    src/cpu_mos6502.cpp
    include/mos6502/cpu_mos6502.h
    src/status_registers.h
    include/mos6502/ibus_device.h
    include/mos6502/ibus_interface.h
    include/mos6502/ic_register.h
    src/ram.cpp
    include/mos6502/ram.h
    src/rom.cpp
    include/mos6502/rom.h
    src/via_6522.cpp
    include/mos6502/via_6522.h
    src/memory.cpp
    include/mos6502/memory.h
)

source_group(libmos6502 FILES ${LIBMOS6502_SOURCES})

add_library(libmos6502 STATIC ${LIBMOS6502_SOURCES})

target_include_directories(libmos6502
    PUBLIC include
    PRIVATE src
)

target_link_libraries(libmos6502
    PUBLIC aeon_streams
)

set_target_properties(
    libmos6502 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
