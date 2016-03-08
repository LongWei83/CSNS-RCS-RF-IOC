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

/* function prototypes */
static long init_bi(struct biRecord *pbi);
static long read_bi(struct biRecord *pbi);

static long init_bo(struct boRecord *pbo);
static long write_bo(struct boRecord *pbo);

static long init_ai(struct aiRecord *pai);
static long read_ai(struct aiRecord *pai);

static long init_ao(struct aoRecord *pao);
static long write_ao(struct aoRecord *pao);

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
        CHECK_BIPARM("SWEEP_OPTION",   CPCI_BI_SWEEP_OPTION);
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
       case CPCI_BI_SWEEP_OPTION:
           pbi->val = SweepOption_get(((recPrivate*)pbi->dpvt)->pCard);
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
        CHECK_BOPARM("SWEEP_OPTION",   CPCI_BO_SWEEP_OPTION);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pbo,
                      "devBoD212 Init_record, bad parm");
        pbo->pact=TRUE;
        return (S_db_badField);
    }

    switch(((recPrivate*)pbo->dpvt)->function) {
       case CPCI_BO_INT_ENABLE:
           pbo->val=0;
           break;
       case CPCI_BO_SWEEP_OPTION:
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
       case CPCI_BO_INT_ENABLE:
           if(pbo->val == 0)
               int_Disable(((recPrivate*)pbo->dpvt)->pCard);
           else
               int_Enable(((recPrivate*)pbo->dpvt)->pCard);
           break;
       case CPCI_BO_SWEEP_OPTION:
           if(pbo->val == 0)
               clear_Sweep_Option(((recPrivate*)pbo->dpvt)->pCard);
           else
               set_Sweep_Option(((recPrivate*)pbo->dpvt)->pCard);
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pbo,
                    "devBoD212 Write_bo, bad parm");
           return(S_db_badField);
    }

    pbo->udf=0;
    return 0;
}

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

    ((recPrivate*) pai->dpvt)->pCard = getCardStruct(pai->inp.value.vmeio.card);
    do {
        CHECK_AIPARM("WORK_PERIOD", CPCI_AI_WORK_PERIOD);
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
       case CPCI_AI_WORK_PERIOD:
           pai->val=get_Work_Period(((recPrivate*)pai->dpvt)->pCard);
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
        CHECK_AOPARM("WORK_PERIOD", CPCI_AO_WORK_PERIOD);
    } while(0);

    if (!parmOK) {
        recGblRecordError(S_db_badField, (void *)pao,
                      "devAoD212 Init_record, bad parm");
        pao->pact=TRUE;
        return (S_db_badField);
    }

    switch (((recPrivate*)pao->dpvt)->function) {
       case CPCI_AO_WORK_PERIOD:
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
       case CPCI_AO_WORK_PERIOD:
           set_Work_Period(((recPrivate*)pao->dpvt)->pCard, pao->val);
           break;
       default:
           recGblRecordError(S_db_badField,(void *)pao,
                    "devAoD212 Write_ao, bad parm");
           return(S_db_badField);
    }

    pao->udf=0;
    return 0;
}
