CLANG_FORMAT="clang-format"

function format {
    DIRECTORY=$1
    echo "Formatting code under $DIRECTORY/"
    find "$DIRECTORY" \( -name '*.h' -or -name '*.cpp' \) -print0 | xargs -0 "$CLANG_FORMAT" -i
}

format $(pwd)/libemu6502
format $(pwd)/libdisasm6502
format $(pwd)/widgets
format $(pwd)/rua1_emu
