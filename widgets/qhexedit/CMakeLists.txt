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

set(QHEXEDIT_SOURCES
    qhexedit.cpp
    qhexeditprivate.cpp
    paint/qhexmetrics.cpp
    paint/qhexpainter.cpp
    document/gapbuffer.cpp
    document/gapbuffer.h
    document/qhexcursor.cpp
    document/qhexdocument.cpp
    document/qhextheme.cpp
    document/commands/hexcommand.cpp
    document/commands/hexcommand.h
    document/commands/insertcommand.cpp
    document/commands/insertcommand.h
    document/commands/removecommand.cpp
    document/commands/removecommand.h
    document/commands/replacecommand.cpp
    document/commands/replacecommand.h
    document/metadata/qhexmetadata.cpp
    document/metadata/qhexmetadataitem.cpp
)

source_group(sources FILES ${QHEXEDIT_SOURCES})

set(QHEXEDIT_MOC_HEADERS
    qhexedit.h
    qhexeditprivate.h
    paint/qhexmetrics.h
    paint/qhexpainter.h
    document/qhexcursor.h
    document/qhexdocument.h
    document/qhextheme.h
    document/metadata/qhexmetadata.h
    document/metadata/qhexmetadataitem.h
)

source_group(sources FILES ${QHEXEDIT_MOC_HEADERS})

qt5_wrap_cpp(QHEXEDIT_MOC_SOURCES
    ${QHEXEDIT_MOC_HEADERS}
)

source_group(generated FILES ${QHEXEDIT_MOC_SOURCES})

###############################################################################

add_library(qhexedit STATIC
    ${QHEXEDIT_SOURCES}
    ${QHEXEDIT_MOC_HEADERS}
    ${QHEXEDIT_MOC_SOURCES}
)

target_include_directories(qhexedit
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(qhexedit
    Qt5::Core
    Qt5::Widgets
)

set_target_properties(
    qhexedit PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
