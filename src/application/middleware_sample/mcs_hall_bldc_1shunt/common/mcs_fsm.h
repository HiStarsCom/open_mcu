/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2023. All rights reserved.
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
  * @file      mcs_fsm.h
  * @author    MCU Algorithm Team
  * @brief     This file provides the definition of finite statemachine (FSM).
  */

#ifndef McuMagicTag_MCS_FSM_H
#define McuMagicTag_MCS_FSM_H

/**
  * @brief motor control FSM state define.
  * @details motor control FSM state type:
  *          + FSM_IDLE         -- IDLE state, system startup default.
  *          + FSM_OFFSET_CALIB -- Self calibrate, for ADC init.
  *          + FSM_CAP_CHARGE   -- IPM cap charge.
  *          + FSM_HFI          -- High frequency injection.
  *          + FSM_CLEAR        -- Clear before every run.
  *          + FSM_STARTUP      -- Start up.
  *          + FSM_RUN          -- Normal running state.
  *          + FSM_BRAKE        -- Brake.
  *          + FSM_STOP         -- Normal stop.
  *          + FSM_FAULT        -- Fault state, waiting for user process.
  */
typedef enum {
    FSM_IDLE = 0,       // 0
    FSM_CAP_CHARGE,     // 1
    FSM_OFFSET_CALIB,   // 2
    FSM_HFI,            // 3
    FSM_CLEAR,          // 4
    FSM_LOCATE,         // 5
    FSM_STARTUP,        // 6
    FSM_RUN,            // 7
    FSM_BRAKE,          // 8
    FSM_STOP,           // 9
    FSM_FAULT           // 10
} FSM_State;

#endif