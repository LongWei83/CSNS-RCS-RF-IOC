
#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysLib.h>
#include <intLib.h>
#include <taskLib.h>
#include <string.h>
#include <logLib.h>
#include <drv/pci/pciConfigLib.h>
#include <arch/ppc/ivPpc.h>
#include "drvD212.h"
#include "plx9656.h"
#include "llrfCommonIO.h"
#include "parameter.h"
#include "drvSup.h"
#include "epicsExport.h"
#include <semLib.h>
#include <dbScan.h>


/* D212 Register Map */
#define    REG_Identifier			0x000

#define    REG_Int_Enable			0x004

#define    REG_RF_Reset				0x00c

#define    REG_Int_Clear			0x008

#define    REG_Alarm				0x010

#define    REG_Drv_Reset			0x014

#define    REG_SG_Mode				0x018

#define    REG_ARC_COUNT			0x01C

#define    REG_Point_Sweep			0x100
#define    REG_Sweep_Option			0x104
#define    REG_AMP_Option			0x108
#define    REG_AMP_FF_Option			0x10C
#define    REG_AMP_Modify_Option		0x110
#define    REG_Tune_Option			0x114
#define    REG_Front_Tune_Option		0x118
#define    REG_Tune_FF_Option			0x11C
#define    REG_Tune_Modify_Option		0x120
#define	   REG_Phase_Option			0x124
#define	   REG_Phase_FF_Option			0x128
#define	   REG_Phase_Modify_Option		0x12C

#define    REG_Fix_Frequency_Set		0x200
#define    REG_Work_Period_Set			0x204
#define    REG_AMP_Set				0x208
#define    REG_AMP_Coefficient			0x20C
#define    REG_AMP_P_Set			0x210
#define    REG_AMP_I_Set			0x214
#define    REG_AMP_I_Set1			0x218
#define    REG_AMP_I_Set2			0x21C
#define    REG_AMP_I_Set3			0x220
#define    REG_Bias_Set				0x224
#define    REG_Fix_Tuning_Angle			0x228
#define    REG_Tuning_Angle_Offset		0x22C
#define    REG_Tune_P_Set			0x230
#define    REG_Tune_I_Set			0x234
#define    REG_Tune_I_Set1			0x238
#define    REG_Tune_I_Set2			0x23C
#define    REG_Tune_I_Set3			0x240
#define    REG_Front_Bias_Set			0x244
#define    REG_Front_Tune_P_Set			0x248
#define    REG_Front_Tune_I_Set			0x24C
#define    REG_Front_Fix_Tuning_Angle		0x250
#define    REG_Phase_P				0x254
#define    REG_Phase_I				0x258
#define    REG_Initial_Phase			0x25C
#define    REG_FF_Delay				0x260

#define    REG_AMP_Upload			0x300
#define    REG_AMP_Set_Upload			0x304
#define    REG_Tuning_Phase_Upload		0x308
#define    REG_Front_Tuning_Phase_Upload	0x30C
#define    REG_Bias_Upload			0x310
#define    REG_Front_Bias_Upload		0x314
#define    REG_Reserved_1			0x318
#define    REG_Reserved_2			0x31C    	

static long D212Report(int level);

static D212Card *firstCard = NULL;
static int dmaCount = 0;
static int intHasConnect[MAX_INT_SUP] = {0, 0, 0, 0};

struct {
        long    number;
        DRVSUPFUN       report;
        DRVSUPFUN       init;
}drvD212 = {
    2,
    D212Report,
    NULL
};
epicsExportAddress (drvet, drvD212);

long D212Report (int level)
{
   D212Card *pCard;
   for (pCard = firstCard; pCard; pCard = pCard->next) 
   {
      /* print a short report */
      printf("Card %d with BDF (%d,%d,%d)\n", pCard->cardNum, pCard->bus, pCard->device, pCard->function);

      /* print additional card information */
      if(level >= 1)
      {
         printf("Bridge PCI Address: 0x%08x\n",  pCard->bridgeAddr);
         printf("FPGA PCI Address: 0x%08x\n", pCard->fpgaAddr);
         printf("Interrupt Line: %d\n", pCard->intLine);
      }

      if(level >= 2)
      {
         /* print more card information */
         printf("Index: %d\n", pCard->index);
         printf("FPGA Version: 0x%08x\n", pCard->fpgaVersion);
         printf("Buffer Address: 0x%08x\n", (unsigned int) pCard->buffer);
         printf("Float Buffer Address: 0x%08x\n", (unsigned int) pCard->floatBuffer);
      }
   }

   return 0;
}

/* The configure function is called from the startup script */
int D212Config (int cardNum, int index)
{
   int bus;
   int device;
   int function;
   unsigned char intLine;
   unsigned int busAddr;
   int i;

   D212Card *pCard;
   D212Card *pCardIndex;

   /* Check card number for sanity */
   if (cardNum < 0)
   {
       fprintf (stderr, "D212Configure: cardNum %d must be >= 0\n",
                cardNum);
       return ERROR;
   } 

   /* Check index for sanity */
   if (index < 0) 
   {
       fprintf (stderr, "D212Configure: index %d must be >= 0\n",index);
       return ERROR;
   }

   /* Find end of card list and check for duplicates */
   for (pCardIndex = firstCard; pCardIndex; pCardIndex = pCardIndex->next)
   {
       if (pCardIndex->cardNum == cardNum) 
       {
           fprintf (stderr, "D212Configure: cardNum %d already in use\n", 
                   cardNum);
           return ERROR;
       }
       if (pCardIndex->index == index)
       {
           fprintf (stderr, "D212Configure: index %d already in use\n",
                   index);
           return ERROR;
       }
   }

   /* find D212 card, the actual PCI target is PLX9656 bridge chip */
   if(pciFindDevice(PLX9656_VENDOR_ID, PLX9656_DEVICE_ID, index,   
                 &bus, &device, &function) == ERROR)
   {
       fprintf (stderr, "D212Configure: fail to find D212 index %d\n",
                index);
       return ERROR;
   }
 
   /* Create new card structure */
   pCard = (D212Card*) malloc (sizeof (D212Card));
   if (!pCard) 
   {
       fprintf (stderr, "D212Config: fail to alloc pCard\n");
       return ERROR;
   }

   /* add card struct to link list */
   if (pCardIndex == NULL && firstCard == NULL)
       firstCard = pCard;
   else
   {
       for(pCardIndex=firstCard; pCardIndex->next!=NULL;
                 pCardIndex=pCardIndex->next);
       pCardIndex->next = pCard;
   }

   pCard->next = NULL;
   pCard->cardNum = cardNum;

   /*BAR0 corresponds to 9656 register*/
   pciConfigInLong (bus, device, function,
                    PCI_CFG_BASE_ADDRESS_0, &busAddr);
   busAddr &= PCI_MEMBASE_MASK;
   pCard->bridgeAddr = busAddr;
 
   /*BAR2 corresponds to FPGA register*/
   pciConfigInLong (bus, device, function,
                    PCI_CFG_BASE_ADDRESS_1, &busAddr);
   busAddr &= PCI_MEMBASE_MASK;
   pCard->fpgaAddr = busAddr;

   /* store BDF and index to card structure */
   pCard->bus = bus;
   pCard->device = device;
   pCard->function = function;
   pCard->index = index;

   /* get interrupt vector */
   pciConfigInByte (bus, device, function,
                    PCI_CFG_DEV_INT_LINE, &intLine);
   pCard->intLine = intLine;

   /* create DMA0 semphore */
   pCard->semDMA0 = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
   if( pCard->semDMA0 == NULL)
   {
       fprintf(stderr,"create semDMA0 error\n");
       return ERROR;
   }

   /* allocate data buffer */
   pCard->buffer = (int *) calloc (DMA_TRANSFER_NUM, sizeof(int));
   if (!pCard->buffer)
   {
       fprintf (stderr, "D212Config: fail to alloc buffer\n");
       return ERROR;
   }

   /* allocate processed float data buffer */
   pCard->floatBuffer = (float *) calloc (DMA_TRANSFER_NUM+0x800+1, sizeof(float));
   if (!pCard->floatBuffer)
   {
       fprintf (stderr, "D212Config: fail to alloc float buffer\n");
       return ERROR;
   }
   
   pCard->errorFlag = 0;

   scanIoInit(&pCard->ioScanPvt); 

   pCard->fpgaVersion = FPGA_REG_READ32(pCard->fpgaAddr, REG_Identifier); 

   /* ensure that each interrupt line of four be connected only once */

   if(! intHasConnect[pCard->intLine - PCIE0_INT0_VEC])
   {
      /* connect ISR to interrupt, use intLine as interrupt vector */
      if(intConnect(INUM_TO_IVEC(pCard->intLine), cpciIntISR, pCard->intLine) == ERROR)
      {
         printf("intConnect error: Card %d\tintLine %d\n", pCard->cardNum, pCard->intLine);
         return ERROR;
      }

      /*enable interrupt*/
      if(intEnable(pCard->intLine) == ERROR)
      {
         printf("intEnable error: Card %d\tintLine %d\n", pCard->cardNum, pCard->intLine);
         return ERROR;
      }

      intHasConnect[pCard->intLine - PCIE0_INT0_VEC] = 1;

      printf("Card %d, intLine %d: now intConnect\n\n", pCard->cardNum, pCard->intLine);
   }
   else
   {
      printf("Card %d, intLine %d: intLine has been connected already\n\n", pCard->cardNum, pCard->intLine);
   }

   /* initialize plx9656 bridge chip */
   plx9656Init(pCard);

   /* start data process task */
   if( ERROR == taskSpawn("dataProcessTask", 100, VX_FP_TASK, 10000, (FUNCPTR) dataProcess, (int) pCard, 0, 0, 0, 0, 0, 0, 0, 0, 0))
   {
      printf("Fail to spawn data process task!\n");
   }

   /* print card configuration information */
   printf("Card %d successfully initialized:\n", pCard->cardNum);
   printf("BDF: %d %d %d\n", pCard->bus, pCard->device, pCard->function);
   printf("Index: %d\n", pCard->index);
   printf("Bridge PCI Address: 0x%08x\n", pCard->bridgeAddr);
   printf("FPGA PCI Address: 0x%08x\n", pCard->fpgaAddr);
   printf("Interrupt Line: %d\n", pCard->intLine);
   printf("FPGA Version: 0x%08x\n", pCard->fpgaVersion);
   printf("Buffer Address: 0x%08x\n", (unsigned int) pCard->buffer);
   printf("Float Buffer Address: 0x%08x\n", (unsigned int) pCard->floatBuffer);
   printf("Start IOC!!!\n");

   return 0;
}

/*---------------------Comment for hardware register access--------------------
 * a) CPCI_WRITE8(pCard->bridgeAddr, REG_9656_DMA0_CSR, 0x09); 
 *
 *    Clear DMA0 Interrupt
 *    Following is the equivalent:
 *    regRead8 = CPCI_READ8(pCard->bridgeAddr, REG_9656_DMA0_CSR);
 *    regRead8 |= PLX9656_DMA0_INTERRUPT_CLEAR;
 *    CPCI_WRITE8(pCard->bridgeAddr, REG_9656_DMA0_CSR, regRead8);
 *
 * b) CPCI_WRITE32(pCard->bridgeAddr, REG_9656_INTCSR, 0x0f0C0900);
 *
 *    Enable LINTi#
 *    Following is the equivalent:
 *    regRead32 = CPCI_READ32(pCard->bridgeAddr, REG_9656_INTCSR);
 *    regRead32 |= PLX9656_INTCSR_LINTi_ENABLE;
 *    CPCI_WRITE32(pCard->bridgeAddr, REG_9656_INTCSR, regRead32);       
 *
 * c) CPCI_WRITE32(pCard->bridgeAddr, REG_9656_INTCSR, 0x0f0C0100);
 *
 *    Disable LINTi#, i.e. Clear Local Interrupt
 *    Following is the equivalent:
 *    regRead32 = CPCI_READ32(pCard->bridgeAddr, REG_9656_INTCSR);
 *    regRead32 &= ~PLX9656_INTCSR_LINTi_ENABLE;
 *    CPCI_WRITE32(pCard->bridgeAddr, REG_9656_INTCSR, regRead32);
 *
 * d) CPCI_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_DPR, 0x00000008);
 *
 *    Select transfers from the Local Bus to the PCI Bus
 *    Following is the equivalent:
 *    regRead32 = CPCI_READ32(pCard->bridgeAddr, REG_9656_DMA0_DPR);
 *    regRead32 |= PLX9656_DMA0_DIRECT_LOC_TO_PCI;
 *    CPCI_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_DPR, regRead32);
 *
 * e) CPCI_WRITE8(pCard->bridgeAddr, REG_9656_DMA0_CSR, 0x03);
 *
 *    DMA Channel 0 Start
 *    Following is the equivalent:
 *    regRead8 = CPCI_READ8(pCard->bridgeAddr, REG_9656_DMA0_CSR);
 *    regRead8 |= PLX9656_DMA0_START;
 *    CPCI_WRITE8(pCard->bridgeAddr, REG_9656_DMA0_CSR, regRead8);
 *
 * f) CPCI_WRITE32(pCard->fpgaAddr, REG_CONTROL, 0x00000002);
 *
 *    Disable FIFO and Interrupt
 *    Following is the equivalent:
 *    regRead32 = CPCI_READ32(pCard->fpgaAddr, REG_CONTROL);
 *    regRead32 &= ~ D212_CR_FIFO_ENABLE;
 *    regRead32 &= ~ D212_CR_INT_ENABLE;
 *    CPCI_WRITE32(pCard->fpgaAddr, REG_CONTROL, regRead32);
 *
 *----------------end of Comment for hardware register access------------------
 */

/* interrupt service routine, one for 8 cards, with 4 different parameters */
void cpciIntISR(int intLine)
{
   D212Card *pCard;

   /* check which card generate interrupt */
   for (pCard = firstCard; pCard; pCard = pCard->next)
   {
      if(intLine == pCard->intLine)
      {
         /* FPGA interrupt */
         if(BRIDGE_REG_READ32(pCard->bridgeAddr, REG_9656_INTCSR) & PLX9656_INTCSR_LINTi_ACTIVE) 
         {
            /* clear FPGA interrupt, i.e. de-assert LINTi line */
            int_Clear(pCard);

            /* start DMA Channel 0 transfer, 8k bytes data at once */
            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_PCI_ADR, (unsigned int) (pCard->buffer));
            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_LOCAL_ADR, REG_AMP_Upload);
            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_SIZE, WAVEFORM_SIZE + 4);
            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_DPR, 0x00000008);
            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_CSR, 0x03);
            dmaCount++;
         }     
         /* DMA Channel 0 interrupt, Transfer Data from hardware to CPU board */
         else if(BRIDGE_REG_READ32(pCard->bridgeAddr, REG_9656_INTCSR) & PLX9656_INTCSR_DMA0_INT_ACTIVE)
         {
            /* clear DMA Channel 0 interrupt */
            BRIDGE_REG_WRITE8(pCard->bridgeAddr, REG_9656_DMA0_CSR, 0x09);
            /* read back, avoid spurious interrupt */
   /*         tmp8 = BRIDGE_REG_READ8(pCard->bridgeAddr, REG_9656_DMA0_CSR);*/

            /* re-enable FPGA interrupt */
     /*       BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_INTCSR, 0x0f0C0900); */

            if(dmaCount < 8)
            {
               /* start DMA Channel 0 transfer, 8k bytes data at once */
               BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_PCI_ADR, (unsigned int) (pCard->buffer + 2049 * dmaCount));
               BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_LOCAL_ADR, REG_AMP_Upload + 4 * dmaCount);
               BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_SIZE, WAVEFORM_SIZE + 4);
               BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_DPR, 0x00000008);
               BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA0_CSR, 0x03);
               dmaCount++; 
            }
            else
            {
               dmaCount = 0;
               /* synchronize data process task */
               semGive(pCard->semDMA0);
            }
         }
      }
   }
}

/*---------------------Comment for hardware register access--------------------
 * a) CPCI_WRITE32(pCard->fpgaAddr, REG_CONTROL, 0x00000007);
 *
 *    Enable FIFO, Period Generation, and Interrupt
 *    Following is the equivalent:
 *    regRead32 = CPCI_READ32(fpgaAddr, REG_CONTROL);
 *    regRead32 |= ( D212_CR_FIFO_ENABLE |
 *                   D212_CR_PERIOD_GEN |
 *                   D212_CR_INT_ENABLE );
 *    CPCI_WRITE32(fpgaAddr, REG_CONTROL, regRead32);
 *
 *----------------end of Comment for hardware register access------------------
 */

/* data process task, one for each of 8 cards */
void dataProcess(D212Card *pCard)
{
   int i;
   float *pDest;
   int *pSrc;

   /* infinite loop, used for data process */
   while(1)
   {
      /* synchronize with ISR */
      semTake(pCard->semDMA0, WAIT_FOREVER);

      /* process waveform 1 data */
      pDest = pCard->floatBuffer + WF1_FADDR;
      pSrc = pCard->buffer + WF1_ADDR;
      for(i=1; i<WAVEFOMR_NUM+1; i++)
      {
           pDest[i] =  (pSrc[i]>>12) * CALC_WF1_MUL + CALC_WF1_ADD;
      }

      /* process waveform 3 data */      
      pDest = pCard->floatBuffer + WF3_FADDR;
      pSrc = pCard->buffer + WF3_ADDR;
      for(i=1; i<WAVEFOMR_NUM+1; i++)
      {
           pDest[i] =  pSrc[i] * CALC_WF3_MUL + CALC_WF3_ADD;
      }

      /* Inform records to retrieve the data */
      scanIoRequest(pCard->ioScanPvt);
   }
}

/*---------------------Comment for hardware register access--------------------
 * a) CPCI_WRITE32(bridgeAddr, REG_9656_INTCSR, 0x0f0C0900);
 *
 *    Enable PCI Interrupt, LINTi# and DMA0 Interrupt
 *    Disable LINTo#
 *    Following is the equivalent:
 *
 *    regRead32 = CPCI_READ32(bridgeAddr, REG_9656_INTCSR);
 *    regRead32 &= ~PLX9656_INTCSR_LINTo_ENABLE;
 *    regRead32 |= ( PLX9656_INTCSR_PCI_INT_ENABLE |
 *                   PLX9656_INTCSR_LINTi_ENABLE   |
 *                   PLX9656_INTCSR_DMA0_INT_ENABLE|
 *                   PLX9656_INTCSR_DMA1_INT_ENABLE );
 *    CPCI_WRITE32(bridgeAddr, REG_9656_INTCSR, regRead32);
 *
 * b) CPCI_WRITE8(bridgeAddr, REG_9656_DMA0_CSR, 0x01);
 *
 *    Enable DMA Channel 0
 *    Following is the equivalent:
 *    regRead8 = CPCI_READ8(bridgeAddr, REG_9656_DMA0_CSR);
 *    regRead8 |= PLX9656_DMA0_ENABLE;
 *    CPCI_WRITE8(bridgeAddr, REG_9656_DMA0_CSR, regRead8);
 *
 * c) CPCI_WRITE32(bridgeAddr, REG_9656_DMA0_MODE, 0x00020d43);
 *
 *    Enable Local Burst, DMA0 Done Interrupt
 *    Select 32 Data Witth, Constant Local Address, DMA0 Interrupt to INTA#
 *    Following is the equivalent:
 *    regRead32 = CPCI_READ32(bridgeAddr, REG_9656_DMA0_MODE);
 *    regRead32 |= ( PLX9656_DMA0_DATA_WIDTH_32 |
 *                   PLX9656_DMA0_LOC_BURST_ENABLE |
 *                   PLX9656_DMA0_DONE_INT_ENABLE |
 *                   PLX9656_DMA0_LOCAL_ADDR_CONST |
 *                   PLX9656_DMA0_INT_SELECT_INTA );
 *    CPCI_WRITE32(bridgeAddr, REG_9656_DMA0_MODE, regRead32);
 *
 * d) The initialization of DMA Channel 1 is equivalent to Channel 0
 *
 *----------------end of Comment for hardware register access------------------
 */

/* initialize PLX9656 bridge chip */
void plx9656Init(D212Card *pCard)
{
   int bridgeAddr = pCard->bridgeAddr;

   BRIDGE_REG_WRITE32(bridgeAddr, REG_9656_INTCSR, 0x0f0C0900); 
/*   BRIDGE_REG_WRITE32(bridgeAddr, REG_9656_INTCSR, 0x0f040900); */

   BRIDGE_REG_WRITE8(bridgeAddr, REG_9656_DMA0_CSR, 0x01); 
/*   BRIDGE_REG_WRITE32(bridgeAddr, REG_9656_DMA0_MODE, 0x00020d43); */ 
   BRIDGE_REG_WRITE32(bridgeAddr, REG_9656_DMA0_MODE, 0x00020dC3); 
        

   BRIDGE_REG_WRITE8(bridgeAddr, REG_9656_DMA1_CSR, 0x01);
   BRIDGE_REG_WRITE32(bridgeAddr, REG_9656_DMA1_MODE, 0x00020d43);
}



/* get card structure by card number */
D212Card* getCardStruct (int cardNum)
{
   D212Card* pCard;
   for (pCard = firstCard; pCard; pCard = pCard->next)
      if (pCard->cardNum == cardNum)
         return pCard;
   return NULL;
}



void int_Enable (D212Card* pCard)
{
   FPGA_REG_WRITE32(pCard->fpgaAddr, REG_Int_Enable, OPTION_SET);
}

void int_Disable (D212Card* pCard)
{
   FPGA_REG_WRITE32(pCard->fpgaAddr, REG_Int_Enable, OPTION_CLEAR);
}

int int_Enable_get (D212Card* pCard)
{
   if (FPGA_REG_READ32(pCard->fpgaAddr, REG_Int_Enable) == 0xAAAAAAAA)
      return 1;
   else 
      return 0;
}

void int_Clear (D212Card* pCard)
{
   FPGA_REG_WRITE32(pCard->fpgaAddr, REG_Int_Clear, OPTION_SET);
}

void set_Sweep_Option (D212Card* pCard)
{
   FPGA_REG_WRITE32(pCard->fpgaAddr, REG_Sweep_Option, OPTION_SET);
}

void clear_Sweep_Option (D212Card* pCard)
{
   FPGA_REG_WRITE32(pCard->fpgaAddr, REG_Sweep_Option, OPTION_CLEAR);
}

int SweepOption_get (D212Card* pCard)
{
   if (FPGA_REG_READ32(pCard->fpgaAddr, REG_Sweep_Option) == 0xAAAAAAAA)
      return 1;
   else 
      return 0;
}

void set_Work_Period (D212Card* pCard, float period)
{
   unsigned int value;
   value = (unsigned int)(period * CALC_Work_Period_Set_MUL + CALC_Work_Period_Set_ADD);
   FPGA_REG_WRITE32(pCard->fpgaAddr, REG_Work_Period_Set, value);
}

float get_Work_Period (D212Card* pCard)
{
   return (FPGA_REG_READ32(pCard->fpgaAddr, REG_Work_Period_Set) - CALC_Work_Period_Set_ADD) / CALC_Work_Period_Set_MUL;
}
