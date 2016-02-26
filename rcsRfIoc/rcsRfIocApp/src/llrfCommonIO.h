#ifndef LLRF_COMMONIO_H
#define LLRF_COMMONIO_H

/*---------------------
 * CPCI register Read Operations, without byte-order reverse
 */
#define FPGA_REG_READ8(base,offset)  \
        *((volatile unsigned char *)  ((unsigned char *)(base) + offset))
#define FPGA_REG_READ16(base,offset) \
        *((volatile unsigned short *) ((unsigned char *)(base) + offset))
#define FPGA_REG_READ32(base,offset) \
        *((volatile unsigned long *)  ((unsigned char *)(base) + offset))

/*---------------------
 * CPCI register Write Operations, without byte-order reverse
 */
#define FPGA_REG_WRITE8(base,offset,value) \
      *(volatile unsigned char *)  ((unsigned char *)(base) + offset) = value
#define FPGA_REG_WRITE16(base,offset,value) \
      *(volatile unsigned short *) ((unsigned char *)(base) + offset) = value
#define FPGA_REG_WRITE32(base,offset,value) \
      *(volatile unsigned long *)  ((unsigned char *)(base) + offset) = value

/*---------------------
 * CPCI register Read Operations, with byte-order reverse
 */
#define BRIDGE_REG_READ8(base,offset)     PCI_IN_BYTE(base + offset)
#define BRIDGE_REG_READ16(base,offset)    PCI_IN_WORD(base + offset)
#define BRIDGE_REG_READ32(base,offset)    PCI_IN_LONG(base + offset)

/*---------------------
 * CPCI register Write Operations, with byte-order reverse
 */
#define BRIDGE_REG_WRITE8(base,offset,value) \
        PCI_OUT_BYTE ((base + offset), value)
#define BRIDGE_REG_WRITE16(base,offset,value) \
        PCI_OUT_WORD ((base + offset), value)
#define BRIDGE_REG_WRITE32(base,offset,value) \
        PCI_OUT_LONG ((base + offset), value)

#endif /*LLRF_COMMONIO_H*/
