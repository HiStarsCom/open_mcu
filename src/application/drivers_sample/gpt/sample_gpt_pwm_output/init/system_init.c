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
#include "iocmg.h"

#define UART0_BAND_RATE 115200

BASE_StatusType CRG_Config(CRG_CoreClkSelect *coreClkSelect)
{
    CRG_Handle crg;
    crg.baseAddress     = CRG;
    crg.pllRefClkSelect = CRG_PLL_REF_CLK_SELECT_HOSC;
    crg.pllPreDiv       = CRG_PLL_PREDIV_4;
    crg.pllFbDiv        = 48; /* PLL Multiplier 48 */
    crg.pllPostDiv      = CRG_PLL_POSTDIV_2;
    crg.coreClkSelect   = CRG_CORE_CLK_SELECT_HOSC;
    crg.handleEx.clk1MSelect   = CRG_1M_CLK_SELECT_HOSC;
    crg.handleEx.pllPostDiv2   = CRG_PLL_POSTDIV2_3;

    if (HAL_CRG_Init(&crg) != BASE_STATUS_OK) {
        return BASE_STATUS_ERROR;
    }
    *coreClkSelect = crg.coreClkSelect;
    return BASE_STATUS_OK;
}

static void GPT2_Init(void)
{
    HAL_CRG_IpEnableSet(GPT2_BASE, IP_CLK_ENABLE);

    g_gptHandle.baseAddress = GPT2;
    g_gptHandle.clockDiv = 9;  /* 9 is the internal frequency division of GPT */
    g_gptHandle.period = 49999;  /* 49999 is the number of GPT counting cycles. */
    g_gptHandle.refA0.refdot = 10000;  /* 10000 is the value of PWM reference point A. */
    g_gptHandle.refA0.refAction = GPT_ACTION_OUTPUT_HIGH;  /* GPT Action high */
    g_gptHandle.refB0.refdot = 30000;  /* 30000 is the value of PWM reference point B. */
    g_gptHandle.refB0.refAction = GPT_ACTION_OUTPUT_LOW;
    g_gptHandle.bufLoad = BASE_CFG_ENABLE;
    g_gptHandle.pwmKeep = BASE_CFG_ENABLE;
    g_gptHandle.handleEx.periodIntEnable = BASE_CFG_DISABLE;
    g_gptHandle.handleEx.outputFinIntEnable = BASE_CFG_DISABLE;
    g_gptHandle.triggleAdcOutFinish = BASE_CFG_DISABLE;
    g_gptHandle.triggleAdcPeriod = BASE_CFG_DISABLE;

    HAL_GPT_Init(&g_gptHandle);
}

static void UART0_Init(void)
{
    HAL_CRG_IpEnableSet(UART0_BASE, IP_CLK_ENABLE);

    g_uart0Handle.baseAddress = UART0;

    g_uart0Handle.baudRate = UART0_BAND_RATE;
    g_uart0Handle.dataLength = UART_DATALENGTH_8BIT;
    g_uart0Handle.stopBits = UART_STOPBITS_ONE;
    g_uart0Handle.parity = UART_PARITY_NONE;
    g_uart0Handle.txMode = UART_MODE_BLOCKING;
    g_uart0Handle.rxMode = UART_MODE_BLOCKING;
    g_uart0Handle.fifoMode = BASE_CFG_ENABLE;
    g_uart0Handle.fifoTxThr = UART_FIFODEPTH_SIZE8;
    g_uart0Handle.fifoRxThr = UART_FIFODEPTH_SIZE8;
    g_uart0Handle.hwFlowCtr = BASE_CFG_DISABLE;
    g_uart0Handle.handleEx.overSampleMultiple = UART_OVERSAMPLING_16X;
    g_uart0Handle.handleEx.msbFirst = BASE_CFG_DISABLE;
    HAL_UART_Init(&g_uart0Handle);
}

static void IOConfig(void)
{
    HAL_IOCMG_SetPinAltFuncMode(GPIO2_2_AS_UART0_TXD);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO2_2_AS_UART0_TXD, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO2_2_AS_UART0_TXD, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO2_2_AS_UART0_TXD, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO2_2_AS_UART0_TXD, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO2_3_AS_UART0_RXD);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO2_3_AS_UART0_RXD, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO2_3_AS_UART0_RXD, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO2_3_AS_UART0_RXD, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO2_3_AS_UART0_RXD, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO5_2_AS_GPT2_PWM);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO5_2_AS_GPT2_PWM, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO5_2_AS_GPT2_PWM, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO5_2_AS_GPT2_PWM, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO5_2_AS_GPT2_PWM, DRIVER_RATE_2);  /* Output signal edge fast/slow */
}

void SystemInit(void)
{
    IOConfig();
    UART0_Init();
    GPT2_Init();
    /* USER CODE BEGIN system_init */
    /* USER CODE END system_init */
}