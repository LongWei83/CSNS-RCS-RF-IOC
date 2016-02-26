#ifndef DRV_D212_h
#define DRV_D212_h

#include <semLib.h>
#include <dbScan.h>

#define TIME_DIFF(x,y)   ((x) >= (y)) ? \
                          ((x) - (y)) : ((x) + (0xffffffff - (y)) + 1)

#define BYTE_ORDER_SWAP(x)    \
   (((((UINT) (x))<<24)&0xFF000000)+((((UINT) (x))<<8)&0x00FF0000)    \
   +((((UINT) (x))>>8)&0x0000FF00)+((((UINT) (x))>>24)&0x000000FF))

#define DMA_TRANSFER_NUM         0x00004000+8   /* 16k+8 */
#define DMA_TRANSFER_SIZE        0x00010000+4*8   /* 64k+4*8 */
#define DMA_WRITE_NUM            0x00000800   /* 2k */
/******* no int ***********
#define INTERRUPT_NUM            100
**************************/
#define WAVEFOMR_NUM             0x00000800   /* 2k */
#define WAVEFORM_SIZE            0x00002004   /* 8k */

/******* no int ***********
#define MODE_TEST                0
#define MODE_NORMAL              1
******************************/

#define OPTION_SET               0xAAAAAAAA
#define OPTION_CLEAR             0x55555555

#define MAX_INT_SUP              4
#define MAX_BOARD_SUP            8

/* Has been defined in mcpx800.h
#define CPCIA_INT_LVL            28
#define CPCIB_INT_LVL            29
#define CPCIC_INT_LVL            30
#define CPCID_INT_LVL            31
*/
/* Has been defined in cpci6200.h */
#define PCIE0_INT0_VEC     0
#define PCIE0_INT1_VEC     1
#define PCIE0_INT2_VEC     2
#define PCIE0_INT3_VEC     3

/* waveform offset in the 16k buffer */
#define WF1_ADDR                 0
#define WF2_ADDR                 2049
#define WF3_ADDR                 4098
#define WF4_ADDR                 6147
#define WF5_ADDR                 8196
#define WF6_ADDR                 10245
#define WF7_ADDR                 12294
#define WF8_ADDR                 14343

#define WF1_FADDR                 0
#define WF2_FADDR                 2049
#define WF3_FADDR                 4098
#define WF4_FADDR                 6147
#define WF5_FADDR                 8196
#define WF6_FADDR_A               10245
#define WF6_FADDR_B               12294
#define WF7_FADDR                 14343
#define WF8_FADDR                 16392


/*
#define WF1_ADDR                 0
#define WF2_ADDR                 2048
#define WF3_ADDR                 4096
#define WF4_ADDR	         6144
#define WF5_ADDR                 8192
#define WF6_ADDR                 10240
#define WF7_ADDR                 12288
#define WF8_ADDR                 14336
*/



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
   SEM_ID semDMA1;              /*DMA1 interrupt*/
   int *buffer;       /*store data transferred via DMA*/
   float *floatBuffer;     /*store processed float data */
   int *writeBuffer;    /* store int data that need to write to FPGA */
   float *ampSkewBuffer;
   float *errorAllBuffer;
   float *errorPhaseBuffer;
   float *errorFrontBuffer;
   float *errorTotalBuffer;
   float *gridBuffer;
   float *frontBuffer;
   int errorFlag;
   /******* no int ***********
   int interruptNum;
   ************************/
   IOSCANPVT ioScanPvt;
};

struct recPrivate {
   struct D212Card *pCard;
   int function;
};


UINT32 sysTimeBaseFreqGet (void);
void cpciIntISR(int intLine);
void dataProcess(D212Card *pCard);
void dmaWrite(D212Card *pCard);
void plx9656Init(D212Card *pCard);
UINT32 sysTimeBaseLGet (void);
UINT sysGetBusSpd (void);
D212Card* getCardStruct (int cardNum);
/******* no int ***********
void setInterruptNum (D212Card* pCard, int interruptNum);
int getInterruptNum (D212Card* pCard);
void setRunMode (int mode);
int getRunMode ();
****************************/

/******* no int ***********
void int_Enable (D212Card* pCard);
void int_Disable (D212Card* pCard);
int int_Enable_get (D212Card* pCard);
***************************/
void int_Clear (D212Card* pCard);
void set_RFReset_Option (D212Card* pCard);
void clear_RFReset_Option (D212Card* pCard);
int RFReset_get (D212Card* pCard);
void set_Sweep_Option (D212Card* pCard);
void clear_Sweep_Option (D212Card* pCard);
int SweepOption_get (D212Card* pCard);
int alarm0_get (D212Card* pCard);
int alarm1_get (D212Card* pCard);
int alarm2_get (D212Card* pCard);
int alarm3_get (D212Card* pCard);
int alarm4_get (D212Card* pCard);
int alarm5_get (D212Card* pCard);
int alarm6_get (D212Card* pCard);
int alarm7_get (D212Card* pCard);
void set_Drv_Option (D212Card* pCard);
void clear_Drv_Option (D212Card* pCard);
int Drv_Reset_get (D212Card* pCard);
void set_SG_Mode (D212Card* pCard);
void clear_SG_Mode (D212Card* pCard);
int SG_Mode_get (D212Card* pCard);
void set_AMP_Option (D212Card* pCard);
void clear_AMP_Option (D212Card* pCard);
int AMP_OPTION_get (D212Card* pCard);
void set_AMP_FF_Option (D212Card* pCard);
void clear_AMP_FF_Option (D212Card* pCard);
int AMP_FF_OPTION_get (D212Card* pCard);
void set_AMP_Modify_Option (D212Card* pCard);
void clear_AMP_Modify_Option (D212Card* pCard);
int AMP_Modify_OPTION_get (D212Card* pCard);
void set_Tune_Option (D212Card* pCard);
void clear_Tune_Option (D212Card* pCard);
int Tune_OPTION_get (D212Card* pCard);
void set_Front_Tune_Option (D212Card* pCard);
void clear_Front_Tune_Option (D212Card* pCard);
int Front_Tune_OPTION_get (D212Card* pCard);
void set_Tune_FF_Option (D212Card* pCard);
void clear_Tune_FF_Option (D212Card* pCard);
int Tune_FF_OPTION_get (D212Card* pCard);
void set_Tune_Modify_Option (D212Card* pCard);
void clear_Tune_Modify_Option (D212Card* pCard);
int Tune_Modify_OPTION_get (D212Card* pCard);
void set_Phase_FF_Option (D212Card* pCard);
void clear_Phase_FF_Option (D212Card* pCard);
int Phase_FF_Option_get (D212Card* pCard);
void set_Phase_Modify_Option (D212Card* pCard);
void clear_Phase_Modify_Option (D212Card* pCard);
int Phase_Modify_Option_get (D212Card* pCard);

void set_Fix_Frequency (D212Card* pCard, float frequency);
float get_Fix_Frequency (D212Card* pCard);
void set_Work_Period (D212Card* pCard, float period);
float get_Work_Period (D212Card* pCard);
void set_AMP (D212Card* pCard, float ampSet);
float get_AMP (D212Card* pCard);
void set_AMP_Coefficient (D212Card* pCard, float ampCoefficient);
float get_AMP_Coefficient (D212Card* pCard);
void set_AMP_P (D212Card* pCard, float ampP);
float get_AMP_P (D212Card* pCard);
void set_AMP_I (D212Card* pCard, float ampI);
float get_AMP_I (D212Card* pCard);
void set_AMP_I_1 (D212Card* pCard, float ampI_1);
float get_AMP_I_1 (D212Card* pCard);
void set_AMP_I_2 (D212Card* pCard, float ampI_2);
float get_AMP_I_2 (D212Card* pCard);
void set_AMP_I_3 (D212Card* pCard, float ampI_3);
float get_AMP_I_3 (D212Card* pCard);
void set_Bias (D212Card* pCard, float bias);
float get_Bias (D212Card* pCard);
void set_Fix_Tuning_Angle (D212Card* pCard, float angle);
float get_Fix_Tuning_Angle (D212Card* pCard);
void set_Tuning_Angle_Offset (D212Card* pCard, float offset);
float get_Tuning_Angle_Offset (D212Card* pCard);
void set_Tune_P (D212Card* pCard, float tuneP);
float get_Tune_P (D212Card* pCard);
void set_Tune_I (D212Card* pCard, float tuneI);
float get_Tune_I (D212Card* pCard);
void set_Tune_I_1 (D212Card* pCard, float tuneI_1);
float get_Tune_I_1 (D212Card* pCard);
void set_Tune_I_2 (D212Card* pCard, float tuneI_2);
float get_Tune_I_2 (D212Card* pCard);
void set_Tune_I_3 (D212Card* pCard, float tuneI_3);
float get_Tune_I_3 (D212Card* pCard);
void set_Front_Bias (D212Card* pCard, float frontBias);
float get_Front_Bias (D212Card* pCard);
void set_Front_Tune_P (D212Card* pCard, float frontTuneP);
float get_Front_Tune_P (D212Card* pCard);
void set_Front_Tune_I (D212Card* pCard, float frontTuneI);
float get_Front_Tune_I (D212Card* pCard);
void set_Front_Fix_Tuning_Angle (D212Card* pCard, float frontFixTunAng);
float get_Front_Fix_Tuning_Angle (D212Card* pCard);
void set_phase_i (D212Card* pCard, float phase_i);
float get_Phase_i (D212Card* pCard);
void set_phase_p (D212Card* pCard, float phase_p);
float get_Phase_p (D212Card* pCard);
void set_Initial_Phase (D212Card* pCard, float initial_phase);
float get_Initial_Phase (D212Card* pCard);
void set_FF_Delay (D212Card* pCard, float ff_delay);
float get_FF_Delay (D212Card* pCard);
float get_ARC_COUNT (D212Card* pCard);


#endif  /*end of DRV_D212_h*/