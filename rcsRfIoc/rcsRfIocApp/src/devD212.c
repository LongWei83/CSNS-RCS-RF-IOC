#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <devSup.h>
#include <recGbl.h>
#include <dbAccess.h>
#include <dbScan.h>
#include <epicsExport.h>
#include <biRecord.h>
#include <boRecord.h>
#include <aiRecord.h>
#include <aoRecord.h>
#include <longinRecord.h>
#include <longoutRecord.h>
#include <waveformRecord.h>
#include <drvD212.h>
#include "llrfCommonIO.h"
#include "plx9656.h"
#include <drv/pci/pciConfigLib.h>


/* record function definition */
/******* no int ***********
#define CPCI_BI_RUN_MODE                0
***************************/
#define CPCI_BI_INT_ENABLE              1
#define CPCI_BI_RF_RESET                2
#define CPCI_BI_SWEEP_OPTION            3
#define CPCI_BI_AMP_OPTION              4
#define CPCI_BI_AMP_FF_OPTION           5
#define CPCI_BI_AMP_MODIFY_OPTION       6
#define CPCI_BI_TUNE_OPTION             7
#define CPCI_BI_FRONT_TUNE_OPTION       8
#define CPCI_BI_TUNE_FF_OPTION          9
#define CPCI_BI_TUNE_MODIFY_OPTION      10
#define CPCI_BI_PHASE_OPTION      	11
#define CPCI_BI_POINT_SWEEP      	12
#define CPCI_BI_ALARM0		      	13
#define CPCI_BI_ALARM1		      	14
#define CPCI_BI_ALARM2		      	15
#define CPCI_BI_ALARM3		      	16
#define CPCI_BI_ALARM4		      	17
#define CPCI_BI_ALARM5		      	18
#define CPCI_BI_ALARM6		      	19
#define CPCI_BI_ALARM7		      	20
#define CPCI_BI_PHASE_FF_OPTION		21
#define CPCI_BI_PHASE_MODIFY_OPTION	22
#define CPCI_BI_DRV_RESET		23
#define CPCI_BI_SG_MODE			24

/******* no int ***********
#define CPCI_BO_RUN_MODE		0
***************************/
#define CPCI_BO_INT_ENABLE		1
#define CPCI_BO_RF_RESET		2
#define CPCI_BO_SWEEP_OPTION		3
#define CPCI_BO_AMP_OPTION		4
#define CPCI_BO_AMP_FF_OPTION		5
#define CPCI_BO_AMP_MODIFY_OPTION	6
#define CPCI_BO_TUNE_OPTION		7
#define CPCI_BO_FRONT_TUNE_OPTION	8
#define CPCI_BO_TUNE_FF_OPTION		9
#define CPCI_BO_TUNE_MODIFY_OPTION	10
#define CPCI_BO_PHASE_OPTION		11
#define CPCI_BO_POINT_SWEEP      	12
#define CPCI_BO_ERROR_OPTION      	13
#define CPCI_BO_PHASE_FF_OPTION		14
#define CPCI_BO_PHASE_MODIFY_OPTION	15
#define CPCI_BO_DRV_RESET		16
#define CPCI_BO_SG_MODE			17

#define CPCI_AI_FIX_FREQUENCY           0
#define CPCI_AI_WORK_PERIOD             1
#define CPCI_AI_AMP_SET                 2
#define CPCI_AI_AMP_COEFFICIENT         3
#define CPCI_AI_AMP_P_SET               4
#define CPCI_AI_AMP_I_SET               5
#define CPCI_AI_AMP_I_SET1              6
#define CPCI_AI_AMP_I_SET2              7
#define CPCI_AI_AMP_I_SET3              8
#define CPCI_AI_BIAS_SET                9
#define CPCI_AI_FIX_TUNING_ANGLE        10
#define CPCI_AI_TUNING_ANGLE_OFFSET     11
#define CPCI_AI_TUNE_P_SET              12
#define CPCI_AI_TUNE_I_SET              13
#define CPCI_AI_TUNE_I_SET1             14
#define CPCI_AI_TUNE_I_SET2             15
#define CPCI_AI_TUNE_I_SET3             16
#define CPCI_AI_FRONT_BIAS_SET          17
#define CPCI_AI_FRONT_TUNE_P_SET        18
#define CPCI_AI_FRONT_TUNE_I_SET        19
#define CPCI_AI_FRONT_FIX_TUNING_ANGLE	20
#define CPCI_AI_PHASE_I			21
#define CPCI_AI_PHASE_P			22
#define CPCI_AI_INITIAL_PHASE		23
#define CPCI_AI_FF_DELAY		24
#define CPCI_AI_ARC_COUNT		25

#define CPCI_AO_FIX_FREQUENCY		0
#define CPCI_AO_WORK_PERIOD		1
#define CPCI_AO_AMP_SET			2
#define CPCI_AO_AMP_COEFFICIENT		3
#define CPCI_AO_AMP_P_SET		4
#define CPCI_AO_AMP_I_SET		5
#define CPCI_AO_AMP_I_SET1		6	
#define CPCI_AO_AMP_I_SET2		7
#define CPCI_AO_AMP_I_SET3		8
#define CPCI_AO_BIAS_SET		9
#define CPCI_AO_FIX_TUNING_ANGLE	10
#define CPCI_AO_TUNING_ANGLE_OFFSET	11
#define CPCI_AO_TUNE_P_SET		12
#define CPCI_AO_TUNE_I_SET		13
#define CPCI_AO_TUNE_I_SET1		14
#define CPCI_AO_TUNE_I_SET2		15
#define CPCI_AO_TUNE_I_SET3		16
#define CPCI_AO_FRONT_BIAS_SET		17
#define CPCI_AO_FRONT_TUNE_P_SET	18
#define CPCI_AO_FRONT_TUNE_I_SET	19
#define CPCI_AO_FRONT_FIX_TUNING_ANGLE  20
#define CPCI_AO_PHASE_I			21
#define CPCI_AO_PHASE_P			22
#define CPCI_AO_INITIAL_PHASE		23
#define CPCI_AO_FF_DELAY		24

/******* no int ***********
#define CPCI_LI_INT_NUM			0

#define CPCI_LO_INT_NUM			0
***************************/

#define CPCI_WF_1			0
#define CPCI_WF_2                       1
#define CPCI_WF_3                       2
#define CPCI_WF_4                       3
#define CPCI_WF_5                       4
#define CPCI_WF_6_A                     5
#define CPCI_WF_6_B                     6
#define CPCI_WF_7                       7
#define CPCI_WF_8                       8
#define CPCI_WF_AMP_SKEW		9
#define CPCI_WF_ERROR_ALL               10
#define CPCI_WF_ERROR_PHASE             11
#define CPCI_WF_ERROR_FRONT             12
#define CPCI_WF_ERROR_TOTAL             13
#define CPCI_WF_GRID	                14
#define CPCI_WF_FRONT                   15

#define CPCI_WF_WR_1                    0

/* check record parameter */
#define CHECK_BIPARM(PARM,VAL)\
        if (!strncmp(pbi->inp.value.vmeio.parm,(PARM),strlen((PARM)))) {\
                ((recPrivate*)pbi->dpvt)->function=VAL;\
                parmOK=1;\
                break;\
        }

#define CHECK_BOPARM(PARM,VAL)\
        if (!strncmp(pbo->out.value.vmeio.parm,(PARM),strlen((PARM)))) {\
                ((recPrivate*)pbo->dpvt)->function=VAL;\
                parmOK=1;\
                break;\
        }

#define CHECK_AIPARM(PARM,VAL)\
        if (!strncmp(pai->inp.value.vmeio.parm,(PARM),strlen((PARM)))) {\
                ((recPrivate*)pai->dpvt)->function=VAL;\
                parmOK=1;\
                break;\
        }

#define CHECK_AOPARM(PARM,VAL)\
        if (!strncmp(pao->out.value.vmeio.parm,(PARM),strlen((PARM)))) {\
                ((recPrivate*)pao->dpvt)->function=VAL;\
                parmOK=1;\
                break;\
        }

/******* no int ***********
#define CHECK_LIPARM(PARM,VAL)\
        if (!strncmp(pli->inp.value.vmeio.parm,(PARM),strlen((PARM)))) {\
                ((recPrivate*)pli->dpvt)->function=VAL;\
                parmOK=1;\
                break;\
        }

#define CHECK_LOPARM(PARM,VAL)\
        if (!strncmp(plo->out.value.vmeio.parm,(PARM),strlen((PARM)))) {\
                ((recPrivate*)plo->dpvt)->function=VAL;\
                parmOK=1;\
                break;\
        }
******************************/

#define CHECK_WFPARM(PARM,VAL)\
        if (!strncmp(pwf->inp.value.vmeio.parm,(PARM),strlen((PARM)))) {\
                ((recPrivate*)pwf->dpvt)->function=VAL;\
                parmOK=1;\
                break;\
        }

#define CHECK_WF_WR_PARM(PARM,VAL)\
        if (!strncmp(pwf->inp.value.vmeio.parm,(PARM),strlen((PARM)))) {\
                ((recPrivate*)pwf->dpvt)->function=VAL;\
                parmOK=1;\
                break;\
        }

/* variable declaration */
/***********************
extern unsigned int dmaWriteMoment[2];
***********************/

/* function prototypes */
static long init_bi(struct biRecord *pbi);
static long read_bi(struct biRecord *pbi);

static long init_bo(struct boRecord *pbo);
static long write_bo(struct boRecord *pbo);

static long init_ai(struct aiRecord *pai);
static long read_ai(struct aiRecord *pai);

static long init_ao(struct aoRecord *pao);
static long write_ao(struct aoRecord *pao);

/******* no int ***********
static long init_li(struct longinRecord *pli);
static long read_li(struct longinRecord *pli);

static long init_lo(struct longoutRecord *plo);
static long write_lo(struct longoutRecord *plo);
**************************/

static long init_wf(struct waveformRecord *pwf);
static long init_xwf(struct waveformRecord *pwf);
static long read_wf(struct waveformRecord *pwf);
static long read_xwf(struct waveformRecord *pwf);
static long wfget_ioint_info(int cmd, struct waveformRecord *pwf, IOSCANPVT *ppvt);

static long init_wf_wr(struct waveformRecord *pwf);
static long write_wf_wr(struct waveformRecord *pwf);

struct {
        long            number;
        DEVSUPFUN       dev_report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_bi;
}devBiD212 = {
        5,
        NULL,
        NULL,
        init_bi,
        NULL,
        read_bi
};

epicsExportAddress(dset,devBiD212);
struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       write_bo;
}devBoD212 = {
        5,
        NULL,
        NULL,
        init_bo,
        NULL,
        write_bo
};

epicsExportAddress(dset,devBoD212);

struct { 
        long            number;
        DEVSUPFUN       dev_report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record; 
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_ai;
        DEVSUPFUN       special_linconv;
}devAiD212 = {
        6,
        NULL,
        NULL,
        init_ai,
        NULL,
        read_ai,
        NULL};

epicsExportAddress(dset,devAiD212);

struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       write_ao;
        DEVSUPFUN       special_linconv;
}devAoD212 = {
        6,
        NULL,
        NULL,
        init_ao,
        NULL,
        write_ao,
        NULL};

epicsExportAddress(dset,devAoD212);


/******* no int ***********
struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_li;
} devLiD212 = {
        5,
        NULL,
        NULL,
        init_li,
        NULL,
        read_li
};

epicsExportAddress(dset,devLiD212);

struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       write_lo;
} devLoD212 = {
        5,
        NULL,
        NULL,
        init_lo,
        NULL,
        write_lo
};

epicsExportAddress(dset,devLoD212);
*************************/

struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_wf;
} devWfD212 = {
        5,
        NULL,
        NULL,
        init_wf,
        wfget_ioint_info,
        read_wf
};

epicsExportAddress(dset,devWfD212);

struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_wf;
} devWfWrD212 = {
        5,
        NULL,
        NULL,
        init_wf_wr,
        NULL,
        write_wf_wr
};

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_wf;
} devXWf = {
	5,
	NULL,
	NULL,
	init_xwf,
	NULL,
	read_xwf
};

epicsExportAddress(dset,devXWf);

epicsExportAddress(dset,devWfWrD212);

static long init_bi(struct biRecord *pbi) {
    int parmOK = 0;

    if (pbi->inp.type != VME_IO) {
        recGblRecordError(S_db_badField, (void *)pbi,
                          "devBiD212 Init_record, Illegal INP");
        pbi->pact = TRUE;
        return (S_db_badField);
    }

    pbi->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) pbi->dpvt)->pCard = getCardStruct(pbi->inp.value.vmeio.card);
    do {
    	/******* no int ***********
        CHECK_BIPARM("RUN_MODE",   CPCI_BI_RUN_MODE);
        ****************************/
        CHECK_BIPARM("INT_ENABLE",   CPCI_BI_INT_ENABLE);  
        CHECK_BIPARM("RF_RESET",   CPCI_BI_RF_RESET);
        CHECK_BIPARM("SWEEP_OPTION",   CPCI_BI_SWEEP_OPTION);
        CHECK_BIPARM("AMP_OPTION",   CPCI_BI_AMP_OPTION);
        CHECK_BIPARM("AMP_FF_OPTION",   CPCI_BI_AMP_FF_OPTION);
        CHECK_BIPARM("AMP_MODIFY_OPTION",   CPCI_BI_AMP_MODIFY_OPTION);
        CHECK_BIPARM("TUNE_OPTION",   CPCI_BI_TUNE_OPTION);
        CHECK_BIPARM("FRONT_TUNE_OPTION",   CPCI_BI_FRONT_TUNE_OPTION);
        CHECK_BIPARM("TUNE_FF_OPTION",   CPCI_BI_TUNE_FF_OPTION);
        CHECK_BIPARM("TUNE_MODIFY_OPTION",   CPCI_BI_TUNE_MODIFY_OPTION);
        CHECK_BIPARM("PHASE_OPTION",   CPCI_BI_PHASE_OPTION);
        CHECK_BIPARM("POINT_SWEEP",  CPCI_BI_POINT_SWEEP);
	CHECK_BIPARM("ALARM0",  CPCI_BI_ALARM0);
	CHECK_BIPARM("ALARM1",  CPCI_BI_ALARM1);
	CHECK_BIPARM("ALARM2",  CPCI_BI_ALARM2);
	CHECK_BIPARM("ALARM3",  CPCI_BI_ALARM3);
	CHECK_BIPARM("ALARM4",  CPCI_BI_ALARM4);
	CHECK_BIPARM("ALARM5",  CPCI_BI_ALARM5);
	CHECK_BIPARM("ALARM6",  CPCI_BI_ALARM6);
	CHECK_BIPARM("ALARM7",  CPCI_BI_ALARM7);
	CHECK_BIPARM("PHASE_FF_OPTION",  CPCI_BI_PHASE_FF_OPTION);
	CHECK_BIPARM("PHASE_MODIFY_OPTION",  CPCI_BI_PHASE_MODIFY_OPTION);
	CHECK_BIPARM("DRV_RESET",  CPCI_BI_DRV_RESET);
	CHECK_BIPARM("SG_MODE",  CPCI_BI_SG_MODE);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pbi,
                      "devBiD212 Init_record, bad parm");
        pbi->pact=TRUE;
        return (S_db_badField);
    }

    return 0;
}

static long read_bi(struct biRecord *pbi) {
    switch (((recPrivate*)pbi->dpvt)->function) {
    	/******* no int ***********
       case CPCI_BI_RUN_MODE:
           pbi->val = getRunMode();
           break;
       **************************/
       case CPCI_BI_INT_ENABLE:
           pbi->val = int_Enable_get(((recPrivate*)pbi->dpvt)->pCard);
           break;    
       case CPCI_BI_RF_RESET:
           pbi->val = RFReset_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_SWEEP_OPTION:
           pbi->val = SweepOption_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_AMP_OPTION:
           pbi->val = AMP_OPTION_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_AMP_FF_OPTION:
           pbi->val = AMP_FF_OPTION_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_AMP_MODIFY_OPTION:
           pbi->val = AMP_Modify_OPTION_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_TUNE_OPTION:
           pbi->val = Tune_OPTION_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_FRONT_TUNE_OPTION:
           pbi->val = Front_Tune_OPTION_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_TUNE_FF_OPTION:
           pbi->val = Tune_FF_OPTION_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_TUNE_MODIFY_OPTION:
           pbi->val = Tune_Modify_OPTION_get(((recPrivate*)pbi->dpvt)->pCard);
           break; 
       case CPCI_BI_PHASE_OPTION:
           pbi->val = Phase_OPTION_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_POINT_SWEEP:
           pbi->val = point_Sweep_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_ALARM0:
           pbi->val = alarm0_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_ALARM1:
           pbi->val = alarm1_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_ALARM2:
           pbi->val = alarm2_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_ALARM3:
           pbi->val = alarm3_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_ALARM4:
           pbi->val = alarm4_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_ALARM5:
           pbi->val = alarm5_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_ALARM6:
           pbi->val = alarm6_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_ALARM7:
           pbi->val = alarm7_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_PHASE_FF_OPTION:
           pbi->val = Phase_FF_Option_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_PHASE_MODIFY_OPTION:
           pbi->val = Phase_Modify_Option_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_DRV_RESET:
           pbi->val = Drv_Reset_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_SG_MODE:
           pbi->val = SG_Mode_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pbi,
                    "devBiD212 Read_bi, bad parm");
           return(S_db_badField);
    }

    pbi->udf=0;
    return 2;
}

static long init_bo(struct boRecord *pbo) {
    int parmOK = 0;

    if (pbo->out.type != VME_IO) {
        recGblRecordError(S_db_badField, (void *)pbo,
                          "devBoD212 Init_record, Illegal OUT");
        pbo->pact = TRUE;
        return (S_db_badField);
    }

    pbo->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) pbo->dpvt)->pCard = getCardStruct(pbo->out.value.vmeio.card);

    do {
    	/******* no int ***********
        CHECK_BOPARM("RUN_MODE",   CPCI_BO_RUN_MODE);
      ***************************/
        CHECK_BOPARM("INT_ENABLE",   CPCI_BO_INT_ENABLE);
        CHECK_BOPARM("RF_RESET",   CPCI_BO_RF_RESET);
        CHECK_BOPARM("SWEEP_OPTION",   CPCI_BO_SWEEP_OPTION);
        CHECK_BOPARM("AMP_OPTION",   CPCI_BO_AMP_OPTION);
        CHECK_BOPARM("AMP_FF_OPTION",   CPCI_BO_AMP_FF_OPTION);
        CHECK_BOPARM("AMP_MODIFY_OPTION",   CPCI_BO_AMP_MODIFY_OPTION);
        CHECK_BOPARM("TUNE_OPTION",   CPCI_BO_TUNE_OPTION);
        CHECK_BOPARM("FRONT_TUNE_OPTION",   CPCI_BO_FRONT_TUNE_OPTION);
        CHECK_BOPARM("TUNE_FF_OPTION",   CPCI_BO_TUNE_FF_OPTION);
        CHECK_BOPARM("TUNE_MODIFY_OPTION",   CPCI_BO_TUNE_MODIFY_OPTION);
        CHECK_BOPARM("PHASE_OPTION",   CPCI_BO_PHASE_OPTION);
        CHECK_BOPARM("POINT_SWEEP",   CPCI_BO_POINT_SWEEP);
        CHECK_BOPARM("ERROR_OPTION",   CPCI_BO_ERROR_OPTION);
        CHECK_BOPARM("PHASE_FF_OPTION",   CPCI_BO_PHASE_FF_OPTION);
        CHECK_BOPARM("PHASE_MODIFY_OPTION",   CPCI_BO_PHASE_MODIFY_OPTION);
        CHECK_BOPARM("DRV_RESET",   CPCI_BO_DRV_RESET);
        CHECK_BOPARM("SG_MODE",   CPCI_BO_SG_MODE);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pbo,
                      "devBoD212 Init_record, bad parm");
        pbo->pact=TRUE;
        return (S_db_badField);
    }

    /* init value */
    switch(((recPrivate*)pbo->dpvt)->function) {
    	/******* no int ***********
       case CPCI_BO_RUN_MODE:
           pbo->val=0;
           break;
      ***************************/
       case CPCI_BO_INT_ENABLE:
           pbo->val=0;
           break;
       case CPCI_BO_RF_RESET:
           pbo->val=0;
           break;
       case CPCI_BO_SWEEP_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_AMP_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_AMP_FF_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_AMP_MODIFY_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_TUNE_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_FRONT_TUNE_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_TUNE_FF_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_TUNE_MODIFY_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_PHASE_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_POINT_SWEEP:
           pbo->val=0;
           break;
       case CPCI_BO_ERROR_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_PHASE_FF_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_PHASE_MODIFY_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_DRV_RESET:
           pbo->val=0;
           break;
       case CPCI_BO_SG_MODE:
           pbo->val=0;
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pbo,
                    "devBoD212 Init_bo, bad parm");
           return(S_db_badField);
    }

    pbo->udf=0;
    return 2;
}

static long write_bo(struct boRecord *pbo) {

    switch (((recPrivate*)pbo->dpvt)->function) {
    	/******* no int ***********
       case CPCI_BO_RUN_MODE:
           if(pbo->val == 0)
              setRunMode(MODE_TEST);
           else
              setRunMode(MODE_NORMAL);
           break;
      ****************************/
       case CPCI_BO_INT_ENABLE:
           if(pbo->val == 0)
               int_Disable(((recPrivate*)pbo->dpvt)->pCard);
           else
               int_Enable(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_RF_RESET:
           if(pbo->val == 0)
               clear_RFReset_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_RFReset_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_SWEEP_OPTION:
           if(pbo->val == 0)
               clear_Sweep_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Sweep_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_AMP_OPTION:
           if(pbo->val == 0)
               clear_AMP_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_AMP_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_AMP_FF_OPTION:
           if(pbo->val == 0)
               clear_AMP_FF_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_AMP_FF_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_AMP_MODIFY_OPTION:
           if(pbo->val == 0)
               clear_AMP_Modify_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_AMP_Modify_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_TUNE_OPTION:
           if(pbo->val == 0)
               clear_Tune_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Tune_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_FRONT_TUNE_OPTION:
           if(pbo->val == 0)
               clear_Front_Tune_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Front_Tune_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_TUNE_FF_OPTION:
           if(pbo->val == 0)
               clear_Tune_FF_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Tune_FF_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_TUNE_MODIFY_OPTION:
           if(pbo->val == 0)
               clear_Tune_Modify_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Tune_Modify_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_PHASE_OPTION:
           if(pbo->val == 0)
               clear_Phase_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Phase_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_POINT_SWEEP:
           if(pbo->val == 0)
               point_Sweep_S(((recPrivate*)pbo->dpvt)->pCard);
           else
               point_Sweep_P(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_ERROR_OPTION:
           if(pbo->val == 0)
               ((recPrivate*)pbo->dpvt)->pCard->errorFlag = 0;
           else
               ((recPrivate*)pbo->dpvt)->pCard->errorFlag = 1;
           break;
       case CPCI_BO_PHASE_FF_OPTION:
           if(pbo->val == 0)
               clear_Phase_FF_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Phase_FF_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_PHASE_MODIFY_OPTION:
           if(pbo->val == 0)
               clear_Phase_Modify_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Phase_Modify_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_DRV_RESET:
           if(pbo->val == 0)
               clear_Drv_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Drv_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_SG_MODE:
           if(pbo->val == 0)
               clear_SG_Mode(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_SG_Mode(((recPrivate*)pbo->dpvt)->pCard);
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pbo,
                    "devBoD212 Write_bo, bad parm");
           return(S_db_badField);
    }

    pbo->udf=0;
    return 0;
}

static long init_ai(struct aiRecord *pai)
{
    int signal;
    int parmOK=0;

    signal = pai->inp.value.vmeio.signal;

    if (pai->inp.type!=VME_IO) {
        recGblRecordError(S_db_badField, (void *)pai,
                          "devLiD212 Init_record, Illegal INP");
        pai->pact=TRUE;
        return (S_db_badField);
    }

    pai->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) pai->dpvt)->pCard = getCardStruct(pai->inp.value.vmeio.card);    do {
        CHECK_AIPARM("FIX_FREQUENCY", CPCI_AI_FIX_FREQUENCY);
        CHECK_AIPARM("WORK_PERIOD", CPCI_AI_WORK_PERIOD);
        CHECK_AIPARM("AMP_SET", CPCI_AI_AMP_SET);
        CHECK_AIPARM("AMP_COEFFICIENT", CPCI_AI_AMP_COEFFICIENT);
        CHECK_AIPARM("AMP_P_SET", CPCI_AI_AMP_P_SET);
        CHECK_AIPARM("AMP_I_SET", CPCI_AI_AMP_I_SET);
        CHECK_AIPARM("AMP_I1_SET", CPCI_AI_AMP_I_SET1);
        CHECK_AIPARM("AMP_I2_SET", CPCI_AI_AMP_I_SET2);
        CHECK_AIPARM("AMP_I3_SET", CPCI_AI_AMP_I_SET3);
        CHECK_AIPARM("BIAS_SET", CPCI_AI_BIAS_SET);
        CHECK_AIPARM("FIX_TUNING_ANGLE", CPCI_AI_FIX_TUNING_ANGLE);
        CHECK_AIPARM("TUNING_ANGLE_OFFSET", CPCI_AI_TUNING_ANGLE_OFFSET);
        CHECK_AIPARM("TUNE_P_SET", CPCI_AI_TUNE_P_SET);
        CHECK_AIPARM("TUNE_I_SET", CPCI_AI_TUNE_I_SET);
        CHECK_AIPARM("TUNE_I1_SET", CPCI_AI_TUNE_I_SET1);
        CHECK_AIPARM("TUNE_I2_SET", CPCI_AI_TUNE_I_SET2);
        CHECK_AIPARM("TUNE_I3_SET", CPCI_AI_TUNE_I_SET3);
        CHECK_AIPARM("FRONT_BIAS_SET", CPCI_AI_FRONT_BIAS_SET);
        CHECK_AIPARM("FRONT_TUNE_P_SET", CPCI_AI_FRONT_TUNE_P_SET);
        CHECK_AIPARM("FRONT_TUNE_I_SET", CPCI_AI_FRONT_TUNE_I_SET);
        CHECK_AIPARM("FRONT_FIX_TUNING_ANGLE", CPCI_AI_FRONT_FIX_TUNING_ANGLE);
        CHECK_AIPARM("PHASE_I", CPCI_AI_PHASE_I);
	CHECK_AIPARM("PHASE_P", CPCI_AI_PHASE_P);
	CHECK_AIPARM("INITIAL_PHASE", CPCI_AI_INITIAL_PHASE);
        CHECK_AIPARM("FF_DELAY", CPCI_AI_FF_DELAY);
        CHECK_AIPARM("ARC_COUNT", CPCI_AI_ARC_COUNT);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pai,
                      "devLiD212 Init_record, bad parm");
        pai->pact=TRUE;
        return (S_db_badField);
    }

    return 0;
}

static long read_ai(struct aiRecord *pai) {
    int signal;
    signal = pai->inp.value.vmeio.signal;

    switch (((recPrivate*)pai->dpvt)->function) {
       case CPCI_AI_FIX_FREQUENCY:
           pai->val=get_Fix_Frequency(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_WORK_PERIOD:
           pai->val=get_Work_Period(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_AMP_SET:
           pai->val=get_AMP(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_AMP_COEFFICIENT:
           pai->val=get_AMP_Coefficient(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_AMP_P_SET:
           pai->val=get_AMP_P(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_AMP_I_SET:
           pai->val=get_AMP_I(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_AMP_I_SET1:
           pai->val=get_AMP_I_1(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_AMP_I_SET2:
           pai->val=get_AMP_I_2(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_AMP_I_SET3:
           pai->val=get_AMP_I_3(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_BIAS_SET:
           pai->val=get_Bias(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_FIX_TUNING_ANGLE:
           pai->val=get_Fix_Tuning_Angle(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_TUNING_ANGLE_OFFSET:
           pai->val=get_Tuning_Angle_Offset(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_TUNE_P_SET:
           pai->val=get_Tune_P(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_TUNE_I_SET:
           pai->val=get_Tune_I(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_TUNE_I_SET1:
           pai->val=get_Tune_I_1(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_TUNE_I_SET2:
           pai->val=get_Tune_I_2(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_TUNE_I_SET3:
           pai->val=get_Tune_I_3(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_FRONT_BIAS_SET:
           pai->val=get_Front_Bias(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_FRONT_TUNE_P_SET:
           pai->val=get_Front_Tune_P(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_FRONT_TUNE_I_SET:
           pai->val=get_Front_Tune_I(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_FRONT_FIX_TUNING_ANGLE:
           pai->val=get_Front_Fix_Tuning_Angle(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_PHASE_I:
           pai->val=get_Phase_i(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_PHASE_P:
           pai->val=get_Phase_p(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_INITIAL_PHASE:
	   pai->val=get_Initial_Phase(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_FF_DELAY:
	   pai->val=get_FF_Delay(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_ARC_COUNT:
	   pai->val=get_ARC_COUNT(((recPrivate*)pai->dpvt)->pCard);
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pai,
                    "devAiD212 Read_ai, bad parm");
           return(S_db_badField);
    }

    pai->udf=0;
    return 2;
}

static long init_ao(struct aoRecord *pao)
{
    int signal;
    int parmOK=0;

    signal = pao->out.value.vmeio.signal;

    if (pao->out.type!=VME_IO) {
        recGblRecordError(S_db_badField, (void *)pao,
                          "devAoD212 Init_record, Illegal OUT");
        pao->pact=TRUE;
        return (S_db_badField);
    }

    pao->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) pao->dpvt)->pCard = getCardStruct(pao->out.value.vmeio.card);
    do {
        CHECK_AOPARM("FIX_FREQUENCY", CPCI_AO_FIX_FREQUENCY);
        CHECK_AOPARM("WORK_PERIOD", CPCI_AO_WORK_PERIOD);
        CHECK_AOPARM("AMP_SET", CPCI_AO_AMP_SET);
        CHECK_AOPARM("AMP_COEFFICIENT", CPCI_AO_AMP_COEFFICIENT);
        CHECK_AOPARM("AMP_P_SET", CPCI_AO_AMP_P_SET);
        CHECK_AOPARM("AMP_I_SET", CPCI_AO_AMP_I_SET);
        CHECK_AOPARM("AMP_I1_SET", CPCI_AO_AMP_I_SET1);
        CHECK_AOPARM("AMP_I2_SET", CPCI_AO_AMP_I_SET2);
        CHECK_AOPARM("AMP_I3_SET", CPCI_AO_AMP_I_SET3);
        CHECK_AOPARM("BIAS_SET", CPCI_AO_BIAS_SET);
        CHECK_AOPARM("FIX_TUNING_ANGLE", CPCI_AO_FIX_TUNING_ANGLE);
        CHECK_AOPARM("TUNING_ANGLE_OFFSET", CPCI_AO_TUNING_ANGLE_OFFSET);
        CHECK_AOPARM("TUNE_P_SET", CPCI_AO_TUNE_P_SET);
        CHECK_AOPARM("TUNE_I_SET", CPCI_AO_TUNE_I_SET);
        CHECK_AOPARM("TUNE_I1_SET", CPCI_AO_TUNE_I_SET1);
        CHECK_AOPARM("TUNE_I2_SET", CPCI_AO_TUNE_I_SET2);
        CHECK_AOPARM("TUNE_I3_SET", CPCI_AO_TUNE_I_SET3);
        CHECK_AOPARM("FRONT_BIAS_SET", CPCI_AO_FRONT_BIAS_SET);
        CHECK_AOPARM("FRONT_TUNE_P_SET", CPCI_AO_FRONT_TUNE_P_SET);
        CHECK_AOPARM("FRONT_TUNE_I_SET", CPCI_AO_FRONT_TUNE_I_SET);
        CHECK_AOPARM("FRONT_FIX_TUNING_ANGLE", CPCI_AO_FRONT_FIX_TUNING_ANGLE);
        CHECK_AOPARM("PHASE_I", CPCI_AO_PHASE_I);
	CHECK_AOPARM("PHASE_P", CPCI_AO_PHASE_P);
	CHECK_AOPARM("INITIAL_PHASE", CPCI_AO_INITIAL_PHASE);
        CHECK_AOPARM("FF_DELAY", CPCI_AO_FF_DELAY);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pao,
                      "devAoD212 Init_record, bad parm");
        pao->pact=TRUE;
        return (S_db_badField);
    }

    /* init value */
    switch (((recPrivate*)pao->dpvt)->function) {
       case CPCI_AO_FIX_FREQUENCY:
           pao->val=0.0;
           break;
       case CPCI_AO_WORK_PERIOD:
           pao->val=0.0;
           break;
       case CPCI_AO_AMP_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_AMP_COEFFICIENT:
           pao->val=0.0;
           break;
       case CPCI_AO_AMP_P_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_AMP_I_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_AMP_I_SET1:
           pao->val=0.0;
           break;
       case CPCI_AO_AMP_I_SET2:
           pao->val=0.0;
           break;
       case CPCI_AO_AMP_I_SET3:
           pao->val=0.0;
           break;
       case CPCI_AO_BIAS_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_FIX_TUNING_ANGLE:
           pao->val=0.0;
           break;
       case CPCI_AO_TUNING_ANGLE_OFFSET:
           pao->val=0.0;
           break;
       case CPCI_AO_TUNE_P_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_TUNE_I_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_TUNE_I_SET1:
           pao->val=0.0;
           break;
       case CPCI_AO_TUNE_I_SET2:
           pao->val=0.0;
           break;
       case CPCI_AO_TUNE_I_SET3:
           pao->val=0.0;
           break;
       case CPCI_AO_FRONT_BIAS_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_FRONT_TUNE_P_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_FRONT_TUNE_I_SET:
           pao->val=0.0;
           break;
       case CPCI_AO_FRONT_FIX_TUNING_ANGLE:
           pao->val=0.0;
           break;
       case CPCI_AO_PHASE_I:
           pao->val=0.0;
           break;
       case CPCI_AO_PHASE_P:
           pao->val=0.0;
           break;
       case CPCI_AO_INITIAL_PHASE:
	   pao->val=0.0;
	   break;
       case CPCI_AO_FF_DELAY:
	   pao->val=0.0;
	   break;
       default:
           recGblRecordError(S_db_badField,(void *)pao,
                    "devAoD212 Init_ao, bad parm");
           return(S_db_badField);
    }

    pao->udf=0;
    return 2;
   
}

static long write_ao(struct aoRecord *pao) {
    int signal;
    signal = pao->out.value.vmeio.signal;

    switch (((recPrivate*)pao->dpvt)->function) {
       case CPCI_AO_FIX_FREQUENCY:
           set_Fix_Frequency(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_WORK_PERIOD:
           set_Work_Period(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_AMP_SET:
           set_AMP(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_AMP_COEFFICIENT:
           set_AMP_Coefficient(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_AMP_P_SET:
           set_AMP_P(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_AMP_I_SET:
           set_AMP_I(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_AMP_I_SET1:
           set_AMP_I_1(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_AMP_I_SET2:
           set_AMP_I_2(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_AMP_I_SET3:
           set_AMP_I_3(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_BIAS_SET:
           set_Bias(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_FIX_TUNING_ANGLE:
           set_Fix_Tuning_Angle(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_TUNING_ANGLE_OFFSET:
           set_Tuning_Angle_Offset(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_TUNE_P_SET:
           set_Tune_P(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_TUNE_I_SET:
           set_Tune_I(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_TUNE_I_SET1:
           set_Tune_I_1(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_TUNE_I_SET2:
           set_Tune_I_2(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_TUNE_I_SET3:
           set_Tune_I_3(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_FRONT_BIAS_SET:
           set_Front_Bias(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_FRONT_TUNE_P_SET:
           set_Front_Tune_P(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_FRONT_TUNE_I_SET:
           set_Front_Tune_I(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;       
       case CPCI_AO_FRONT_FIX_TUNING_ANGLE:
           set_Front_Fix_Tuning_Angle(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_PHASE_I:
           set_phase_i(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_PHASE_P:
           set_phase_p(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_INITIAL_PHASE:
           set_Initial_Phase(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_FF_DELAY:
           set_FF_Delay(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pao,
                    "devAoD212 Write_ao, bad parm");
           return(S_db_badField);
    }

    pao->udf=0;
    return 0;
}


/******* no int ***********
static long init_li(struct longinRecord *pli) {
    int parmOK=0;

    if (pli->inp.type!=VME_IO) {
        recGblRecordError(S_db_badField, (void *)pli,
                          "devLiD212 Init_record, Illegal INP");
        pli->pact=TRUE;
        return (S_db_badField);
    }

    pli->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) pli->dpvt)->pCard = getCardStruct(pli->inp.value.vmeio.card);
    do {
        CHECK_LIPARM("INT_NUM", CPCI_LI_INT_NUM);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pli,
                      "devLiD212 Init_record, bad parm");
        pli->pact=TRUE;
        return (S_db_badField);
    }

    return 0;
}

static long read_li(struct longinRecord *pli) {
    switch (((recPrivate*)pli->dpvt)->function) {
       case CPCI_LI_INT_NUM:
          pli->val = getInterruptNum(((recPrivate*)pli->dpvt)->pCard);
          break;
       default:
           recGblRecordError(S_db_badField,(void *)pli,
                    "devLiD212 Read_li, bad parm");
           return(S_db_badField);
    }

    pli->udf=0;
    return 0;
}

static long init_lo(struct longoutRecord *plo) {
    int parmOK=0;

    if (plo->out.type!=VME_IO) {
        recGblRecordError(S_db_badField, (void *)plo,
                          "devLoD212 Init_record, Illegal OUT");
        plo->pact=TRUE;
        return (S_db_badField);
    }

    plo->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) plo->dpvt)->pCard = getCardStruct(plo->out.value.vmeio.card);

    do {
        CHECK_LOPARM("INT_NUM", CPCI_LO_INT_NUM);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)plo,
                      "devLoD212 Init_record, bad parm");
        plo->pact=TRUE;
        return (S_db_badField);
    }
    

    switch (((recPrivate*)plo->dpvt)->function) {
       case CPCI_LO_INT_NUM:
           plo->val=0;
           break;
       default:
           recGblRecordError(S_db_badField,(void *)plo,
                    "devLoD212 Init_lo, bad parm");
           return(S_db_badField);
    }

    plo->udf=0;
    return 0;
}

static long write_lo(struct longoutRecord *plo) {

    switch (((recPrivate*)plo->dpvt)->function) {
       case CPCI_LO_INT_NUM:
          setInterruptNum(((recPrivate*)plo->dpvt)->pCard, plo->val); 
          break;

       default:
           recGblRecordError(S_db_badField,(void *)plo,
                    "devLoD212 Write_lo, bad parm");
           return(S_db_badField);
    }

    plo->udf=0;
    return 0;
}

***********************************/

static long init_wf(struct waveformRecord *pwf) {
    int parmOK = 0;

    if (pwf->inp.type!=VME_IO) {
        recGblRecordError(S_db_badField, (void *)pwf,
                          "devWfD212 Init_record, Illegal INP");
        pwf->pact=TRUE;
        return (S_db_badField);
    }

    pwf->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) pwf->dpvt)->pCard = getCardStruct(pwf->inp.value.vmeio.card);

    do {
        CHECK_WFPARM("WF_1", CPCI_WF_1);
        CHECK_WFPARM("WF_2", CPCI_WF_2);
        CHECK_WFPARM("WF_3", CPCI_WF_3);
        CHECK_WFPARM("WF_4", CPCI_WF_4);
        CHECK_WFPARM("WF_5", CPCI_WF_5);
        CHECK_WFPARM("WF_6_A", CPCI_WF_6_A);
        CHECK_WFPARM("WF_6_B", CPCI_WF_6_B);
        CHECK_WFPARM("WF_7", CPCI_WF_7);
        CHECK_WFPARM("WF_8", CPCI_WF_8);
        CHECK_WFPARM("WF_AMP_SKEW", CPCI_WF_AMP_SKEW);
        CHECK_WFPARM("WF_ERROR_ALL", CPCI_WF_ERROR_ALL);
        CHECK_WFPARM("WF_ERROR_PHASE", CPCI_WF_ERROR_PHASE);
        CHECK_WFPARM("WF_ERROR_FRONT", CPCI_WF_ERROR_FRONT);
        CHECK_WFPARM("WF_ERROR_TOTAL", CPCI_WF_ERROR_TOTAL);
        CHECK_WFPARM("WF_GRID", CPCI_WF_GRID);
        CHECK_WFPARM("WF_FRONT", CPCI_WF_FRONT);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pwf,
                      "devWfD212 Init_record, bad parm");
        pwf->pact=TRUE;
        return (S_db_badField);
    }

    if (pwf->ftvl != DBF_FLOAT) {
        recGblRecordError(S_db_badField, (void *)pwf,
                   "devWfD212 (init_record) Illegal FTVL field");
        return(S_db_badField);
    }

    return 0;
}

static long read_wf(struct waveformRecord *pwf) {
    int numRead = pwf->nelm;
    float *pSrc;
    float *pDest = pwf->bptr;
    switch (((recPrivate*)pwf->dpvt)->function) {
       case CPCI_WF_1:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF1_FADDR + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_2:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF2_FADDR + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_3:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF3_FADDR + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_4:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF4_FADDR + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_5:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF5_FADDR + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_6_A:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF6_FADDR_A + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_6_B:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF6_FADDR_B + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_7:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF7_FADDR + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_8:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF8_FADDR + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_AMP_SKEW:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->ampSkewBuffer + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_ERROR_ALL:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->errorAllBuffer + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_ERROR_PHASE:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->errorPhaseBuffer + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_ERROR_FRONT:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->errorFrontBuffer + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_ERROR_TOTAL:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->errorTotalBuffer + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_GRID:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->gridBuffer + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_FRONT:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->frontBuffer + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       default:
           recGblRecordError(S_db_badField,(void *)pwf,
                    "devWfD212 Read_wf, bad parm");
           return(S_db_badField);
    } 

    pwf->nord = numRead;

    return 0;
}


static long init_xwf(struct waveformRecord *pwf) {

    switch(pwf->inp.type){
	case CONSTANT:
	case PV_LINK:
	case DB_LINK:
	case CA_LINK:
		break;
	default:
		recGblRecordError(S_db_badField,(void *)pwf,
                    "devXWF (init_xwf) Illegal INP field");
		return(S_db_badField);
    }
    return 0;
}

static long read_xwf(struct waveformRecord *pwf) {
    int numRead = pwf->nelm;
    float *pDest;
    float a[2048]={0.0};
    int i;
    	for(i=0;i<2048;i++)
	{
		a[i]=i*22.5/2047-2.5;
	}
	pDest=pwf->bptr;
	memcpy(pDest,&a[0],2048*sizeof(float));
	if(numRead>0){
		pwf->nord=numRead;
		if(pwf->tsel.type==CONSTANT && pwf->tse==epicsTimeEventDeviceTime)
			dbGetTimeStamp(&pwf->inp,&pwf->time);
	}
   return 0;
}

static long wfget_ioint_info(int cmd, struct waveformRecord *pwf,
                           IOSCANPVT *ppvt) {
    *ppvt = ((recPrivate*)pwf->dpvt)->pCard->ioScanPvt;
    return 0;
}

static long init_wf_wr(struct waveformRecord *pwf) {
    int parmOK = 0;

    if (pwf->inp.type!=VME_IO) {
        recGblRecordError(S_db_badField, (void *)pwf,
                          "devWfWrD212 Init_record, Illegal INP");
        pwf->pact=TRUE;
        return (S_db_badField);
    }

    pwf->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) pwf->dpvt)->pCard = getCardStruct(pwf->inp.value.vmeio.card);

    do {
        CHECK_WFPARM("WF_WR_1", CPCI_WF_WR_1);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pwf,
                      "devWfWrD212 Init_record, bad parm");
        pwf->pact=TRUE;
        return (S_db_badField);
    }

    if (pwf->ftvl != DBF_FLOAT) {
        recGblRecordError(S_db_badField, (void *)pwf,
                   "devWfWrD212 (init_record) Illegal FTVL field");
        return(S_db_badField);
    }

    return 0;
}

static long write_wf_wr(struct waveformRecord *pwf) {
    int i;
    D212Card *pCard = ((recPrivate*)pwf->dpvt)->pCard;
    int numWrite = pwf->nelm;
    float *pSrc = pwf->bptr;
    int *pDest = ((recPrivate*)pwf->dpvt)->pCard->writeBuffer;
    for(i=0; i<numWrite; i++, pDest++, pSrc++)
       *pDest = *pSrc * 1.0 + 0.0; 
    switch (((recPrivate*)pwf->dpvt)->function) {
       case CPCI_WF_WR_1:
            /*******************************
            dmaWriteMoment[0] = sysTimeBaseLGet();
***********************************************/

            /* start DMA Channel 1 transfer, write data, 2048 bytes */
            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA1_PCI_ADR, (unsigned int) (pCard->writeBuffer));
/*            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA1_LOCAL_ADR, REG_FPGA_VERSION); */
            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA1_SIZE, DMA_WRITE_NUM);
            BRIDGE_REG_WRITE32(pCard->bridgeAddr, REG_9656_DMA1_DPR, 0x00000000);
            BRIDGE_REG_WRITE8(pCard->bridgeAddr, REG_9656_DMA1_CSR, 0x03);
          break;
       default:
           recGblRecordError(S_db_badField,(void *)pwf,
                    "devWfWrD212 Read_wf, bad parm");
           return(S_db_badField);
    }

    pwf->nord = numWrite;

    return 0;
}

