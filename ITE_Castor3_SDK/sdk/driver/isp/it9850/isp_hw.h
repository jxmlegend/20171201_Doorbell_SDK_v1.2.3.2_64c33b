#ifndef __ISP_HW_H_HL88MIOL_N62I_1CVD_9BT8_WHZ2ZK9PMMDS__
#define __ISP_HW_H_HL88MIOL_N62I_1CVD_9BT8_WHZ2ZK9PMMDS__

#ifdef __cplusplus
extern "C" {
#endif

#include "isp_types.h"

//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================
void
ISP_LogReg(
    void);

//=============================================================================
/**
 * Clear ISP Interrupt.
 */
//=============================================================================
void
ISP_ClearInterrupt_Reg(
    void);

//=============================================================================
/**
 * Driver Fire ISP Engine.
 */
//=============================================================================
void
ISP_DriverFire_Reg(
    void);

//=============================================================================
/**
 * Driver Update ISP Parameter.
 */
//=============================================================================
void
ISP_UpdateFire_Reg(
    void);

//=============================================================================
/**
 * Set Input Format
 */
//=============================================================================
void
ISP_SetInputParameter_Reg(
    const ISP_INPUT_INFO *pInInfo);

//=============================================================================
/**
 * Set ISP input buffer relate parameters.
 */
//=============================================================================
void
ISP_SetInputBuf_Reg(
    const ISP_INPUT_INFO *pInInfo);

//=============================================================================
/**
 * Set ISP input buffer address relate parameters
 */
//=============================================================================
void
ISP_SetInputAddr_Reg(
    const ISP_INPUT_INFO *pInInfo);

//=============================================================================
/**
 * Set Remap Y Address
 */
//=============================================================================
void
ISP_SetRemapYAddress_Reg(
    const MMP_UINT8 tableIdx);

//=============================================================================
/**
 * Set Remap UV Address
 */
//=============================================================================
void
ISP_SetRemapUVAddress_Reg(
    const MMP_UINT8 tableIdx);

//=============================================================================
/**
 * Set Deinterlace Parameter.
 */
//=============================================================================
void
ISP_SetDeInterlaceParam_Reg(
    ISP_DEVICE                 ptDev,
    const ISP_DEINTERLACE_CTRL *pDeInterlace);

//=============================================================================
/**
 * Set 3D-Deinterlace parameters.
 */
//=============================================================================
void
ISP_Set3DDeInterlaceParm_Reg(
    ISP_DEVICE ptDev);

//=============================================================================
/**
 * Set 2D-Deinterlace parameters.
 */
//=============================================================================
void
ISP_Set2DDeInterlaceParam_Reg(
    ISP_DEVICE ptDev);

//=============================================================================
/**
 * YUV to RGB transfer matrix.
 */
//=============================================================================
void
ISP_SetYUVtoRGBMatrix_Reg(
    const ISP_YUV_TO_RGB *pYUVtoRGB);

//=============================================================================
/**
 * Set color correction matrix and constant
 */
//=============================================================================
void
ISP_SetCCMatrix_Reg(
    const ISP_COLOR_CORRECTION *pColorCorrect);

//=============================================================================
/*
 * Set Pre-Scale Register
 */
//=============================================================================
void
ISP_SetPreScaleParam_Reg(
    const ISP_PRESCALE_CTRL *pPreScaleFun);

//=============================================================================
/**
 * Set Pre-Scale Weight.
 */
//=============================================================================
void
ISP_SetPreScaleMatrix_Reg(
    const ISP_PRESCALE_CTRL *pPreScaleFun);

void
    ISP_SetIntPreScaleMatrix_Reg(
    MMP_UINT8 WeightMatX[][ISP_SCALE_TAP]);

//=============================================================================
/*
 * Set Scale Factor
 */
//=============================================================================
void
ISP_SetScaleParam_Reg(
    const ISP_SCALE_CTRL *pScaleFun);

//=============================================================================
/**
 * Set Frame Function 0
 */
//=============================================================================
void
ISP_SetFrameFun0_Reg(
    const ISP_FRMFUN_CTRL *pFrameFun);

//=============================================================================
/**
 * Set Output Format
 */
//=============================================================================
void
ISP_SetOutParameter_Reg(
    const ISP_OUTPUT_INFO *pOutInfo);

//=============================================================================
/**
 * Set Output Information
 */
//=============================================================================
void
ISP_SetOutBufInfo_Reg(
    const ISP_OUTPUT_INFO *pOutInfo);

//=============================================================================
/**
 * Set Output Address
 */
//=============================================================================
void
ISP_SetOutAddress_Reg(
    const ISP_OUTPUT_INFO *pOutInfo);

//=============================================================================
/**
 * Set ISP Engine Mode.
 */
//=============================================================================
void
ISP_SetEngineMode_Reg(
    const ISP_ENGINE_MODE_CTRL *pEngineMode);

//=============================================================================
/**
 * Set Run-Length Encoder
 */
//=============================================================================
void
ISP_SetRunLengthEnc_Reg(
    const ISP_RUNLEN_ENC_CTRL *pCtrl);

//=============================================================================
/**
 * Wait ISP engine idle!  //for JPG module use
 */
//=============================================================================
ISP_RESULT
ISP_WaitEngineIdle(
    void);

//=============================================================================
/**
 * Wait ISP interrupt idle!
 */
//=============================================================================
ISP_RESULT
ISP_WaitInterruptIdle(
    void);

//=============================================================================
/**
 * Set Interrupt Information
 */
//=============================================================================
void
ISP_SetInterruptParameter_Reg(
    const ISP_CONTEXT *pISPctxt);

//=============================================================================
/**
 * ISP engine clock related.
 */
//=============================================================================
void
ISP_PowerUp(
    void);

void
ISP_PowerDown(
    void);

#ifdef __cplusplus
}
#endif

#endif