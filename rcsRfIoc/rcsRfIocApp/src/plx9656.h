#ifndef PLX9656_H
#define PLX9656_H

/*9656 register address*/
#define REG_9656_INTCSR 		0x68
#define REG_9656_DMA0_MODE   		0x80
#define REG_9656_DMA0_PCI_ADR 		0x84
#define REG_9656_DMA0_LOCAL_ADR    	0x88
#define REG_9656_DMA0_SIZE    		0x8C
#define REG_9656_DMA0_DPR    		0x90
#define REG_9656_DMA0_CSR    		0xA8
#define REG_9656_DMA1_MODE              0x94
#define REG_9656_DMA1_PCI_ADR           0x98
#define REG_9656_DMA1_LOCAL_ADR         0x9C
#define REG_9656_DMA1_SIZE              0xA0
#define REG_9656_DMA1_DPR               0xA4
#define REG_9656_DMA1_CSR               0xA9

/*---------------------
 * 9656 Interrupt Control/Status Register (0x68) Bit Assignments
 */
#define PLX9656_INTCSR_PCI_INT_ENABLE	0x00000100
#define PLX9656_INTCSR_LINTi_ENABLE	0x00000800
#define PLX9656_INTCSR_LINTi_ACTIVE	0x00008000
#define PLX9656_INTCSR_LINTo_ENABLE	0x00010000
#define PLX9656_INTCSR_DMA0_INT_ENABLE	0x00040000
#define PLX9656_INTCSR_DMA0_INT_ACTIVE	0x00200000
#define PLX9656_INTCSR_DMA1_INT_ENABLE  0x00080000
#define PLX9656_INTCSR_DMA1_INT_ACTIVE  0x00400000

/*---------------------
 * 9656 DMA Channel 0 Mode Register (0x80) Bit Assignments
 */
#define PLX9656_DMA0_DATA_WIDTH_32	0x00000003
#define PLX9656_DMA0_LOC_BURST_ENABLE	0x00000100
#define PLX9656_DMA0_DONE_INT_ENABLE	0x00000400
#define PLX9656_DMA0_LOCAL_ADDR_CONST	0x00000800
#define PLX9656_DMA0_INT_SELECT_INTA	0x00020000

/*---------------------
 * 9656 DMA Channel 0 Descriptor Pointer Register (0x90) Bit Assignments
 */
#define PLX9656_DMA0_DIRECT_LOC_TO_PCI	0x00000008

/*---------------------
 * 9656 DMA Channel 0 Command/Status Register (0xA8) Bit Assignments
 */
#define PLX9656_DMA0_ENABLE		0x01
#define PLX9656_DMA0_START		0x02
#define PLX9656_DMA0_INTERRUPT_CLEAR	0x08

/*---------------------
 * 9656 DMA Channel 1 Mode Register (0x94) Bit Assignments
 */
#define PLX9656_DMA1_DATA_WIDTH_32      0x00000003
#define PLX9656_DMA1_LOC_BURST_ENABLE   0x00000100
#define PLX9656_DMA1_DONE_INT_ENABLE    0x00000400
#define PLX9656_DMA1_LOCAL_ADDR_CONST   0x00000800
#define PLX9656_DMA1_INT_SELECT_INTA    0x00020000

/*---------------------
 * 9656 DMA Channel 1 Descriptor Pointer Register (0xA4) Bit Assignments
 */
#define PLX9656_DMA1_DIRECT_LOC_TO_PCI  0x00000008

/*---------------------
 * 9656 DMA Channel 1 Command/Status Register (0xA9) Bit Assignments
 */
#define PLX9656_DMA1_ENABLE             0x01
#define PLX9656_DMA1_START              0x02
#define PLX9656_DMA1_INTERRUPT_CLEAR    0x08

/* defines */
/*
#define PLX9656_VENDOR_ID      	        0x10B5
#define PLX9656_DEVICE_ID               0x9110
*/
#define PLX9656_VENDOR_ID      	        0x0709
#define PLX9656_DEVICE_ID               0xd212

#endif /*End of PLX9656_H*/
