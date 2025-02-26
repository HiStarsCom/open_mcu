/**
  * @copyright Copyright (c) 2022, HiSilicon (Shanghai) Technologies Co., Ltd. All rights reserved.
  * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
  * following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
  * disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
  * following disclaimer in the documentation and/or other materials provided with the distribution.
  * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
  * products derived from this software without specific prior written permission.
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
  * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * @file      system_init.c
  * @author    MCU Driver Team
  * @brief     This file contains driver init functions.
  */

#include "main.h"
#include "ioconfig.h"
#include "iocmg_ip.h"
#include "debug.h"

#define UART0_BAND_RATE 115200

BASE_StatusType CRG_Config(CRG_CoreClkSelect *coreClkSelect)
{
    BASE_FUNC_ASSERT_PARAM(coreClkSelect != NULL);

    CRG_Handle crg;
    crg.baseAddress     = CRG;
    crg.pllRefClkSelect = CRG_PLL_REF_CLK_SELECT_HOSC;
    crg.pllFbDiv        = 0x30;  /* PLL loop divider ratio = 0x30 */
    crg.pllPreDiv       = CRG_PLL_PREDIV_4;
    crg.pllPostDiv      = CRG_PLL_POSTDIV_2;
    crg.coreClkSelect   = 0;                     /* Core select HOSC. */
    crg.handleEx.pllPostDiv2 = CRG_PLL_POSTDIV2_3;
    crg.handleEx.clk1MSelect = CRG_1M_CLK_SELECT_HOSC; /* Set the 1MHz clock select. */
    crg.handleEx.clk1MDiv = 0x29;   /* Default frequency divider of the 1 MHz clock = 0x29 */
    if (HAL_CRG_Init(&crg) != BASE_STATUS_OK) {
        return BASE_STATUS_ERROR;                 /* Crg error. */
    }
    *coreClkSelect = crg.coreClkSelect;
    return BASE_STATUS_OK;
}

__weak void ACMP0PositveCallFunc(void *handle)
{
    BASE_FUNC_UNUSED(handle);
    /* USER CODE BEGIN ACMP0PositveCallFunc */
    /* USER CODE END ACMP0PositveCallFunc */
}

__weak void ACMP0NegativeCallFunc(void *handle)
{
    BASE_FUNC_UNUSED(handle);
    /* USER CODE BEGIN ACMP0NegativeCallFunc */
    /* USER CODE END ACMP0NegativeCallFunc */
}

__weak void ACMP0EdgedCallFunc(void *handle)
{
    BASE_FUNC_UNUSED(handle);
    /* USER CODE BEGIN ACMP0EdgedCallFunc */
    /* USER CODE END ACMP0EdgedCallFunc */
}

static void ACMP0_Init(void)
{
    HAL_CRG_IpEnableSet(ACMP0_BASE, IP_CLK_ENABLE);                /* ACMP clock bit reset. */
    HAL_CRG_IpClkSelectSet(ACMP0_BASE, 0);
    g_acmp0.baseAddress =  ACMP0_BASE;
    g_acmp0.inOutConfig.inputNNum = ACMP_INPUT_N_SELECT3;     /* ACMP pin configure. */
    g_acmp0.inOutConfig.inputPNum = ACMP_INPUT_P_SELECT5;
    g_acmp0.inOutConfig.polarity = ACMP_OUT_NOT_INVERT;
    g_acmp0.filterCtrl.filterMode = ACMP_FILTER_NONE;       /* ACMP filter mode. */
    g_acmp0.hysteresisVol = ACMP_HYS_VOL_ZERO;
    g_acmp0.interruptEn = BASE_CFG_SET;
    HAL_ACMP_Init(&g_acmp0);                              /* ACMP function call back. */
    HAL_ACMP_RegisterCallBack(&g_acmp0, ACMP_POS_INT, ACMP0PositveCallFunc);
    HAL_ACMP_RegisterCallBack(&g_acmp0, ACMP_NEG_INT, ACMP0NegativeCallFunc);
    HAL_ACMP_RegisterCallBack(&g_acmp0, ACMP_EDGE_INT, ACMP0EdgedCallFunc);
    IRQ_Register(IRQ_ACMP_INT, HAL_ACMP_IrqHandler, &g_acmp0);  /* ACMP IRQ register. */
    IRQ_SetPriority(IRQ_ACMP_INT, 1);
    IRQ_EnableN(IRQ_ACMP_INT);
}

static void DAC0_Init(void)
{
    HAL_CRG_IpEnableSet(DAC0_BASE, IP_CLK_ENABLE);  /* DAC0 clock enable. */
    HAL_CRG_IpClkSelectSet(DAC0_BASE, 0);
    g_dac0.baseAddress = DAC0;
    g_dac0.dacValue = 250;   /* 250: DAC convert value. */
    HAL_DAC_Init(&g_dac0);
}

static void UART0_Init(void)
{
    HAL_CRG_IpEnableSet(UART0_BASE, IP_CLK_ENABLE);

    g_uart0.baseAddress = UART0;       /* UART0 use to debug. */

    g_uart0.baudRate = UART0_BAND_RATE;
    g_uart0.dataLength = UART_DATALENGTH_8BIT;   /* word length 8 bit. */
    g_uart0.stopBits = UART_STOPBITS_ONE;       /* Stop bit one bit. */
    g_uart0.parity = UART_PARITY_NONE;
    g_uart0.txMode = UART_MODE_BLOCKING;       /* Blocking mode */
    g_uart0.rxMode = UART_MODE_BLOCKING;
    g_uart0.fifoMode = BASE_CFG_ENABLE;
    g_uart0.fifoTxThr = UART_FIFODEPTH_SIZE8;   /* FIFO depth */
    g_uart0.fifoRxThr = UART_FIFODEPTH_SIZE8;
    g_uart0.hwFlowCtr = BASE_CFG_DISABLE;
    g_uart0.handleEx.overSampleMultiple = UART_OVERSAMPLING_16X;  /* 16x oversampling. */
    g_uart0.handleEx.msbFirst = BASE_CFG_DISABLE;
    HAL_UART_Init(&g_uart0);
}

static void IOConfig(void)
{
    SYSCTRL0->SC_SYS_STAT.BIT.update_mode = 0;
    SYSCTRL0->SC_SYS_STAT.BIT.update_mode_clear = 1;
    HAL_IOCMG_SetPinAltFuncMode(GPIO0_3_AS_UART0_TXD);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO0_3_AS_UART0_TXD, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO0_3_AS_UART0_TXD, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO0_3_AS_UART0_TXD, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO0_3_AS_UART0_TXD, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO0_4_AS_UART0_RXD);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO0_4_AS_UART0_RXD, PULL_UP);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO0_4_AS_UART0_RXD, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO0_4_AS_UART0_RXD, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO0_4_AS_UART0_RXD, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO0_7_AS_ACMP0_OUT);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO0_7_AS_ACMP0_OUT, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO0_7_AS_ACMP0_OUT, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO0_7_AS_ACMP0_OUT, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO0_7_AS_ACMP0_OUT, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO2_6_AS_ACMP_N3);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO2_6_AS_ACMP_N3, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO2_6_AS_ACMP_N3, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO2_6_AS_ACMP_N3, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO2_6_AS_ACMP_N3, DRIVER_RATE_2);  /* Output signal edge fast/slow */
}

void SystemInit(void)
{
    IOConfig();
    UART0_Init();
    ACMP0_Init();
    DAC0_Init();
    /* USER CODE BEGIN system_init */
    /* USER CODE END system_init */
}