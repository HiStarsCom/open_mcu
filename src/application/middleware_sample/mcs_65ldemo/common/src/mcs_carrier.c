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
  * @file      mcs_carrier.c
  * @author    MCU Algorithm Team
  * @brief     This file provides carrier interrupt application for motor control.
  */

#include "mcs_carrier.h"
#include "mcs_math.h"
#include "typedefs.h"
#include "mcs_assert.h"

/**
  * @brief Synchronous rotation coordinate system angle.
  * @param mtrCtrl The motor control handle.
  * @retval None.
  */
static void MCS_SyncCoorAngle(MTRCTRL_Handle *mtrCtrl)
{
    MCS_ASSERT_PARAM(mtrCtrl != NULL);
    /* Synchronous rotation coordinate system angle. */
    switch (mtrCtrl->stateMachine) {
        case FSM_STARTUP:
            /* Current ramp angle is 0. */
            if (mtrCtrl->startup.stage == STARTUP_STAGE_CURR) {
                mtrCtrl->axisAngle = 0;
            } else if (mtrCtrl->startup.stage == STARTUP_STAGE_SPD) { /* IF control phase angle self-addition. */
                mtrCtrl->axisAngle = IF_CurrAngleCalc(&mtrCtrl->ifCtrl, mtrCtrl->spdRef);
            } else if (mtrCtrl->startup.stage == STARTUP_STAGE_SWITCH) { /* Switch Angle */
                mtrCtrl->axisAngle = mtrCtrl->smo.elecAngle;
            }
            break;

        case FSM_RUN:
            mtrCtrl->axisAngle = mtrCtrl->smo.elecAngle;
            break;

        default:
            mtrCtrl->axisAngle = 0;
            break;
    }
}

/**
  * @brief PWM waveform setting and sampling point setting for a single resistors and three resistors.
  * @param mtrCtrl The motor control handle.
  * @retval None.
  */
static void MCS_PwmAdcSet(MTRCTRL_Handle *mtrCtrl)
{
    MCS_ASSERT_PARAM(mtrCtrl != NULL);
    MCS_SampleMode sampleMode = mtrCtrl->sampleMode;
    /* Set the duty cycle according to the sampling mode. */
    if (sampleMode == THREE_RESISTORS) {
        SVPWM_Exec(&mtrCtrl->sv, &mtrCtrl->vabRef, &mtrCtrl->dutyUvw);
        mtrCtrl->setPwmDutyCb(&mtrCtrl->dutyUvw, &mtrCtrl->dutyUvw);
    } else if (sampleMode == SINGLE_RESISTOR) {
        R1SVPWM_Exec(&mtrCtrl->r1Sv, &mtrCtrl->vabRef, &mtrCtrl->dutyUvwLeft, &mtrCtrl->dutyUvwRight);
        mtrCtrl->setPwmDutyCb(&mtrCtrl->dutyUvwLeft, &mtrCtrl->dutyUvwRight);
        /* The ADC sampling point position needs to be set based on the phase shift of a single resistors. */
        mtrCtrl->setADCTriggerTimeCb((unsigned short)(mtrCtrl->r1Sv.samplePoint[0] * mtrCtrl->aptMaxcntCmp), \
                                     (unsigned short)(mtrCtrl->r1Sv.samplePoint[1] * mtrCtrl->aptMaxcntCmp));
    }
}

/**
  * @brief Carrier interrupt function.
  * @param mtrCtrl The motor control handle.
  * @retval None.
  */
void MCS_CarrierProcess(MTRCTRL_Handle *mtrCtrl)
{
    MCS_ASSERT_PARAM(mtrCtrl != NULL);
    MCS_ASSERT_PARAM(mtrCtrl->sampleMode < SAMPLE_MODE_END);
    MCS_ASSERT_PARAM(mtrCtrl->readCurrUvwCb != NULL);
    MCS_ASSERT_PARAM(mtrCtrl->setPwmDutyCb != NULL);
    MCS_ASSERT_PARAM(mtrCtrl->readCurrBiasCb != NULL);

    if (mtrCtrl->sampleMode == SINGLE_RESISTOR && mtrCtrl->setADCTriggerTimeCb == NULL) {
        return;
    }
    UvwAxis *iuvw = &mtrCtrl->iuvw;
    AlbeAxis *iabFbk = &mtrCtrl->iabFbk;
    AlbeAxis *vabRef = &mtrCtrl->vabRef;
    IBIAS_Handle *iuvwAdcBias = &mtrCtrl->adcCalibrCurrUvw;

    /* Read the three-phase current value. */
    if (mtrCtrl->stateMachine != FSM_OFFSET_CALIB) {
        mtrCtrl->readCurrUvwCb(iuvw, iuvwAdcBias);
    }
    /* Clark Calc */
    ClarkeCalc(iuvw, iabFbk);

    /* Smo observation */
    FOSMO_Exec(&mtrCtrl->smo, iabFbk, vabRef, mtrCtrl->spdRef);
    mtrCtrl->spdFbk = mtrCtrl->smo.spdEst;
    /* Synchronization angle */
    MCS_SyncCoorAngle(mtrCtrl);

    /* Park transformation */
    ParkCalc(iabFbk, mtrCtrl->axisAngle, &mtrCtrl->idqFbk);

    /* statemachine */
    switch (mtrCtrl->stateMachine) {
        case FSM_STARTUP:
        case FSM_RUN:
            CURRCTRL_Exec(&mtrCtrl->currCtrl, &mtrCtrl->vdqRef, mtrCtrl->spdRef, 0); /* feedforward disabled. */
            InvParkCalc(&mtrCtrl->vdqRef, mtrCtrl->axisAngle, vabRef);
            MCS_PwmAdcSet(mtrCtrl);
            break;

        case FSM_OFFSET_CALIB:
            /* Calibrate ADC shift due to temperature */
            mtrCtrl->readCurrBiasCb(iuvwAdcBias);
            break;

        case FSM_CAP_CHARGE:
        case FSM_CLEAR:
        case FSM_IDLE:
            break;

        default:
            vabRef->alpha = 0.0f;
            vabRef->beta = 0.0f;
            MCS_PwmAdcSet(mtrCtrl);
            break;
    }
}