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

#define UART0_BAND_RATE 115200

#define SPI1_FREQ_SCR 2
#define SPI1_FREQ_CPSDVSR 10

BASE_StatusType CRG_Config(CRG_CoreClkSelect *coreClkSelect)
{
    CRG_Handle crg;
    crg.baseAddress     = CRG; /* crg baseaddress */
    crg.pllRefClkSelect = CRG_PLL_REF_CLK_SELECT_HOSC;
    crg.pllPreDiv       = CRG_PLL_PREDIV_4;
    crg.pllFbDiv        = 32; /* PLL Multiplier 32 */
    crg.pllPostDiv      = CRG_PLL_POSTDIV_1;
    crg.coreClkSelect   = CRG_CORE_CLK_SELECT_HOSC;
    crg.handleEx.clk1MSelect   = CRG_1M_CLK_SELECT_HOSC;
    crg.handleEx.pllPostDiv2   = CRG_PLL_POSTDIV2_1;

    if (HAL_CRG_Init(&crg) != BASE_STATUS_OK) { /* CRG init */
        return BASE_STATUS_ERROR;
    }
    *coreClkSelect = crg.coreClkSelect;
    return BASE_STATUS_OK;
}

static void SPI1_Init(void)
{
    HAL_CRG_IpEnableSet(SPI1_BASE, IP_CLK_ENABLE);

    g_spiSampleHandle.baseAddress = SPI1;

    g_spiSampleHandle.mode = HAL_SPI_MASTER;  /* spi master */
    g_spiSampleHandle.csMode = SPI_CHIP_SELECT_MODE_INTERNAL;
    g_spiSampleHandle.xFerMode = HAL_XFER_MODE_BLOCKING;
    g_spiSampleHandle.clkPolarity = HAL_SPI_CLKPOL_0; /* spi ploarity is 0 */
    g_spiSampleHandle.clkPhase =  HAL_SPI_CLKPHA_0;
    g_spiSampleHandle.endian = HAL_SPI_BIG_ENDIAN;
    g_spiSampleHandle.frameFormat = HAL_SPI_MODE_MOTOROLA; /* motorola frame format */
    g_spiSampleHandle.dataWidth = SPI_DATA_WIDTH_16BIT;
    g_spiSampleHandle.freqScr = SPI1_FREQ_SCR;
    g_spiSampleHandle.freqCpsdvsr = SPI1_FREQ_CPSDVSR;
    g_spiSampleHandle.waitEn = BASE_CFG_DISABLE;
    g_spiSampleHandle.waitVal = 127; /* mircowire wait time 127 */
    g_spiSampleHandle.rxBuff = NULL;
    g_spiSampleHandle.txBuff = NULL;
    g_spiSampleHandle.transferSize = 0;
    g_spiSampleHandle.txCount = 0; /* transfer count */
    g_spiSampleHandle.rxCount = 0;
    g_spiSampleHandle.state = HAL_SPI_STATE_RESET;
    g_spiSampleHandle.rxIntSize =  SPI_RX_INTERRUPT_SIZE_1;
    g_spiSampleHandle.txIntSize =  SPI_TX_INTERRUPT_SIZE_1; /* tx interrupt size */
    g_spiSampleHandle.rxDMABurstSize =  SPI_RX_DMA_BURST_SIZE_1;
    g_spiSampleHandle.txDMABurstSize =  SPI_TX_DMA_BURST_SIZE_1;
    HAL_SPI_Init(&g_spiSampleHandle);
    HAL_SPI_ChipSelectChannelSet(&g_spiSampleHandle, SPI_CHIP_SELECT_CHANNEL_1);
}

static void UART0_Init(void)
{
    HAL_CRG_IpEnableSet(UART0_BASE, IP_CLK_ENABLE);

    g_uart0.baseAddress = UART0; /* uart0 baseaaddress */

    g_uart0.baudRate = UART0_BAND_RATE;
    g_uart0.dataLength = UART_DATALENGTH_8BIT; /* data length 8 */
    g_uart0.stopBits = UART_STOPBITS_ONE;
    g_uart0.parity = UART_PARITY_NONE;
    g_uart0.txMode = UART_MODE_BLOCKING; /* blocking mode */
    g_uart0.rxMode = UART_MODE_BLOCKING;
    g_uart0.fifoMode = BASE_CFG_ENABLE;
    g_uart0.fifoTxThr = UART_FIFODEPTH_SIZE8; /* fifo size 8 */
    g_uart0.fifoRxThr = UART_FIFODEPTH_SIZE8;
    g_uart0.hwFlowCtr = BASE_CFG_DISABLE;
    g_uart0.handleEx.overSampleMultiple = UART_OVERSAMPLING_16X;
    g_uart0.handleEx.msbFirst = BASE_CFG_DISABLE;
    HAL_UART_Init(&g_uart0);
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

    HAL_IOCMG_SetPinAltFuncMode(GPIO4_6_AS_SPI1_CLK);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO4_6_AS_SPI1_CLK, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO4_6_AS_SPI1_CLK, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO4_6_AS_SPI1_CLK, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO4_6_AS_SPI1_CLK, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO1_3_AS_SPI1_TXD);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO1_3_AS_SPI1_TXD, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO1_3_AS_SPI1_TXD, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO1_3_AS_SPI1_TXD, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO1_3_AS_SPI1_TXD, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO1_4_AS_SPI1_RXD);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO1_4_AS_SPI1_RXD, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO1_4_AS_SPI1_RXD, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO1_4_AS_SPI1_RXD, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO1_4_AS_SPI1_RXD, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO4_5_AS_SPI1_CSN0);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO4_5_AS_SPI1_CSN0, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO4_5_AS_SPI1_CSN0, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO4_5_AS_SPI1_CSN0, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO4_5_AS_SPI1_CSN0, DRIVER_RATE_2);  /* Output signal edge fast/slow */

    HAL_IOCMG_SetPinAltFuncMode(GPIO3_0_AS_SPI1_CSN1);  /* Check function selection */
    HAL_IOCMG_SetPinPullMode(GPIO3_0_AS_SPI1_CSN1, PULL_NONE);  /* Pull-up and pull-down */
    HAL_IOCMG_SetPinSchmidtMode(GPIO3_0_AS_SPI1_CSN1, SCHMIDT_DISABLE);  /* Schmitt input on/off */
    HAL_IOCMG_SetPinLevelShiftRate(GPIO3_0_AS_SPI1_CSN1, LEVEL_SHIFT_RATE_SLOW);  /* Output drive capability */
    HAL_IOCMG_SetPinDriveRate(GPIO3_0_AS_SPI1_CSN1, DRIVER_RATE_2);  /* Output signal edge fast/slow */
}

void SystemInit(void)
{
    IOConfig();
    UART0_Init();
    SPI1_Init();

    /* USER CODE BEGIN system_init */
    /* USER CODE END system_init */
}