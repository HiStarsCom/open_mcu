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
  * @file      main.h
  * @author    MCU Driver Team
  * @brief     This file contains driver init functions.
  */

/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef McuMagicTag_SYSTEM_INIT_H
#define McuMagicTag_SYSTEM_INIT_H

#include "gpio.h"
#include "timer.h"
#include "timer_ex.h"
#include "crg.h"
#include "iocmg.h"

#define    IO_SPEED_FAST     0x00U
#define    IO_SPEED_SLOW     0x01U

#define    IO_DRV_LEVEL4     0x00U
#define    IO_DRV_LEVEL3     0x01U
#define    IO_DRV_LEVEL2     0x02U
#define    IO_DRV_LEVEL1     0x03U

#define    XTAL_DRV_LEVEL4   0x03U
#define    XTAL_DRV_LEVEL3   0x02U
#define    XTAL_DRV_LEVEL2   0x01U
#define    XTAL_DRV_LEVEL1   0x00U

#define LED_A_PIN GPIO_PIN_1
#define LED_A_HANDLE g_gpio1
#define LED_B_PIN GPIO_PIN_4
#define LED_B_HANDLE g_gpio1
#define LED_C_PIN GPIO_PIN_7
#define LED_C_HANDLE g_gpio1
#define LED_D_PIN GPIO_PIN_0
#define LED_D_HANDLE g_gpio2
#define LED_E_PIN GPIO_PIN_6
#define LED_E_HANDLE g_gpio2
#define LED_G_PIN GPIO_PIN_1
#define LED_G_HANDLE g_gpio3
#define LED_H_PIN GPIO_PIN_5
#define LED_H_HANDLE g_gpio3
#define LED_F_PIN GPIO_PIN_0
#define LED_F_HANDLE g_gpio3
#define LED1_SELECT_PIN GPIO_PIN_6
#define LED1_SELECT_HANDLE g_gpio3
#define LED2_SELECT_PIN GPIO_PIN_7
#define LED2_SELECT_HANDLE g_gpio3
#define LED3_SELECT_PIN GPIO_PIN_0
#define LED3_SELECT_HANDLE g_gpio4
#define LED4_SELECT_PIN GPIO_PIN_1
#define LED4_SELECT_HANDLE g_gpio4

extern TIMER_Handle g_timer1;

extern GPIO_Handle g_gpio1;
extern GPIO_Handle g_gpio2;
extern GPIO_Handle g_gpio3;
extern GPIO_Handle g_gpio4;

BASE_StatusType CRG_Config(CRG_CoreClkSelect *coreClkSelect);
void SystemInit(void);

void TIMER1_InterruptProcess(void *handle);
void TIMER1_DMAOverFlow_InterruptProcess(void *handle);

#endif /* McuMagicTag_SYSTEM_INIT_H */