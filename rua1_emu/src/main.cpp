#include <mos6502/cpu_mos6502.h>
#include <mos6502/rom.h>
#include <mos6502/bus.h>
#include <application.h>

int main(int argc, char *argv[])
{
    rua1::application app(argc, argv);
    return app.exec();

    /* RAM   $0000-3FFF (only 1/2 of 32KB available.  Writing to I/O also writes to the upper half of RAM, but
     *                                                you can't read it back since it would interfere with I/O.)
     * ROM   $8000-FFFF (all 32KB available)
     * VIA1  $6000-600F
     * VIA2  $5000-500F
     * VIA3  $4800-480F
     * ACIA1 $4400-4403
     * ACIA2 $4200, 4204, 4208, 420C
     * ACIA3 $4100, 4104, 4108, 410C
     */
    /*
        mos6502::bus bus;
        mos6502::rom rom{0x8000, 0x8000};

        bus.add(rom);

        mos6502::cpu_mos6502 cpu{bus};
        cpu.run();*/
    return 0;
}
