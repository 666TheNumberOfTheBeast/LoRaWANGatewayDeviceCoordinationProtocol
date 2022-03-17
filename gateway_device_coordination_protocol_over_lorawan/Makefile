#
# OMNeT++/OMNEST Makefile for gateway_device_coordination_protocol_over_lorawan
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -KFLORA_PROJ=../flora-1.0.0/flora -KINET4_3_PROJ=../inet4.3 -DINET_IMPORT -I. -I$$\(FLORA_PROJ\)/src -I$$\(INET4_3_PROJ\)/src -L/usr/lib/x86_64-linux-gnu/ -L$$\(FLORA_PROJ\)/src -L$$\(INET4_3_PROJ\)/src -lssl -lcrypto -lflora$$\(D\) -lINET$$\(D\)
#

# Name of target to be created (-o option)
TARGET_DIR = .
TARGET_NAME = gateway_device_coordination_protocol_over_lorawan$(D)
TARGET = $(TARGET_NAME)$(EXE_SUFFIX)
TARGET_IMPLIB = $(TARGET_NAME)$(IMPLIB_SUFFIX)
TARGET_IMPDEF = $(TARGET_NAME)$(IMPDEF_SUFFIX)
TARGET_FILES = $(TARGET_DIR)/$(TARGET)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I. -I$(FLORA_PROJ)/src -I$(INET4_3_PROJ)/src

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)/usr/lib/x86_64-linux-gnu/ $(LDFLAG_LIBPATH)$(FLORA_PROJ)/src $(LDFLAG_LIBPATH)$(INET4_3_PROJ)/src  -lssl -lcrypto -lflora$(D) -lINET$(D)

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/src/LoRaDevices/LoRaEndDevice.o \
    $O/src/LoRaDevices/LoRaGateway.o \
    $O/src/LoRaDevices/LoRaNetworkServer.o \
    $O/src/LoRaDevices/Utils.o \
    $O/src/IPPackets/IPv4Packet_m.o \
    $O/src/LoRaFrames/LoRaAppDownlinkFrame_m.o \
    $O/src/LoRaFrames/LoRaAppUplinkFrame_m.o \
    $O/src/LoRaFrames/LoRaDatalinkFrame_m.o \
    $O/src/LoRaFrames/LoRaJoinAcceptFrame_m.o \
    $O/src/LoRaFrames/LoRaJoinRequestFrame_m.o \
    $O/src/LoRaFrames/LoRaPhysicalFrame_m.o \
    $O/src/TransportSegments/TCPSegment_m.o \
    $O/src/TransportSegments/UDPSegment_m.o

# Message files
MSGFILES = \
    src/IPPackets/IPv4Packet.msg \
    src/LoRaFrames/LoRaAppDownlinkFrame.msg \
    src/LoRaFrames/LoRaAppUplinkFrame.msg \
    src/LoRaFrames/LoRaDatalinkFrame.msg \
    src/LoRaFrames/LoRaJoinAcceptFrame.msg \
    src/LoRaFrames/LoRaJoinRequestFrame.msg \
    src/LoRaFrames/LoRaPhysicalFrame.msg \
    src/TransportSegments/TCPSegment.msg \
    src/TransportSegments/UDPSegment.msg

# SM files
SMFILES =

# Other makefile variables (-K)
FLORA_PROJ=../flora-1.0.0/flora
INET4_3_PROJ=../inet4.3

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(PLATFORM),win32.x86_64)
LIBS += -Wl,-rpath,$(abspath /usr/lib/x86_64-linux-gnu/) -Wl,-rpath,$(abspath $(FLORA_PROJ)/src) -Wl,-rpath,$(abspath $(INET4_3_PROJ)/src)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE (if COPTS has changed since last build)
# and make the object files depend on it
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
  $(shell $(MKPATH) "$O")
  $(file >$(COPTS_FILE),$(COPTS))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
-include makefrag

#------------------------------------------------------------------------------

# Main target
all: $(TARGET_FILES)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clang-msabi)
	-$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib) 2>/dev/null
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_FILES)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(CLEANALL_COMMAND)
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

help:
	@echo "$$HELP_SYNOPSYS"
	@echo "$$HELP_TARGETS"
	@echo "$$HELP_VARIABLES"
	@echo "$$HELP_EXAMPLES"

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)
