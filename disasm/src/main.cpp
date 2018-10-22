#include <disasm6502/disasm.h>
#include <aeon/common/string.h>
#include <iostream>
#include <array>
#include <vector>
#include <string>

const std::size_t INIT_BUFFER_SIZE = 1024;

int main(int argc, char *argv[])
{
    // TODO: Grab from arguments.
    const std::uint16_t offset = 0x8000;

    try
    {
        disasm6502::initialize();

        std::freopen(nullptr, "rb", stdin);

        if(std::ferror(stdin))
            throw std::runtime_error(std::strerror(errno));

        std::vector<std::uint8_t> input;
        std::array<char, INIT_BUFFER_SIZE> buf{};
        std::size_t len;

        while((len = std::fread(std::data(buf), 1, std::size(buf), stdin)) > 0)
        {
            if(std::ferror(stdin) && !std::feof(stdin))
                throw std::runtime_error(std::strerror(errno));

            input.insert(std::end(input), std::data(buf), std::data(buf) + len);
        }

        const auto disassembled_file = disasm6502::disassemble(aeon::common::span{input}, offset);

        for (const auto &disassembled : disassembled_file)
        {
            std::string assembly_string;
            for (const auto byte : disassembled.bytes)
            {
                assembly_string += aeon::common::string::int_to_hex_string(byte) + ' ';
            }
            aeon::common::string::trim(assembly_string);

            std::cout << std::left << std::setw(8) << aeon::common::string::int_to_hex_string(disassembled.address);
            std::cout << std::left << std::setw(16) << assembly_string;
            std::cout << disassembled.disassembly;
            std::cout << '\n';
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
