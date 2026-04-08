/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************//*!
 *
 * @file mds_config.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file defines Ieee11073 Agent object configuration. 
 *
 *****************************************************************************/
#ifndef _MDS_CONFIG_H
#define _MDS_CONFIG_H

/******************************************************************************
 * MACROS
 *****************************************************************************/
/* Enable Multi Person Support */
#define MULTI_PERSON_SUPPORT

//#define MDS_ATTR_SYS_TYPE
#define MDS_ATTR_SYS_TYPE_SPEC_LIST
#define MDS_PROD_SPEC
#define MDS_SETTABLE_TIME

#ifdef MDS_SETTABLE_TIME
    #define MDS_RELATIVE_TIME
    #define MDS_HIRES_TIME
    #define MDS_RTC
    #define MDS_SET_CLOCK
#endif

#define MDS_POWER_STATUS
#define MDS_BATT_CHARGE
#define MDS_BATT_REMAIN
#define MDS_CONFIRM_TIMEOUT
#define MDS_ABSOLUTE_TIME
#define MDS_ADJUST_ABSOLUTE_TIME

/* Numeric Defines */
//#define NUMERIC_SIMPLE_NU_OBS_VAL
//#define NUMERIC_COMP_SIMPLE_NUM_OBS_VAL
//#define NUMERIC_BASIC_NU_OBS_VAL
//#define NUMERIC_COMP_BASIC_NU_OBS_VAL
//#define NUMERIC_NU_OBS_VAL

/* RTSA defines */
#define RTSA_SCALE_RANGE_SPEC8
//#define RTSA_SCALE_RANGE_SPEC16

/* Enumeration defines */
#define ENUM_OBSRVD_VAL_SIMP_OID
//#define ENUM_OBSRVD_VAL_SIMP_BIT_STR
//#define ENUM_OBSRVD_VAL_BASIC_BIT_STR
//#define ENUM_OBSRVD_VAL_SIMP_STR
//#define ENUM_OBSRVD_VAL
#define ENUM_OBS_VAL_PARTITION

#define MDC_FIXED_DATA_FORMAT

#define MDS_RTSA
#define MDS_ENUMERATION
#define MDS_NUMERIC
#define MDS_EPISOIDIC_SCANNER
#define MDS_PERIODIC_SCANNER
#define MDS_PMSTORE
#if (defined(MDS_PERIODIC_SCANNER)||defined(MDS_EPISOIDIC_SCANNER))
#define MDS_CFGSCANNER
#define MDS_SCANNER
#endif

#if (defined(MDS_RTSA)||defined(MDS_ENUMERATION)||defined(MDS_NUMERIC))
#define MDS_METRIC
#endif

#define MAX_MANAGER_DATA_COUNT  0
#define MAX_AGENT_DATA_COUNT    1
/* SYSTEM ID        | LENGTH (max 8) |             ID                           | */
#define SYSTEM_ID        0x00, 0x08,    0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8

#endif /* _MDS_CONFIG_H */