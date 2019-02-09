#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/dispatch.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/net/comm.o \
	${OBJECTDIR}/src/net/queue.o \
	${OBJECTDIR}/src/net/security.o \
	${OBJECTDIR}/src/net/transport.o \
	${OBJECTDIR}/src/server.o


# C Compiler Flags
CFLAGS=--pedantic

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/z1prot_sim

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/z1prot_sim: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/z1prot_sim ${OBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${OBJECTDIR}/src/dispatch.o: src/dispatch.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dispatch.o src/dispatch.c

${OBJECTDIR}/src/main.o: src/main.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.c

${OBJECTDIR}/src/net/comm.o: src/net/comm.c
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/comm.o src/net/comm.c

${OBJECTDIR}/src/net/queue.o: src/net/queue.c
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/queue.o src/net/queue.c

${OBJECTDIR}/src/net/security.o: src/net/security.c
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/security.o src/net/security.c

${OBJECTDIR}/src/net/transport.o: src/net/transport.c
	${MKDIR} -p ${OBJECTDIR}/src/net
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net/transport.o src/net/transport.c

${OBJECTDIR}/src/server.o: src/server.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/server.o src/server.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
