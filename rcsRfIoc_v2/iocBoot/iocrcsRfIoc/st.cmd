## Example vxWorks startup file

## The following is needed if your board support package doesn't at boot time
## automatically cd to the directory containing its startup script
cd "/home/long/Epics/apps/rcsRfIoc_v2"

ld <bin/vxWorks-mpc8572/rcsRfIoc.munch

epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES","13107200")

dbLoadDatabase "dbd/rcsRfIoc.dbd"
rcsRfIoc_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords "db/rcsRfGl.db"
dbLoadRecords "db/rcsRf1.db"
dbLoadRecords "db/rcsRf2.db"
dbLoadRecords "db/rcsRf3.db"
dbLoadRecords "db/rcsRf4.db"
dbLoadRecords "db/rcsRf5.db"
dbLoadRecords "db/rcsRf6.db"
dbLoadRecords "db/rcsRf7.db"
dbLoadRecords "db/rcsRf8.db"
dbLoadRecords "db/rcsRf9.db"

#D212Config(cardNum,index)
D212Config(0,0)
D212Config(1,1)
D212Config(2,2)
D212Config(3,3)
D212Config(4,4)
D212Config(5,5)
D212Config(6,6)
D212Config(7,7)
D212Config(8,8)

iocInit

dbpf "rcsRf1:Int_Delay_set","0"
dbpf "rcsRf2:Int_Delay_set","1.5"
dbpf "rcsRf3:Int_Delay_set","3"
dbpf "rcsRf4:Int_Delay_set","4.5"
dbpf "rcsRf5:Int_Delay_set","6"
dbpf "rcsRf6:Int_Delay_set","7.5"
dbpf "rcsRf7:Int_Delay_set","9"
dbpf "rcsRf8:Int_Delay_set","10.5"
dbpf "rcsRf9:Int_Delay_set","12"

dbpf "rcsRf1:workPeriod_set","40"
dbpf "rcsRf2:workPeriod_set","40"
dbpf "rcsRf3:workPeriod_set","40"
dbpf "rcsRf4:workPeriod_set","40"
dbpf "rcsRf5:workPeriod_set","40"
dbpf "rcsRf6:workPeriod_set","40"
dbpf "rcsRf7:workPeriod_set","40"
dbpf "rcsRf8:workPeriod_set","40"
dbpf "rcsRf9:workPeriod_set","40"

dbpf "rcsRf1:Initial_Ref_Phase_set","0"
dbpf "rcsRf2:Initial_Ref_Phase_set","112"
dbpf "rcsRf3:Initial_Ref_Phase_set","75"
dbpf "rcsRf4:Initial_Ref_Phase_set","108"
dbpf "rcsRf5:Initial_Ref_Phase_set","112"
dbpf "rcsRf6:Initial_Ref_Phase_set","104"
dbpf "rcsRf7:Initial_Ref_Phase_set","156"
dbpf "rcsRf8:Initial_Ref_Phase_set","288"



