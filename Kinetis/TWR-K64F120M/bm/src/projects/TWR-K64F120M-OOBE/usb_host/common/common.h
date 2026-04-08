/*
 * Copyright (c) 2010 Freescale Semiconductor;
 * All Rights Reserved
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
 */
/*
 * File:	common.h
 * Purpose:	File to be included by all project files
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/* Debug prints ON (#define) or OFF (#undef) */
#define DEBUG
#define DEBUG_PRINT

/* Include the generic CPU header file */
#include "arm_cm4.h"
#include "Settings.h"

/* Include the platform specific header file */
#if (defined(TOWER))
#include "tower.h"
#else
#error "No valid platform defined"
#endif

/* Include the cpu specific header file */

#if(defined(MCU_MK64FN1M0VCM12))
#include "MK63F12.h"
#else
#error "No valid CPU defined"
#endif

/* Include any toolchain specfic header files */
#if (defined(__MWERKS__))
#include "mwerks.h"
#elif (defined(__DCC__))
#include "build/wrs/diab.h"
#elif (defined(__ghs__))
#include "build/ghs/ghs.h"
#elif (defined(__GNUC__))
#include "build/gnu/gnu.h"
#elif (defined(IAR))
#include "iar.h"
#else
#warning "No toolchain specific header included"
#endif

/* Include common utilities */

#include <stdio.h>
#include "assert.h"
#include "startup.h"

#if (defined(IAR))
#include "intrinsics.h"
#else
#endif

#endif		/* _COMMON_H_ */
