/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
#pragma config FUSES_USERCFG1_FSEQ0_SEQNUM = 0x1U
#pragma config FUSES_USERCFG1_FSEQ0_SEQBAR = 0xfffeU
#pragma config FUSES_USERCFG1_FSEQ1_SEQNUM = 0x1U
#pragma config FUSES_USERCFG1_FSEQ1_SEQBAR = 0xfffeU
#pragma config FUSES_USERCFG1_FSEQ2_SEQNUM = 0x1U
#pragma config FUSES_USERCFG1_FSEQ2_SEQBAR = 0xfffeU
#pragma config FUSES_USERCFG1_FSEQ3_SEQNUM = 0x1U
#pragma config FUSES_USERCFG1_FSEQ3_SEQBAR = 0xfffeU
#pragma config FUSES_USERCFG1_FSEQ4_SEQNUM = 0x1U
#pragma config FUSES_USERCFG1_FSEQ4_SEQBAR = 0xfffeU
#pragma config FUSES_USERCFG1_FSEQ5_SEQNUM = 0x1U
#pragma config FUSES_USERCFG1_FSEQ5_SEQBAR = 0xfffeU
#pragma config FUSES_USERCFG1_FSEQ6_SEQNUM = 0x1U
#pragma config FUSES_USERCFG1_FSEQ6_SEQBAR = 0xfffeU
#pragma config FUSES_USERCFG1_FSEQ7_SEQNUM = 0x1U
#pragma config FUSES_USERCFG1_FSEQ7_SEQBAR = 0xfffeU
#pragma config FUSES_USERCFG1_AFSEQ0_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ0_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ1_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ1_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ2_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ2_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ3_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ3_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ4_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ4_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ5_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ5_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ6_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ6_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ7_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG1_AFSEQ7_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG1_FUCFG0_WDT_ENABLE = CLEAR
#pragma config FUSES_USERCFG1_FUCFG0_WDT_WEN = CLEAR
#pragma config FUSES_USERCFG1_FUCFG0_WDT_RUNSTDBY = CLEAR
#pragma config FUSES_USERCFG1_FUCFG0_WDT_ALWAYSON = CLEAR
#pragma config FUSES_USERCFG1_FUCFG0_WDT_PER = 0x0U
#pragma config FUSES_USERCFG1_FUCFG0_WDT_WINDOW = 0x0U
#pragma config FUSES_USERCFG1_FUCFG0_WDT_EWOFFSET = 0x0U
#pragma config FUSES_USERCFG1_FUCFG1_FUCFG1 = 0xffffffffU
#pragma config FUSES_USERCFG1_FUCFG2_BRCFGUDSSMOR_BOR_HYST = SET
#pragma config FUSES_USERCFG1_FUCFG2_BRCFGUDSSMOR_BOR_TRIP = 0x3U
#pragma config FUSES_USERCFG1_FUCFG3_HYST_BOR_VDDIO = SET
#pragma config FUSES_USERCFG1_FUCFG3_BOR_TRIP_VDDIO = 0x3U
#pragma config FUSES_USERCFG1_FUCFG3_HYST_BOR_VDDIOB = SET
#pragma config FUSES_USERCFG1_FUCFG3_BOR_TRIP_VDDIOB = 0x3U
#pragma config FUSES_USERCFG1_FUCFG3_HYST_BOR_VDDA = SET
#pragma config FUSES_USERCFG1_FUCFG3_BOR_TRIP_VDDA = 0x3U
#pragma config FUSES_USERCFG1_FUCFG3_HYST_BOR_VDDREG = SET
#pragma config FUSES_USERCFG1_FUCFG5_UCP0 = 0xfU
#pragma config FUSES_USERCFG1_FUCFG5_UCP1 = 0xfU
#pragma config FUSES_USERCFG1_FUCFG6_PFM_ECCCTL = 0x3U
#pragma config FUSES_USERCFG1_FUCFG6_PFM_ECCUNLCK = SET
#pragma config FUSES_USERCFG1_FUCFG6_PFM_SECCNT = 0x0U
#pragma config FUSES_USERCFG1_FUCFG7_PFM_TEMP = SET
#pragma config FUSES_USERCFG1_FUCFG8_PFM_UC1WP = CLEAR
#pragma config FUSES_USERCFG1_FUCFG8_PFM_UC2WP = CLEAR
#pragma config FUSES_USERCFG1_FUCFG8_PFM_UC1WPLOCK = CLEAR
#pragma config FUSES_USERCFG1_FUCFG8_PFM_UC2WPLOCK = CLEAR
#pragma config FUSES_USERCFG1_FUCFG9_FUCFG9 = 0x202U
#pragma config FUSES_BOOTCFG1_BLDRCFG_PFM_BCRP = CLEAR
#pragma config FUSES_BOOTCFG1_BLDRCFG_PFM_BCWP = CLEAR
#pragma config FUSES_BOOTCFG1_BLDRCFG_BROM_EN_BFMCHK = NONE
#pragma config FUSES_BOOTCFG1_BLDRCFG_BROM_EN_PLL = CLEAR
#pragma config FUSES_BOOTCFG1_BLDRCFG_BROM_EN_DALUN = SET
#pragma config FUSES_BOOTCFG1_BLDRCFG_BROM_EN_RWLOCKS = SET
#pragma config FUSES_BOOTCFG1_BROM_BSEQ_SEQNUM = 0x1U
#pragma config FUSES_BOOTCFG1_BROM_BSEQ_SEQBAR = 0xfffeU
#pragma config FUSES_BOOTCFG1_BFM_CHK_TABLEPTR_BFM_CHK_TABLEPTR = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_NS0_KEYVAL_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_NS1_KEYVAL_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_NS2_KEYVAL_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_NS3_KEYVAL_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_NS0_CELOCK_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_NS1_CELOCK_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_NS2_CELOCK_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_NS3_CELOCK_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_S0_KEYVAL_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_S1_KEYVAL_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_S2_KEYVAL_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_S3_KEYVAL_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_S0_CELOCK_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_S1_CELOCK_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_S2_CELOCK_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_S3_CELOCK_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_ALL0_KEYVAL_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_ALL1_KEYVAL_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_ALL2_KEYVAL_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ1_CE_ALL3_KEYVAL_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_ALL0_CELOCK_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_ALL1_CELOCK_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_ALL2_CELOCK_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ1_CE_ALL3_CELOCK_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ0_CE_ALL0_KEYVAL_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ0_CE_ALL1_KEYVAL_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ0_CE_ALL2_KEYVAL_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_TZ0_CE_ALL3_KEYVAL_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ0_CE_ALL0_CELOCK_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ0_CE_ALL1_CELOCK_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ0_CE_ALL2_CELOCK_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_CELOCK_TZ0_CE_ALL3_CELOCK_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_CRCCMD0_KEYVAL_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_CRCCMD1_KEYVAL_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_CRCCMD2_KEYVAL_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_CRCCMD3_KEYVAL_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYCONFIG_CRCCMD0_KEYCONFIG_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYCONFIG_CRCCMD1_KEYCONFIG_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYCONFIG_CRCCMD2_KEYCONFIG_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYCONFIG_CRCCMD3_KEYCONFIG_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_HOSTDALELEV0_KEYVAL_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_HOSTDALELEV1_KEYVAL_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_HOSTDALELEV2_KEYVAL_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYVAL_HOSTDALELEV3_KEYVAL_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYCONFIG_HOSTDALELEV0_KEYCONFIG_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYCONFIG_HOSTDALELEV1_KEYCONFIG_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYCONFIG_HOSTDALELEV2_KEYCONFIG_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG1_KEYCONFIG_HOSTDALELEV3_KEYCONFIG_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG1_ROM_CTRLA_ROM_CTRLA = 0x706U
#pragma config FUSES_BOOTCFG1_FCR_CTRLA_FCR_CTRLA = 0xfcf04U
#pragma config FUSES_BOOTCFG1_RPMU_VREGCTRL_RPMU_VREGCTRL = 0x707f736U
#pragma config FUSES_BOOTCFG1_PLL0_CTRL_PLL0_CTRL = 0x3a86U
#pragma config FUSES_BOOTCFG1_PLL0_FBDIV_PLL0_FBDIV = 0x3ffU
#pragma config FUSES_BOOTCFG1_PLL0_REFDIV_PLL0_REFDIV = 0x3fU
#pragma config FUSES_BOOTCFG1_PLL0_POSTDIVA_PLL0_POSTDIVA = 0xbfU
#pragma config FUSES_BOOTCFG1_MCLK_CLKDIV1_MCLK_CLKDIV1 = 0xffffffffU
#pragma config FUSES_BOOTCFG1_GCLK_GENCTRL0_GCLK_GENCTRL0 = 0xffffffffU
#pragma config FUSES_BOOTCFG1_BROM_BOOTCFGCRC0_BROM_BOOTCFGCRC = 0xffffffffU
#pragma config FUSES_BOOTCFG1_BROM_BOOTCFGCRC1_BROM_BOOTCFGCRC = 0xffffffffU
#pragma config FUSES_BOOTCFG1_BROM_BOOTCFGCRC2_BROM_BOOTCFGCRC = 0xffffffffU
#pragma config FUSES_BOOTCFG1_BROM_BOOTCFGCRC3_BROM_BOOTCFGCRC = 0xffffffffU
#pragma config FUSES_BOOTCFG1_BROM_PAGEEND_BROM_PAGEEND = 0xffffffffU
#pragma config FUSES_USERCFG2_FSEQ0_SEQNUM = 0x0U
#pragma config FUSES_USERCFG2_FSEQ0_SEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_FSEQ1_SEQNUM = 0x0U
#pragma config FUSES_USERCFG2_FSEQ1_SEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_FSEQ2_SEQNUM = 0x0U
#pragma config FUSES_USERCFG2_FSEQ2_SEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_FSEQ3_SEQNUM = 0x0U
#pragma config FUSES_USERCFG2_FSEQ3_SEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_FSEQ4_SEQNUM = 0x0U
#pragma config FUSES_USERCFG2_FSEQ4_SEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_FSEQ5_SEQNUM = 0x0U
#pragma config FUSES_USERCFG2_FSEQ5_SEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_FSEQ6_SEQNUM = 0x0U
#pragma config FUSES_USERCFG2_FSEQ6_SEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_FSEQ7_SEQNUM = 0x0U
#pragma config FUSES_USERCFG2_FSEQ7_SEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ0_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ0_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ1_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ1_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ2_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ2_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ3_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ3_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ4_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ4_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ5_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ5_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ6_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ6_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ7_ASEQNUM = 0xffffU
#pragma config FUSES_USERCFG2_AFSEQ7_ASEQBAR = 0xffffU
#pragma config FUSES_USERCFG2_FUCFG0_WDT_ENABLE = CLEAR
#pragma config FUSES_USERCFG2_FUCFG0_WDT_WEN = SET
#pragma config FUSES_USERCFG2_FUCFG0_WDT_RUNSTDBY = SET
#pragma config FUSES_USERCFG2_FUCFG0_WDT_ALWAYSON = CLEAR
#pragma config FUSES_USERCFG2_FUCFG0_WDT_PER = 0x0U
#pragma config FUSES_USERCFG2_FUCFG0_WDT_WINDOW = 0x0U
#pragma config FUSES_USERCFG2_FUCFG0_WDT_EWOFFSET = 0x0U
#pragma config FUSES_USERCFG2_FUCFG1_FUCFG1 = 0xffffffffU
#pragma config FUSES_USERCFG2_FUCFG2_BRCFGUDSSMOR_BOR_HYST = SET
#pragma config FUSES_USERCFG2_FUCFG2_BRCFGUDSSMOR_BOR_TRIP = 0x3U
#pragma config FUSES_USERCFG2_FUCFG3_HYST_BOR_VDDIO = SET
#pragma config FUSES_USERCFG2_FUCFG3_BOR_TRIP_VDDIO = 0x3U
#pragma config FUSES_USERCFG2_FUCFG3_HYST_BOR_VDDIOB = SET
#pragma config FUSES_USERCFG2_FUCFG3_BOR_TRIP_VDDIOB = 0x3U
#pragma config FUSES_USERCFG2_FUCFG3_HYST_BOR_VDDA = SET
#pragma config FUSES_USERCFG2_FUCFG3_BOR_TRIP_VDDA = 0x3U
#pragma config FUSES_USERCFG2_FUCFG3_HYST_BOR_VDDREG = SET
#pragma config FUSES_USERCFG2_FUCFG5_UCP0 = 0xfU
#pragma config FUSES_USERCFG2_FUCFG5_UCP1 = 0xfU
#pragma config FUSES_USERCFG2_FUCFG6_PFM_ECCCTL = 0x3U
#pragma config FUSES_USERCFG2_FUCFG6_PFM_ECCUNLCK = SET
#pragma config FUSES_USERCFG2_FUCFG6_PFM_SECCNT = 0x0U
#pragma config FUSES_USERCFG2_FUCFG7_PFM_TEMP = SET
#pragma config FUSES_USERCFG2_FUCFG8_PFM_UC1WP = CLEAR
#pragma config FUSES_USERCFG2_FUCFG8_PFM_UC2WP = CLEAR
#pragma config FUSES_USERCFG2_FUCFG8_PFM_UC1WPLOCK = CLEAR
#pragma config FUSES_USERCFG2_FUCFG8_PFM_UC2WPLOCK = CLEAR
#pragma config FUSES_USERCFG2_FUCFG9_FUCFG9 = 0x202U
#pragma config FUSES_BOOTCFG2_BLDRCFG_PFM_BCRP = CLEAR
#pragma config FUSES_BOOTCFG2_BLDRCFG_PFM_BCWP = CLEAR
#pragma config FUSES_BOOTCFG2_BLDRCFG_BROM_EN_BFMCHK = NONE
#pragma config FUSES_BOOTCFG2_BLDRCFG_BROM_EN_PLL = CLEAR
#pragma config FUSES_BOOTCFG2_BLDRCFG_BROM_EN_DALUN = SET
#pragma config FUSES_BOOTCFG2_BLDRCFG_BROM_EN_RWLOCKS = SET
#pragma config FUSES_BOOTCFG2_BROM_BSEQ_SEQNUM = 0x0U
#pragma config FUSES_BOOTCFG2_BROM_BSEQ_SEQBAR = 0xffffU
#pragma config FUSES_BOOTCFG2_BFM_CHK_TABLEPTR_BFM_CHK_TABLEPTR = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_NS0_KEYVAL_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_NS1_KEYVAL_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_NS2_KEYVAL_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_NS3_KEYVAL_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_NS0_CELOCK_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_NS1_CELOCK_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_NS2_CELOCK_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_NS3_CELOCK_TZ1_CE_NS = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_S0_KEYVAL_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_S1_KEYVAL_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_S2_KEYVAL_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_S3_KEYVAL_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_S0_CELOCK_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_S1_CELOCK_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_S2_CELOCK_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_S3_CELOCK_TZ1_CE_S = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_ALL0_KEYVAL_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_ALL1_KEYVAL_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_ALL2_KEYVAL_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ1_CE_ALL3_KEYVAL_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_ALL0_CELOCK_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_ALL1_CELOCK_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_ALL2_CELOCK_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ1_CE_ALL3_CELOCK_TZ1_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ0_CE_ALL0_KEYVAL_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ0_CE_ALL1_KEYVAL_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ0_CE_ALL2_KEYVAL_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_TZ0_CE_ALL3_KEYVAL_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ0_CE_ALL0_CELOCK_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ0_CE_ALL1_CELOCK_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ0_CE_ALL2_CELOCK_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_CELOCK_TZ0_CE_ALL3_CELOCK_TZ0_CE_ALL = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_CRCCMD0_KEYVAL_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_CRCCMD1_KEYVAL_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_CRCCMD2_KEYVAL_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_CRCCMD3_KEYVAL_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYCONFIG_CRCCMD0_KEYCONFIG_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYCONFIG_CRCCMD1_KEYCONFIG_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYCONFIG_CRCCMD2_KEYCONFIG_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYCONFIG_CRCCMD3_KEYCONFIG_CRCCMD = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_HOSTDALELEV0_KEYVAL_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_HOSTDALELEV1_KEYVAL_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_HOSTDALELEV2_KEYVAL_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYVAL_HOSTDALELEV3_KEYVAL_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYCONFIG_HOSTDALELEV0_KEYCONFIG_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYCONFIG_HOSTDALELEV1_KEYCONFIG_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYCONFIG_HOSTDALELEV2_KEYCONFIG_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG2_KEYCONFIG_HOSTDALELEV3_KEYCONFIG_HOSTDALELEV = 0xffffffffU
#pragma config FUSES_BOOTCFG2_ROM_CTRLA_ROM_CTRLA = 0x706U
#pragma config FUSES_BOOTCFG2_FCR_CTRLA_FCR_CTRLA = 0xfcf04U
#pragma config FUSES_BOOTCFG2_RPMU_VREGCTRL_RPMU_VREGCTRL = 0x707f736U
#pragma config FUSES_BOOTCFG2_PLL0_CTRL_PLL0_CTRL = 0x3a86U
#pragma config FUSES_BOOTCFG2_PLL0_FBDIV_PLL0_FBDIV = 0x3ffU
#pragma config FUSES_BOOTCFG2_PLL0_REFDIV_PLL0_REFDIV = 0x3fU
#pragma config FUSES_BOOTCFG2_PLL0_POSTDIVA_PLL0_POSTDIVA = 0xbfU
#pragma config FUSES_BOOTCFG2_MCLK_CLKDIV1_MCLK_CLKDIV1 = 0xffffffffU
#pragma config FUSES_BOOTCFG2_GCLK_GENCTRL0_GCLK_GENCTRL0 = 0xffffffffU
#pragma config FUSES_BOOTCFG2_BROM_BOOTCFGCRC0_BROM_BOOTCFGCRC = 0xffffffffU
#pragma config FUSES_BOOTCFG2_BROM_BOOTCFGCRC1_BROM_BOOTCFGCRC = 0xffffffffU
#pragma config FUSES_BOOTCFG2_BROM_BOOTCFGCRC2_BROM_BOOTCFGCRC = 0xffffffffU
#pragma config FUSES_BOOTCFG2_BROM_BOOTCFGCRC3_BROM_BOOTCFGCRC = 0xffffffffU
#pragma config FUSES_BOOTCFG2_BROM_PAGEEND_BROM_PAGEEND = 0xffffffffU




// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
/* Following MISRA-C rules are deviated in the below code block */
/* MISRA C-2012 Rule 7.2 - Deviation record ID - H3_MISRAC_2012_R_7_2_DR_1 */
/* MISRA C-2012 Rule 11.1 - Deviation record ID - H3_MISRAC_2012_R_11_1_DR_1 */
/* MISRA C-2012 Rule 11.3 - Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
/* MISRA C-2012 Rule 11.8 - Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */



// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************
/******************************************************
 * USB Driver Initialization
 ******************************************************/
 
/*  When designing a Self-powered USB Device, the application should make sure
    that USB_DEVICE_Attach() function is called only when VBUS is actively powered.
    Therefore, the firmware needs some means to detect when the Host is powering 
    the VBUS. A 5V tolerant I/O pin can be connected to VBUS (through a resistor)
    and can be used to detect when VBUS is high or low. The application can specify
    a VBUS Detect function through the USB Driver Initialize data structure. 
    The USB device stack will periodically call this function. If the VBUS is 
    detected, the USB_DEVICE_EVENT_POWER_DETECTED event is generated. If the VBUS 
    is removed (i.e., the device is physically detached from Host), the USB stack 
    will generate the event USB_DEVICE_EVENT_POWER_REMOVED. The application should 
    call USB_DEVICE_Detach() when VBUS is removed. 
    
    The following are the steps to generate the VBUS_SENSE Function through MHC     
        1) Navigate to MHC->Tools->Pin Configuration and Configure the pin used 
           as VBUS_SENSE. Set this pin Function as "GPIO" and set as "Input". 
           Provide a custom name to the pin.
        2) Select the USB Driver Component in MHC Project Graph and enable the  
           "Enable VBUS Sense" Check-box.     
        3) Specify the custom name of the VBUS SENSE pin in the "VBUS SENSE Pin Name" box.  */ 
static DRV_USB_VBUS_LEVEL DRV_USBFSV1_VBUS_Comparator(void)
{
    DRV_USB_VBUS_LEVEL retVal = DRV_USB_VBUS_LEVEL_INVALID;
    
    if(1U == USB_VBUS_SENSE_Get())
    {
        retVal = DRV_USB_VBUS_LEVEL_VALID;
    }
    return (retVal);

}

static const DRV_USBFSV1_INIT drvUSBFSInit =
{
    /* Interrupt Source for USB module */
    .interruptSource = USB_OTHER_IRQn,
 
    /* Interrupt Source for USB module */
    .interruptSource1 = USB_SOF_IRQn,
 
    /* Interrupt Source for USB module */
    .interruptSource2 = USB_TRCPT0_IRQn,
 
    /* Interrupt Source for USB module */
    .interruptSource3 = USB_TRCPT1_IRQn,

    /* System module initialization */
    .moduleInit = {0},

    /* USB Controller to operate as USB Device */
    .operationMode = DRV_USBFSV1_OPMODE_DEVICE,

    /* USB Full Speed Operation */
    .operationSpeed = USB_SPEED_FULL,
    
    /* Stop in idle */
    .runInStandby = true,

    /* Suspend in sleep */
    .suspendInSleep = false,

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_REGS,

    /* Function to check for VBus */
    .vbusComparator = DRV_USBFSV1_VBUS_Comparator

};




// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

static const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)RTC_Timer32CallbackRegister,
    .timerStart = (SYS_TIME_PLIB_START)RTC_Timer32Start,
    .timerStop = (SYS_TIME_PLIB_STOP)RTC_Timer32Stop,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)RTC_Timer32FrequencyGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)NULL,
};

static const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = RTC_IRQn,
};

// </editor-fold>

static const SYS_DEBUG_INIT debugInit =
{
    .moduleInit = {0},
    .errorLevel = SYS_DEBUG_GLOBAL_ERROR_LEVEL,
    .consoleIndex = 0,
};


// <editor-fold defaultstate="collapsed" desc="SYS_CONSOLE Instance 0 Initialization Data">


static const SYS_CONSOLE_UART_PLIB_INTERFACE sysConsole0UARTPlibAPI =
{
    .read_t = (SYS_CONSOLE_UART_PLIB_READ)SERCOM5_USART_Read,
    .readCountGet = (SYS_CONSOLE_UART_PLIB_READ_COUNT_GET)SERCOM5_USART_ReadCountGet,
    .readFreeBufferCountGet = (SYS_CONSOLE_UART_PLIB_READ_FREE_BUFFFER_COUNT_GET)SERCOM5_USART_ReadFreeBufferCountGet,
    .write_t = (SYS_CONSOLE_UART_PLIB_WRITE)SERCOM5_USART_Write,
    .writeCountGet = (SYS_CONSOLE_UART_PLIB_WRITE_COUNT_GET)SERCOM5_USART_WriteCountGet,
    .writeFreeBufferCountGet = (SYS_CONSOLE_UART_PLIB_WRITE_FREE_BUFFER_COUNT_GET)SERCOM5_USART_WriteFreeBufferCountGet,
};

static const SYS_CONSOLE_UART_INIT_DATA sysConsole0UARTInitData =
{
    .uartPLIB = &sysConsole0UARTPlibAPI,
};

static const SYS_CONSOLE_INIT sysConsole0Init =
{
    .deviceInitData = (const void*)&sysConsole0UARTInitData,
    .consDevDesc = &sysConsoleUARTDevDesc,
    .deviceIndex = 0,
};



// </editor-fold>




// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************

/* MISRAC 2012 deviation block end */

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{

    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */

  
    PORT_Initialize();

    CLOCK_Initialize();




    EVSYS_Initialize();

    PM_Initialize();

    SERCOM5_USART_Initialize();

    RTC_Initialize();

	BSP_Initialize();

    /* MISRAC 2012 deviation block start */
    /* Following MISRA-C rules deviated in this block  */
    /* MISRA C-2012 Rule 11.3 - Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
    /* MISRA C-2012 Rule 11.8 - Deviation record ID - H3_MISRAC_2012_R_11_8_DR_1 */


    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -  
    H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
        
    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);
    
    /* MISRAC 2012 deviation block end */
    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -  
     H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
        
    sysObj.sysDebug = SYS_DEBUG_Initialize(SYS_DEBUG_INDEX_0, (SYS_MODULE_INIT*)&debugInit);

    /* MISRAC 2012 deviation block end */
    /* MISRA C-2012 Rule 11.3, 11.8 deviated below. Deviation record ID -  
     H3_MISRAC_2012_R_11_3_DR_1 & H3_MISRAC_2012_R_11_8_DR_1*/
        sysObj.sysConsole0 = SYS_CONSOLE_Initialize(SYS_CONSOLE_INDEX_0, (SYS_MODULE_INIT *)&sysConsole0Init);
   /* MISRAC 2012 deviation block end */

    /* Initialize USB Driver */ 
    sysObj.drvUSBFSV1Object = DRV_USBFSV1_Initialize(DRV_USBFSV1_INDEX_0, (SYS_MODULE_INIT *) &drvUSBFSInit);


    /* Initialize the USB device layer */
    sysObj.usbDevObject0 = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData0);



    /* MISRAC 2012 deviation block end */
    //APP_Initialize();
    USBAPP2_Initialize();


    NVIC_Initialize();


    /* MISRAC 2012 deviation block end */
}

/*******************************************************************************
 End of File
*/
