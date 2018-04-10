#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <semLib.h>
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
#define CPCI_BI_ALARM8		      	21
#define CPCI_BI_PHASE_FF_OPTION		22
#define CPCI_BI_PHASE_MODIFY_OPTION	23
#define CPCI_BI_DRV_RESET		24
#define CPCI_BI_SG_MODE			25
#define CPCI_BI_FRE_CHANGE_OPTION	26
#define CPCI_BI_AMP_CHANGE_OPTION	27
#define CPCI_BI_BEAM_INT		28

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
#define CPCI_BO_FRE_CHANGE_OPTION	18
#define CPCI_BO_AMP_CHANGE_OPTION	19
#define CPCI_BO_BEAM_INT		20
#define CPCI_BO_CURVE_CHANGE_OPTION	21

#define CPCI_AI_FIX_FREQUENCY           0
#define CPCI_AI_WORK_PERIOD             1
#define CPCI_AI_AMP_SET                 2
#define CPCI_AI_AMP_COEFFICIENT         3
#define CPCI_AI_AMP_P_SET               4
#define CPCI_AI_AMP_I_SET               5
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
#define CPCI_AI_PRETRIG_DELAY		26
#define CPCI_AI_INITIAL_REF_PHASE	27
#define CPCI_AI_INT_DELAY		28
#define CPCI_AI_CHOPPER_DUTY		29
#define CPCI_AI_RF_HARMONIC		30
#define CPCI_AI_EX_PHASE		31
#define CPCI_AI_RBF_DELAY		32
#define CPCI_AI_BPM_DELAY_SET		33
#define CPCI_AI_CHOPPER_PAHSE_SET	34
#define CPCI_AI_EX_DELAY_SET		35


#define CPCI_AO_FIX_FREQUENCY		0
#define CPCI_AO_WORK_PERIOD		1
#define CPCI_AO_AMP_SET			2
#define CPCI_AO_AMP_COEFFICIENT		3
#define CPCI_AO_AMP_P_SET		4
#define CPCI_AO_AMP_I_SET		5
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
#define CPCI_AO_PRETRIG_DELAY		25
#define CPCI_AO_INITIAL_REF_PHASE	26
#define CPCI_AO_INT_DELAY		27
#define CPCI_AO_CHOPPER_DUTY		28
#define CPCI_AO_RF_HARMONIC		29
#define CPCI_AO_ALL_FREQUENCY		30
#define CPCI_AO_ALL_BEAM_PHASE		31
#define CPCI_AO_ALL_PRETRIG		32
#define CPCI_AO_ALL_AMP_COEFFIC		33
#define CPCI_AO_EX_PHASE		34
#define CPCI_AO_RBF_DELAY		35
#define CPCI_AO_BPM_DELAY_SET		36
#define CPCI_AO_CHOPPER_PAHSE_SET	37
#define CPCI_AO_EX_DELAY_SET		38


/******* no int ***********
#define CPCI_LI_INT_NUM			0

#define CPCI_LO_INT_NUM			0
***************************/

#define CPCI_WF_1			0
#define CPCI_WF_2                       1
#define CPCI_WF_3                       2
#define CPCI_WF_4_A                     3
#define CPCI_WF_4_B                     4
#define CPCI_WF_5_A                     5
#define CPCI_WF_5_B                     6
#define CPCI_WF_6_A                     7
#define CPCI_WF_6_B                     8
#define CPCI_WF_7                       9
#define CPCI_WF_8                       10
#define CPCI_WF_AMP_SKEW		11
#define CPCI_WF_GRID	                12
#define CPCI_WF_FRONT                   13

#define CPCI_WR_RD1	                0
#define CPCI_WR_RD2	                1

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
/* function prototypes */
static long init_bi(struct biRecord *pbi);
static long read_bi(struct biRecord *pbi);

static long init_bo(struct boRecord *pbo);
static long write_bo(struct boRecord *pbo);

static long init_ai(struct aiRecord *pai);
static long read_ai(struct aiRecord *pai);

static long init_ao(struct aoRecord *pao);
static long write_ao(struct aoRecord *pao);

static long init_wf(struct waveformRecord *pwf);
static long init_xwf(struct waveformRecord *pwf);
static long read_wf(struct waveformRecord *pwf);
static long read_xwf(struct waveformRecord *pwf);
static long init_wf_wr_1(struct waveformRecord *pwf);
static long write_wf_wr_1(struct waveformRecord *pwf);
static long init_wf_wr_2(struct waveformRecord *pwf);
static long write_wf_wr_2(struct waveformRecord *pwf);
static long init_wr_rd(struct waveformRecord *pwf);
static long read_wr_rd(struct waveformRecord *pwf);
static long wfget_ioint_info(int cmd, struct waveformRecord *pwf, IOSCANPVT *ppvt);

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

struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_wf;
} devWfWr1 = {
        5,
        NULL,
        NULL,
        init_wf_wr_1,
        NULL,
        write_wf_wr_1
};

epicsExportAddress(dset,devWfWr1);

struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_wf;
} devWfWr2 = {
        5,
        NULL,
        NULL,
        init_wf_wr_2,
        NULL,
        write_wf_wr_2
};

epicsExportAddress(dset,devWfWr2);

struct {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_wf;
} devWrRd = {
        5,
        NULL,
        NULL,
        init_wr_rd,
        NULL,
        read_wr_rd
};

epicsExportAddress(dset,devWrRd);


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
	CHECK_BIPARM("ALARM8",  CPCI_BI_ALARM8);
	CHECK_BIPARM("PHASE_FF_OPTION",  CPCI_BI_PHASE_FF_OPTION);
	CHECK_BIPARM("PHASE_MODIFY_OPTION",  CPCI_BI_PHASE_MODIFY_OPTION);
	CHECK_BIPARM("DRV_RESET",  CPCI_BI_DRV_RESET);
	CHECK_BIPARM("SG_MODE",  CPCI_BI_SG_MODE);
	CHECK_BIPARM("FRE_CHANGE_OPTION",  CPCI_BI_FRE_CHANGE_OPTION);
	CHECK_BIPARM("AMP_CHANGE_OPTION",  CPCI_BI_AMP_CHANGE_OPTION);
	CHECK_BIPARM("BEAM_INT",  CPCI_BI_BEAM_INT);
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
       case CPCI_BI_ALARM8:
           pbi->val = alarm8_get(((recPrivate*)pbi->dpvt)->pCard);
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
       case CPCI_BI_FRE_CHANGE_OPTION:
           pbi->val = Fre_Change_Option_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_AMP_CHANGE_OPTION:
           pbi->val = Amp_Change_Option_get(((recPrivate*)pbi->dpvt)->pCard);
           break;
       case CPCI_BI_BEAM_INT:
           pbi->val = beam_Int_get(((recPrivate*)pbi->dpvt)->pCard);
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
	CHECK_BOPARM("FRE_CHANGE_OPTION",   CPCI_BO_FRE_CHANGE_OPTION);
	CHECK_BOPARM("AMP_CHANGE_OPTION",   CPCI_BO_AMP_CHANGE_OPTION);
	CHECK_BOPARM("BEAM_INT",   CPCI_BO_BEAM_INT);
	CHECK_BOPARM("CURVE_CHANGE",   CPCI_BO_CURVE_CHANGE_OPTION);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pbo,
                      "devBoD212 Init_record, bad parm");
        pbo->pact=TRUE;
        return (S_db_badField);
    }

    /* init value */
    switch(((recPrivate*)pbo->dpvt)->function) {
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
       case CPCI_BO_FRE_CHANGE_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_AMP_CHANGE_OPTION:
           pbo->val=0;
           break;
       case CPCI_BO_BEAM_INT:
           pbo->val=0;
           break;
       case CPCI_BO_CURVE_CHANGE_OPTION:
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
       int i = 0;
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
               clear_point_Sweep(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_point_Sweep(((recPrivate*)pbo->dpvt)->pCard);
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
       case CPCI_BO_FRE_CHANGE_OPTION:
           if(pbo->val == 0)
               clear_Fre_Change_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Fre_Change_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_AMP_CHANGE_OPTION:
           if(pbo->val == 0)
               clear_Amp_Change_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Amp_Change_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_BEAM_INT:
           if(pbo->val == 0)
               clear_beam_Int(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_beam_Int(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_CURVE_CHANGE_OPTION:
           if(pbo->val == 0){
               clear_curve_Change();
	       for(i=0;i<9;i++){
	           int_Enable(getCardStruct(i));
	       }
           }
           else
           {
               set_curve_Change();
	       for(i=0;i<9;i++){
	           int_Enable(getCardStruct(i));
	       }
           }
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
        CHECK_AIPARM("BIAS_SET", CPCI_AI_BIAS_SET);
        CHECK_AIPARM("FIX_TUNING_ANGLE", CPCI_AI_FIX_TUNING_ANGLE);
        CHECK_AIPARM("TUNING_ANGLE_OFFSET", CPCI_AI_TUNING_ANGLE_OFFSET);
        CHECK_AIPARM("TUNE_P_SET", CPCI_AI_TUNE_P_SET);
        CHECK_AIPARM("TUNE_I_SET", CPCI_AI_TUNE_I_SET);
        CHECK_AIPARM("TUNE_I1", CPCI_AI_TUNE_I_SET1);
        CHECK_AIPARM("TUNE_I2", CPCI_AI_TUNE_I_SET2);
        CHECK_AIPARM("TUNE_I3", CPCI_AI_TUNE_I_SET3);
        CHECK_AIPARM("FRONT_BIAS_SET", CPCI_AI_FRONT_BIAS_SET);
        CHECK_AIPARM("FRONT_TUNE_P_SET", CPCI_AI_FRONT_TUNE_P_SET);
        CHECK_AIPARM("FRONT_TUNE_I_SET", CPCI_AI_FRONT_TUNE_I_SET);
        CHECK_AIPARM("FRONT_FIX_TUNING_ANGLE", CPCI_AI_FRONT_FIX_TUNING_ANGLE);
        CHECK_AIPARM("PHASE_I", CPCI_AI_PHASE_I);
	CHECK_AIPARM("PHASE_P", CPCI_AI_PHASE_P);
	CHECK_AIPARM("INITIAL_PHASE", CPCI_AI_INITIAL_PHASE);
        CHECK_AIPARM("FF_DELAY", CPCI_AI_FF_DELAY);
        CHECK_AIPARM("ARC_COUNT", CPCI_AI_ARC_COUNT);
	CHECK_AIPARM("PRETRIG_DELAY", CPCI_AI_PRETRIG_DELAY);
	CHECK_AIPARM("INITIAL_REF_PHASE", CPCI_AI_INITIAL_REF_PHASE);
	CHECK_AIPARM("INT_DELAY", CPCI_AI_INT_DELAY);
	CHECK_AIPARM("CHOPPER_DUTY", CPCI_AI_CHOPPER_DUTY);
	CHECK_AIPARM("RF_HARMONIC", CPCI_AI_RF_HARMONIC);
	CHECK_AIPARM("EX_PHASE", CPCI_AI_EX_PHASE);
	CHECK_AIPARM("RBF_DELAY", CPCI_AI_RBF_DELAY);
	CHECK_AIPARM("BPM_DELAY_SET", CPCI_AI_BPM_DELAY_SET);
	CHECK_AIPARM("CHOPPER_PAHSE_SET", CPCI_AI_CHOPPER_PAHSE_SET);
	CHECK_AIPARM("EX_DELAY_SET", CPCI_AI_EX_DELAY_SET);
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
       case CPCI_AI_PRETRIG_DELAY:
	   pai->val=get_PreTrig_Delay(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_INITIAL_REF_PHASE:
	   pai->val=get_Initial_Ref_Phase(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_INT_DELAY:
	   pai->val=get_Int_Delay(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_CHOPPER_DUTY:
	   pai->val=get_Chopper_Duty(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_RF_HARMONIC:
	   pai->val=get_Rf_Harmonic(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_EX_PHASE:
	   pai->val=get_EX_Phase(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_RBF_DELAY:
	   pai->val=get_RBF_Delay(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_BPM_DELAY_SET:
	   pai->val=get_BPM_Delay_Set(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_CHOPPER_PAHSE_SET:
	   pai->val=get_Chopper_Phase_Set(((recPrivate*)pai->dpvt)->pCard);
           break;
       case CPCI_AI_EX_DELAY_SET:
	   pai->val=get_EX_Delay_set(((recPrivate*)pai->dpvt)->pCard);
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
        CHECK_AOPARM("BIAS_SET", CPCI_AO_BIAS_SET);
        CHECK_AOPARM("FIX_TUNING_ANGLE", CPCI_AO_FIX_TUNING_ANGLE);
        CHECK_AOPARM("TUNING_ANGLE_OFFSET", CPCI_AO_TUNING_ANGLE_OFFSET);
        CHECK_AOPARM("TUNE_P_SET", CPCI_AO_TUNE_P_SET);
        CHECK_AOPARM("TUNE_I_SET", CPCI_AO_TUNE_I_SET);
        CHECK_AOPARM("TUNE_I1", CPCI_AO_TUNE_I_SET1);
        CHECK_AOPARM("TUNE_I2", CPCI_AO_TUNE_I_SET2);
        CHECK_AOPARM("TUNE_I3", CPCI_AO_TUNE_I_SET3);
        CHECK_AOPARM("FRONT_BIAS_SET", CPCI_AO_FRONT_BIAS_SET);
        CHECK_AOPARM("FRONT_TUNE_P_SET", CPCI_AO_FRONT_TUNE_P_SET);
        CHECK_AOPARM("FRONT_TUNE_I_SET", CPCI_AO_FRONT_TUNE_I_SET);
        CHECK_AOPARM("FRONT_FIX_TUNING_ANGLE", CPCI_AO_FRONT_FIX_TUNING_ANGLE);
        CHECK_AOPARM("PHASE_I", CPCI_AO_PHASE_I);
	CHECK_AOPARM("PHASE_P", CPCI_AO_PHASE_P);
	CHECK_AOPARM("INITIAL_PHASE", CPCI_AO_INITIAL_PHASE);
        CHECK_AOPARM("FF_DELAY", CPCI_AO_FF_DELAY);
	CHECK_AOPARM("PRETRIG_DELAY", CPCI_AO_PRETRIG_DELAY);
        CHECK_AOPARM("INITIAL_REF_PHASE", CPCI_AO_INITIAL_REF_PHASE);
        CHECK_AOPARM("INT_DELAY", CPCI_AO_INT_DELAY);
        CHECK_AOPARM("CHOPPER_DUTY", CPCI_AO_CHOPPER_DUTY);
        CHECK_AOPARM("RF_HARMONIC", CPCI_AO_RF_HARMONIC);
	CHECK_AOPARM("ALL_FREQUENCY", CPCI_AO_ALL_FREQUENCY);
	CHECK_AOPARM("ALL_BEAM_PHASE", CPCI_AO_ALL_BEAM_PHASE);
	CHECK_AOPARM("ALL_PRETRIG", CPCI_AO_ALL_PRETRIG);
	CHECK_AOPARM("ALL_AMP_COEFFIC", CPCI_AO_ALL_AMP_COEFFIC);
	CHECK_AOPARM("EX_PHASE", CPCI_AO_EX_PHASE);
	CHECK_AOPARM("RBF_DELAY", CPCI_AO_RBF_DELAY);
	CHECK_AOPARM("BPM_DELAY_SET", CPCI_AO_BPM_DELAY_SET);
	CHECK_AOPARM("CHOPPER_PAHSE_SET", CPCI_AO_CHOPPER_PAHSE_SET);
	CHECK_AOPARM("EX_DELAY_SET", CPCI_AO_EX_DELAY_SET);
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
       case CPCI_AO_PRETRIG_DELAY:
	   pao->val=0.0;
	   break;
       case CPCI_AO_INITIAL_REF_PHASE:
	   pao->val=0.0;
	   break;
       case CPCI_AO_INT_DELAY:
	   pao->val=0.0;
	   break;
       case CPCI_AO_CHOPPER_DUTY:
	   pao->val=0.0;
	   break;
       case CPCI_AO_RF_HARMONIC:
	   pao->val=0.0;
	   break;
       case CPCI_AO_ALL_FREQUENCY:
	   pao->val=0.0;
	   break;
       case CPCI_AO_ALL_BEAM_PHASE:
	   pao->val=0.0;
	   break;
       case CPCI_AO_ALL_PRETRIG:
	   pao->val=0.0;
	   break;
       case CPCI_AO_ALL_AMP_COEFFIC:
	   pao->val=0.0;
	   break;
       case CPCI_AO_EX_PHASE:
	   pao->val=0.0;
	   break;
       case CPCI_AO_RBF_DELAY:
	   pao->val=0.0;
	   break;
       case CPCI_AO_BPM_DELAY_SET:
	   pao->val=0.0;
	   break;
       case CPCI_AO_CHOPPER_PAHSE_SET:
	   pao->val=0.0;
	   break;
       case CPCI_AO_EX_DELAY_SET:
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
       case CPCI_AO_PRETRIG_DELAY:
           set_PreTrig_Delay(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_INITIAL_REF_PHASE:
           set_Initial_Ref_Phase(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_INT_DELAY:
           set_Int_Delay(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_CHOPPER_DUTY:
           set_Chopper_Duty(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_RF_HARMONIC:
           set_Rf_Harmonic(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_ALL_FREQUENCY:
           set_All_Frequency(pao->val);
           break;
       case CPCI_AO_ALL_BEAM_PHASE:
           set_All_Beam_Phase(pao->val);
           break;
       case CPCI_AO_ALL_PRETRIG:
           set_All_Pretrig(pao->val);
           break;
       case CPCI_AO_ALL_AMP_COEFFIC:
           set_All_Amp_Coeffic(pao->val);
           break;
       case CPCI_AO_EX_PHASE:
           set_EX_Phase(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_RBF_DELAY:
           set_RBF_Delay(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_BPM_DELAY_SET:
           set_BPM_Delay_Set(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_CHOPPER_PAHSE_SET:
           set_Chopper_Phase_Set(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       case CPCI_AO_EX_DELAY_SET:
           set_EX_Delay_set(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pao,
                    "devAoD212 Write_ao, bad parm");
           return(S_db_badField);
    }

    pao->udf=0;
    return 0;
}

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
        CHECK_WFPARM("WF_4_A", CPCI_WF_4_A);
        CHECK_WFPARM("WF_4_B", CPCI_WF_4_B);
        CHECK_WFPARM("WF_5_A", CPCI_WF_5_A);
        CHECK_WFPARM("WF_5_B", CPCI_WF_5_B);
        CHECK_WFPARM("WF_6_A", CPCI_WF_6_A);
        CHECK_WFPARM("WF_6_B", CPCI_WF_6_B);
        CHECK_WFPARM("WF_7", CPCI_WF_7);
        CHECK_WFPARM("WF_8", CPCI_WF_8);
        CHECK_WFPARM("WF_AMP_SKEW", CPCI_WF_AMP_SKEW);
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
       case CPCI_WF_4_A:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF4_FADDR_A + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_4_B:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF4_FADDR_B + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_5_A:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF5_FADDR_A + 1;
          memcpy(pDest, pSrc, numRead*sizeof(float));
          break;
       case CPCI_WF_5_B:
          pSrc = ((recPrivate*)pwf->dpvt)->pCard->floatBuffer + WF5_FADDR_B + 1;
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
                    "devWfCPCI9110 Read_wf, bad parm");
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
		a[i]=i*21.5/2047-1.5;
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


static long init_wf_wr_1(struct waveformRecord *pwf) {
    /* INP must be PV_LINK, DB_LINK or CA_LINK*/
    switch (pwf->inp.type) {
    case PV_LINK:
    case DB_LINK:
    case CA_LINK:
        break;
    default:
        recGblRecordError(S_db_badField, (void *)pwf,
            "devWfWr (init_record) Illegal INP field");
        return(S_db_badField);
    }

    return 0;
}


static long write_wf_wr_1(struct waveformRecord *pwf) {

    long nRequest = pwf->nelm;
    unsigned int *pDest;
    pDest=pwf->bptr;
    float *pwrBuffer;
    unsigned int *pwdata;
    int i,j;

    for(i=0;i<9;i++){
    pwrBuffer = getCardStruct(i)->wrBuffer1;
    pwdata = getCardStruct(i)->wdata1;

    dbGetLink(&pwf->inp, DBR_FLOAT, getCardStruct(i)->wrBuffer1, 0, &nRequest);
    for(j=0;j<8192;j++)
    {
        /*wdata[i] = (unsigned int)(wrBuffer[i]);*/
        pwdata[j] = pwrBuffer[j];
    }
    }

    semGive(semDMAwr1);

    if (nRequest > 0) {
        pwf->nord = nRequest;
        if (pwf->tsel.type == CONSTANT &&
            pwf->tse == epicsTimeEventDeviceTime)
            dbGetTimeStamp(&pwf->inp, &pwf->time);
    }

    return 0;

}


static long init_wf_wr_2(struct waveformRecord *pwf) {
    /* INP must be PV_LINK, DB_LINK or CA_LINK*/
    switch (pwf->inp.type) {
    case PV_LINK:
    case DB_LINK:
    case CA_LINK:
        break;
    default:
        recGblRecordError(S_db_badField, (void *)pwf,
            "devWfWr (init_record) Illegal INP field");
        return(S_db_badField);
    }

    return 0;
}


static long write_wf_wr_2(struct waveformRecord *pwf) {

    long nRequest = pwf->nelm;
    unsigned int *pDest;
    pDest=pwf->bptr;
    float *pwrBuffer;
    unsigned int *pwdata;
    int i,j;

    for(i=0;i<8;i++){
    pwrBuffer = getCardStruct(i)->wrBuffer2;
    pwdata = getCardStruct(i)->wdata2;

    dbGetLink(&pwf->inp, DBR_FLOAT, getCardStruct(i)->wrBuffer2, 0, &nRequest);
    for(j=0;j<16640;j++)
    {
        /*wdata[i] = (unsigned int)(wrBuffer[i]);*/
        /*pwdata[j] = pwrBuffer[j]+j;*/
        pwdata[j] = pwrBuffer[j];
    }
    }

    semGive(semDMAwr2);

    if (nRequest > 0) {
        pwf->nord = nRequest;
        if (pwf->tsel.type == CONSTANT &&
            pwf->tse == epicsTimeEventDeviceTime)
            dbGetTimeStamp(&pwf->inp, &pwf->time);
    }

    return 0;

}

static long init_wr_rd(struct waveformRecord *pwf) {
    int parmOK = 0;

    if (pwf->inp.type!=VME_IO) {
        recGblRecordError(S_db_badField, (void *)pwf,
                          "devWrRd Init_record, Illegal INP");
        pwf->pact=TRUE;
        return (S_db_badField);
    }

    pwf->dpvt = (void*) calloc(1, sizeof(recPrivate*));

    ((recPrivate*) pwf->dpvt)->pCard = getCardStruct(pwf->inp.value.vmeio.card);

    do {
        CHECK_WFPARM("WR_RD1", CPCI_WR_RD1);
	CHECK_WFPARM("WR_RD2", CPCI_WR_RD2);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pwf,
                      "devWrRd Init_record, bad parm");
        pwf->pact=TRUE;
        return (S_db_badField);
    }

    if (pwf->ftvl != DBF_ULONG) {
        recGblRecordError(S_db_badField, (void *)pwf,
                   "devWrRd (init_record) Illegal FTVL field");
        return(S_db_badField);
    }

    return 0;
}


static long read_wr_rd(struct waveformRecord *pwf) {
    int numRead = pwf->nelm;
    unsigned int *pSrc1;
    unsigned int *pSrc2;
    unsigned int *pDest = pwf->bptr;
    pSrc1 = ((recPrivate*)pwf->dpvt)->pCard->wrRdBuffer1;
    pSrc2 = ((recPrivate*)pwf->dpvt)->pCard->wrRdBuffer2;
    switch (((recPrivate*)pwf->dpvt)->function) {
       case CPCI_WR_RD1:
               memcpy(pDest, pSrc1, numRead*sizeof(unsigned int));
               if(((recPrivate*)pwf->dpvt)->pCard->readDMA1 == 1)
               {
               BRIDGE_REG_WRITE8(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_CSR, 0x05);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->fpgaAddr, 0x20, 0xaaaaaaaa);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_PCI_ADR, (unsigned int) (pSrc1));
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_LOCAL_ADR, 0x324);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_SIZE, 32768);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_DPR, 0x00000008);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_CSR, 0x03);
               ((recPrivate*)pwf->dpvt)->pCard->readDMA1 = 0;
               }
           break;
       case CPCI_WR_RD2:
               memcpy(pDest, pSrc2, numRead*sizeof(unsigned int));
               if(((recPrivate*)pwf->dpvt)->pCard->readDMA2 == 1)
               {
	       BRIDGE_REG_WRITE8(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_CSR, 0x05);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->fpgaAddr, 0x24, 0xaaaaaaaa);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_PCI_ADR, (unsigned int) (pSrc2));
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_LOCAL_ADR, 0x32C);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_SIZE, 66560);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_DPR, 0x00000008);
               BRIDGE_REG_WRITE32(((recPrivate*)pwf->dpvt)->pCard->bridgeAddr, REG_9656_DMA1_CSR, 0x03);
               ((recPrivate*)pwf->dpvt)->pCard->readDMA2 = 0;
               }
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pwf,
                    "devWfCPCI9110 Read_wf, bad parm");
           return(S_db_badField);
    } 

    pwf->nord = numRead;

    return 0;
}


static long wfget_ioint_info(int cmd, struct waveformRecord *pwf,
                           IOSCANPVT *ppvt) {
    *ppvt = ((recPrivate*)pwf->dpvt)->pCard->ioScanPvt;
    return 0;
}
