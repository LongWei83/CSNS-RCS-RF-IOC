## Example vxWorks startup file

## The following is needed if your board support package doesn't at boot time
## automatically cd to the directory containing its startup script
cd "/home/RCS/EpicsBak/rcsRfIoc"

ld <bin/vxWorks-mpc8572/rcsRfIoc.munch

epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES","1640800")

dbLoadDatabase "dbd/rcsRfIoc.dbd"
rcsRfIoc_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords "db/rcsRfGl.db"
dbLoadRecords "db/rcsRf1.db"
dbLoadRecords "db/rcsRf2.db"

#D212Config(cardNum,index)
D212Config(0,0)
D212Config(1,1)

iocInit
