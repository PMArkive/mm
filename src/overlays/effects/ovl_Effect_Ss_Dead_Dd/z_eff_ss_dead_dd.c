/*
 * File: z_eff_ss_dead_dd.c
 * Overlay: ovl_Effect_Ss_Dead_Dd
 * Description:
 */

#include "z_eff_ss_dead_dd.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rScale regs[0]
#define rPrimColorR regs[2]
#define rPrimColorG regs[3]
#define rPrimColorB regs[4]
#define rAlpha regs[5]
#define rEnvColorR regs[6]
#define rEnvColorG regs[7]
#define rEnvColorB regs[8]
#define rScaleStep regs[9]
#define rAlphaStep regs[10]
#define rAlphaMode regs[11]

#define PARAMS ((EffectSsDeadDdInitParams*)initParamsx)

u32 EffectSsDeadDd_Init(PlayState* play, u32 index, EffectSs* this, void* initParamsx);
void EffectSsDeadDd_Update(PlayState* play, u32 index, EffectSs* this);
void EffectSsDeadDd_Draw(PlayState* play, u32 index, EffectSs* this);

const EffectSsInit Effect_Ss_Dead_Dd_InitVars = {
    EFFECT_SS_DEAD_DD,
    EffectSsDeadDd_Init,
};

u32 EffectSsDeadDd_Init(PlayState* play, u32 index, EffectSs* this, void* initParamsx) {
    EffectSsDeadDdInitParams* initParams = PARAMS;

    if (initParams->type == DEADDD_TYPE_0) {
        this->pos = initParams->pos;
        this->velocity = initParams->velocity;
        this->accel = initParams->accel;
        this->life = initParams->life;
        this->rScaleStep = initParams->scaleStep;
        this->rAlphaMode = initParams->alphaStep;

        if (initParams->alphaStep != 0) {
            this->rAlphaStep = initParams->alphaStep;
        } else {
            this->rAlphaStep = initParams->primColor.a / initParams->life;
        }

        this->draw = EffectSsDeadDd_Draw;
        this->update = EffectSsDeadDd_Update;
        this->rScale = initParams->scale;
        this->rPrimColorR = initParams->primColor.r;
        this->rPrimColorG = initParams->primColor.g;
        this->rPrimColorB = initParams->primColor.b;
        this->rAlpha = initParams->primColor.a;
        this->rEnvColorR = initParams->envColor.r;
        this->rEnvColorG = initParams->envColor.g;
        this->rEnvColorB = initParams->envColor.b;

    } else if (initParams->type == DEADDD_TYPE_1) {
        s32 i;

        this->life = initParams->life;
        this->rScaleStep = initParams->scaleStep;
        this->rAlphaMode = 0;
        this->rAlphaStep = 155 / initParams->life;
        this->rScale = initParams->scale;
        this->rPrimColorR = 255;
        this->rPrimColorG = 255;
        this->rPrimColorB = 155;
        this->rAlpha = 155;
        this->rEnvColorR = 250;
        this->rEnvColorG = 180;
        this->rEnvColorB = 0;
        this->draw = EffectSsDeadDd_Draw;
        this->update = EffectSsDeadDd_Update;

        for (i = initParams->randIter; i > 0; i--) {
            this->pos.x = ((Rand_ZeroOne() - 0.5f) * initParams->randPosScale) + initParams->pos.x;
            this->pos.y = ((Rand_ZeroOne() - 0.5f) * initParams->randPosScale) + initParams->pos.y;
            this->pos.z = ((Rand_ZeroOne() - 0.5f) * initParams->randPosScale) + initParams->pos.z;
            this->accel.x = this->velocity.x = (Rand_ZeroOne() - 0.5f) * 2.0f;
            this->accel.y = this->velocity.y = (Rand_ZeroOne() - 0.5f) * 2.0f;
            this->accel.z = this->velocity.z = (Rand_ZeroOne() - 0.5f) * 2.0f;
        }
    } else {
        return 0;
    }

    return 1;
}

void EffectSsDeadDd_Draw(PlayState* play, u32 index, EffectSs* this) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    Mtx* mtx;
    f32 scale;

    OPEN_DISPS(gfxCtx);

    scale = this->rScale * 0.01f;
    SkinMatrix_SetTranslate(&mfTrans, this->pos.x, this->pos.y, this->pos.z);
    SkinMatrix_SetScale(&mfScale, scale, scale, scale);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &mfScale, &mfResult);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        func_8012C974(gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, this->rPrimColorR, this->rPrimColorG, this->rPrimColorB, this->rAlpha);
        gDPSetEnvColor(POLY_XLU_DISP++, this->rEnvColorR, this->rEnvColorG, this->rEnvColorB, this->rAlpha);
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(POLY_XLU_DISP++, &D_01000000, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gDPSetCombineLERP(POLY_XLU_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
                          PRIMITIVE, 0);
        gSPDisplayList(POLY_XLU_DISP++, gLensFlareCircleDL);
    }

    CLOSE_DISPS(gfxCtx);
}

void EffectSsDeadDd_Update(PlayState* play, u32 index, EffectSs* this) {
    this->rScale += this->rScaleStep;

    if (this->rScale < 0) {
        this->rScale = 0;
    }

    if (this->rAlphaMode != 0) {
        this->rAlpha += this->rAlphaStep;
        if (this->rAlpha > 255) {
            this->rAlpha = 255;
        }
    } else {
        if (this->rAlpha < this->rAlphaStep) {
            this->rAlpha = 0;
        } else {
            this->rAlpha -= this->rAlphaStep;
        }
    }
}
