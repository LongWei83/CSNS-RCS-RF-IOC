#ifndef DRV_D212_h
#define DRV_D212_h

#include <semLib.h>
#include <dbScan.h>

#define TIME_DIFF(x,y)   ((x) >= (y)) ? \
                          ((x) - (y)) : ((x) + (0xffffffff - (y)) + 1)

#define BYTE_ORDER_SWAP(x)    \
   (((((UINT) (x))<<24)&0xFF000000)+((((UINT) (x))<<8)&0x00FF0000)    \
   +((((UINT) (x))>>8)&0x0000FF00)+((((UINT) (x))>>24)&0x000000FF))

#define MAX_INT_SUP              4

#define DMA_TRANSFER_NUM         0x00004000+8   /* 16k+8 */
#define DMA_TRANSFER_SIZE        0x00010000+4*8   /* 64k+4*8 */
#define DMA_WRITE_NUM            0x00000800   /* 2k */
/******* no int ***********
#define INTERRUPT_NUM            100
**************************/
#define WAVEFOMR_NUM             0x00000800   /* 2k */
#define WAVEFORM_SIZE            0x00002004   /* 8k */

#define WF1_ADDR                 0
#define WF3_ADDR                 4098
#define WF1_FADDR                0
#define WF3_FADDR                4098

#define OPTION_SET               0xAAAAAAAA
#define OPTION_CLEAR             0x55555555

/* Has been defined in cpci6200.h */
#define PCIE0_INT0_VEC     0
#define PCIE0_INT1_VEC     1
#define PCIE0_INT2_VEC     2
#define PCIE0_INT3_VEC     3


typedef struct D212Card D212Card;
typedef struct recPrivate recPrivate;

/* linked list of card structures */
struct D212Card {
   struct D212Card *next;
   int cardNum;
   unsigned int bridgeAddr;             /*9656 register*/
   unsigned int fpgaAddr;               /*FPGA register*/
   int bus;
   int device;
   int function;
   int index;
   unsigned int intLine;
   unsigned int fpgaVersion;
   SEM_ID semDMA0;              /*DMA0 interrupt*/
   int *buffer;       /*store data transferred via DMA*/
   float *floatBuffer;     /*store processed float data */
   float *errorPhaseBuffer;
   IOSCANPVT ioScanPvt;
   int errorFlag;
};

struct recPrivate {
   struct D212Card *pCard;
   int function;
};

void cpciIntISR(int intLine);
void dataProcess(D212Card *pCard);
void int_Enable (D212Card* pCard);
void int_Disable (D212Card* pCard);
int int_Enable_get (D212Card* pCard);
void set_Sweep_Option (D212Card* pCard);
void clear_Sweep_Option (D212Card* pCard);
int SweepOption_get (D212Card* pCard);
void set_Work_Period (D212Card* pCard, float period);
float get_Work_Period (D212Card* pCard);
D212Card* getCardStruct (int cardNum);

#endif  /*end of DRV_D212_h*/
