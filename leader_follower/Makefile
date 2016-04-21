#******************************************************************************

# 
# Verbose make prints
# 
VERBOSE=1

#
# Defines the part type that this project uses.
#
PART=LM3S9B96

#
# The base directory for StellarisWare.
#
ROOT=.

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=./drivers
VPATH+=./utils

#
# Where to find header files that do not live in the source directory.
#
IPATH=.

# The default rule, which causes the EvalBot Autonomous Drive Quickstart Example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/leader_follower.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the EvalBot Autonomous Drive Quickstart Example.
#
${COMPILER}/leader_follower.axf: ${COMPILER}/main.o
${COMPILER}/leader_follower.axf: ${COMPILER}/lfDisplay.o
${COMPILER}/leader_follower.axf: ${COMPILER}/lfMotors.o
${COMPILER}/leader_follower.axf: ${COMPILER}/lfUtility.o
${COMPILER}/leader_follower.axf: ${COMPILER}/display96x16x1.o
${COMPILER}/leader_follower.axf: ${COMPILER}/motor.o
${COMPILER}/leader_follower.axf: ${COMPILER}/sensors.o
${COMPILER}/leader_follower.axf: ${COMPILER}/ustdlib.o
${COMPILER}/leader_follower.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/leader_follower.axf: ${ROOT}/driverlib/${COMPILER}-cm3/libdriver-cm3.a
${COMPILER}/leader_follower.axf: leader_follower.ld
SCATTERgcc_leader_follower=leader_follower.ld
ENTRY_leader_follower=ResetISR
CFLAGSgcc=-DTARGET_IS_TEMPEST_RB1 -DUART_BUFFERED -g -O0
LDFLAGS=-Map=gcc/leader_follower.map

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
