TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#=============================
# Build the IOC support library

INCLUDES += -I/home/RCS/workbench/vxworks-6.8/target/config/cpci6200

ifneq ($(findstring Host, $(VALID_BUILDS)),)
    VALID_BUILDS = Host
endif

LIBRARY_IOC += rcsRfIocSupport

# xxxRecord.h will be created from xxxRecord.dbd
# Install devXxxSoft.dbd into <top>/dbd
DBD += devD212.dbd

# Compile and add the code to the support library
rcsRfIocSupport_SRCS += devD212.c
rcsRfIocSupport_SRCS += drvD212.c
rcsRfIocSupport_SRCS += dbSubExample.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application.

rcsRfIocSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# Build the IOC application

PROD_IOC = rcsRfIoc
# rcsRfIoc.dbd will be created and installed
DBD += rcsRfIoc.dbd

# rcsRfIoc.dbd will be made up from these files:
rcsRfIoc_DBD += base.dbd
rcsRfIoc_DBD += devD212.dbd
rcsRfIoc_DBD += dbSubExample.dbd

# rcsRfIoc_registerRecordDeviceDriver.cpp derives from rcsRfIoc.dbd
rcsRfIoc_SRCS += rcsRfIoc_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
rcsRfIoc_SRCS_DEFAULT += rcsRfIocMain.cpp
rcsRfIoc_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#rcsRfIoc_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link in the code from the support library
rcsRfIoc_LIBS += rcsRfIocSupport

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

# Finally link to the EPICS Base libraries
rcsRfIoc_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

