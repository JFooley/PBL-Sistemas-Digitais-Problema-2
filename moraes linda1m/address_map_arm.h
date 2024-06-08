/* Mascaras para extrair os diferentes campos */
#define OPCODE_MASK          0b1111               // 4 bits
#define REG_MASK             0b11111              // 5 bits
#define OFFSET_MASK          0b111111111          // 9 bits
#define SMEN_OFFSET_MASK     0b11111111111111     // 14 bits
#define BMEN_OFFSET_MASK     0b111111111111       // 12 bits
#define DPMEN_OFFSET_MASK    0b1111               // 4 bits
#define REF_POINT_MASK       0b111111111          // 9 bits

/* Instrução */
#define WBR_OPCODE           0b0000
#define WSM_OPCODE           0b0001
#define WBM_OPCODE           0b0010
#define DP_OPCODE            0b0011

/* Enderecos da GPU */
#define DATA_A               0x80   // Registrador com opcode e endereçamentos
#define DATA_B               0x70   // Registradores de dados
#define START                0xc0   // WRREG

/* Memory */
#define DDR_BASE             0x00000000
#define DDR_SPAN             0x3FFFFFFF
#define A9_ONCHIP_BASE       0xFFFF0000
#define A9_ONCHIP_SPAN       0x0000FFFF
#define SDRAM_BASE           0xC0000000
#define SDRAM_SPAN           0x03FFFFFF
#define FPGA_ONCHIP_BASE     0xC8000000
#define FPGA_ONCHIP_SPAN     0x0003FFFF
#define FPGA_CHAR_BASE       0xC9000000
#define FPGA_CHAR_SPAN       0x00001FFF

/* Cyclone V FPGA devices */
#define LW_BRIDGE_BASE       0xFF200000

#define LEDR_BASE            0x00000000
#define HEX3_HEX0_BASE       0x00000020
#define HEX5_HEX4_BASE       0x00000030
#define SW_BASE              0x00000040
#define KEY_BASE             0x00000050
#define JP1_BASE             0x00000060
#define JP2_BASE             0x00000070
#define PS2_BASE             0x00000100
#define PS2_DUAL_BASE        0x00000108
#define JTAG_UART_BASE       0x00001000
#define JTAG_UART_2_BASE     0x00001008
#define IrDA_BASE            0x00001020
#define TIMER0_BASE          0x00002000
#define TIMER1_BASE          0x00002020
#define AV_CONFIG_BASE       0x00003000
#define PIXEL_BUF_CTRL_BASE  0x00003020
#define CHAR_BUF_CTRL_BASE   0x00003030
#define AUDIO_BASE           0x00003040
// Audio Core Registers
#define FIFOSPACE            1   // word offset
#define LDATA                2   // word offset
#define RDATA                3   // word offset
#define VIDEO_IN_BASE        0x00003060
#define ADC_BASE             0x00004000

#define LW_BRIDGE_SPAN       0x00005000

/* ARM Peripherals */
#define I2C0_BASE            0xFFC04000   // base
#define I2C0_CON             0x00000000   // word offset
#define I2C0_TAR             0x00000001   // word offset
#define I2C0_DATA_CMD        0x00000004   // word offset
#define I2C0_FS_SCL_HCNT     0x00000007   // word offset
#define I2C0_FS_SCL_LCNT     0x00000008   // word offset
#define I2C0_ENABLE          0x0000001B   // word offset
#define I2C0_RXFLR           0x0000001E   // word offset
#define I2C0_ENABLE_STATUS   0x00000027   // word offset
#define I2C0_SPAN            0x00000100   // span

/* Cyclone V HPS devices */
#define HPS_BRIDGE_BASE      0xFF700000   // base
#define HPS_GPIO0_BASE       0x00008000   // word offset
#define HPS_GPIO1_BASE       0x00009000
#define HPS_GPIO2_BASE       0x0000A000
#define I2C1_BASE            0x00505000
#define I2C2_BASE            0x00506000
#define I2C3_BASE            0x00507000
#define HPS_TIMER0_BASE      0x00508000
#define HPS_TIMER1_BASE      0x00509000
#define HPS_TIMER2_BASE      0x00600000
#define HPS_TIMER3_BASE      0x00601000
#define HPS_RSTMGR           0x00605000
#define HPS_RSTMGR_PREMODRST 0x00605014
#define FPGA_BRIDGE          0x0060501C

#define HPS_BRIDGE_SPAN      0x006FFFFF   // span

#define PIN_MUX              0xFFD08400   // word offset
#define CLK_MGR              0xFFD04000

#define SPIM0_BASE           0xFFF00000   // base
#define SPIM0_SR             0x00000028   // word offset
#define SPIM0_DR             0x00000060
#define SPIM0_SPAN           0x00000100   // span

/* ARM A9 MPCORE devices */
#define PERIPH_BASE          0xFFFEC000    // base address of peripheral devices
#define MPCORE_PRIV_TIMER    0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF     0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR               0x00          // offset to CPU interface control reg
#define ICCPMR               0x04          // offset to interrupt priority mask reg
#define ICCIAR               0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR              0x10          // offset to end of interrupt reg

/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST      0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR               0x00          // offset to distributor control reg
#define ICDISER              0x100         // offset to interrupt set-enable regs
#define ICDICER              0x180         // offset to interrupt clear-enable regs
#define ICDIPTR              0x800         // offset to interrupt processor targets regs
#define ICDICFR              0xC00         // offset to interrupt configuration regs

#define SYSMGR_BASE          0xFFD08000    // base
#define SYSMGR_GENERALIO7    0