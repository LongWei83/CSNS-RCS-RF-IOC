#include <stdio.h>
#include <math.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <dbAccess.h>

int mySubDebug;

static long mySubInit(subRecord *precord)
{
    if (mySubDebug)
        printf("Record %s called mySubInit(%p)\n",
               precord->name, (void*) precord);
    return 0;
}

static long mySubProcess(subRecord *precord)
{
    if (mySubDebug)
        printf("Record %s called mySubProcess(%p)\n",
               precord->name, (void*) precord);
    return 0;
}

static long myAsubInit(aSubRecord *precord)
{
    if (mySubDebug)
        printf("Record %s called myAsubInit(%p)\n",
               precord->name, (void*) precord);
    return 0;
}

static long myAsubProcess(aSubRecord *precord)
{
    long i;
    unsigned long *a;
    double *b;
    unsigned long *c;
    unsigned long *d;

    unsigned long noa;
    unsigned long nob;
    unsigned long noc;

    double *aDest;
    double *bDest;
    double *cDest;
    
    a = (unsigned long *)precord->a;
    b = (double *)precord->b;
    c = (unsigned long *)precord->c;

    aDest = (double *)precord->vala;
    bDest = (double *)precord->valb;
    cDest = (double *)precord->valc;

    noa = precord->noa;
    nob = precord->nob;
    noc = precord->noc;

    dbGetLink(&precord->inpa, DBR_ULONG, precord->a, 0, &noa);
    dbGetLink(&precord->inpb, DBR_DOUBLE, precord->b, 0, &nob);

    for(i=0;i<precord->nova;i++)
    {
        if((c[0] > 0)&&(c[0] <= a[0]))
        {
            aDest[i] = sqrt(bDest[i]*bDest[i]*(c[0]-1)/c[0] + b[i]*b[i]/c[0]);
            if(c[0] == a[0])
                cDest[i] = aDest[i];
        }
        else
        {
            aDest[i] = 0;
            bDest[i] = 0;
        }
    }

    for(i=0;i<precord->noc;i++){
        if(c[i] < a[i])
        {
            c[i]++;
        }
        else
        {
            c[i] = 0;
        }
    }

    memcpy(bDest, aDest, nob);
    
    return 0;
}

static long offSet(aSubRecord *precord)
{
    long i;
    double *a;
    double *b;

    unsigned long noa;
    unsigned long nob;

    double *aDest;

    a = (double *)precord->a;
    b = (double *)precord->b;

    aDest = (double *)precord->vala;

    noa = precord->noa;
    nob = precord->nob;

    dbGetLink(&precord->inpa, DBR_DOUBLE, precord->a, 0, &noa);
    dbGetLink(&precord->inpb, DBR_DOUBLE, precord->b, 0, &nob);

    for(i=0;i<precord->nova;i++)
    {
        aDest[i] = a[i] + b[0];
    }
    
    return 0;
}

static long maxHold(aSubRecord *precord)
{
    long i;
    double *a;
    double *b;

    unsigned long noa;
    unsigned long nob;

    double *aDest;

    a = (double *)precord->a;
    b = (double *)precord->b;

    aDest = (double *)precord->vala;

    noa = precord->noa;
    nob = precord->nob;

    dbGetLink(&precord->inpa, DBR_DOUBLE, precord->a, 0, &noa);
    dbGetLink(&precord->inpb, DBR_DOUBLE, precord->b, 0, &nob);

    for(i=0;i<precord->nova;i++)
    {
	if(b[0] == 0)
	{
		aDest[i] = 0;
	}
	else
	{
		if(a[i] * a[i] > aDest[i] * aDest[i])
        	aDest[i] = a[i];
	}
    }
    
    return 0;
}

/* Register these symbols for use by IOC code: */

epicsExportAddress(int, mySubDebug);
epicsRegisterFunction(mySubInit);
epicsRegisterFunction(mySubProcess);
epicsRegisterFunction(myAsubInit);
epicsRegisterFunction(myAsubProcess);
epicsRegisterFunction(offSet);
epicsRegisterFunction(maxHold);
