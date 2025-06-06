/*
 * File: z_eff_ss_extra.c
 * Overlay: ovl_Effect_Ss_Extra
 * Description: The floating points that pop-up in Swamp Bow Minigame
 *   i.e. 100 points for hitting the deku scrubs in the background
 */

#include "z_eff_ss_extra.h"
#include "objects/object_yabusame_point/object_yabusame_point.h"

#define PARAMS ((EffectSsExtraInitParams*)initParamsx)

u32 EffectSsExtra_Init(PlayState* play, u32 index, EffectSs* this, void* initParamsx);
void EffectSsExtra_Update(PlayState* play, u32 index, EffectSs* this);
void EffectSsExtra_Draw(PlayState* play, u32 index, EffectSs* this);

static s16 sScores[] = { 30, 60, 100 };

const EffectSsInit Effect_Ss_Extra_InitVars = {
    EFFECT_SS_EXTRA,
    EffectSsExtra_Init,
};

static TexturePtr sPointTextures[] = { gYabusamePoint30Tex, gYabusamePoint60Tex, gYabusamePoint100Tex };

#define rObjId regs[0]
#define rTimer regs[1]
#define rScoreIndex regs[2]
#define rScale regs[3]

u32 EffectSsExtra_Init(PlayState* play, u32 index, EffectSs* this, void* initParamsx) {
    s32 pad;
    EffectSsExtraInitParams* params = PARAMS;
    s32 objIndex;

    objIndex = Object_GetIndex(&play->objectCtx, OBJECT_YABUSAME_POINT);
    if ((objIndex >= 0) && (Object_IsLoaded(&play->objectCtx, objIndex))) {
        void* segBackup = gSegments[6];

        gSegments[6] = VIRTUAL_TO_PHYSICAL(play->objectCtx.status[objIndex].segment);

        this->pos = params->pos;
        this->velocity = params->velocity;
        this->accel = params->accel;
        this->draw = EffectSsExtra_Draw;
        this->update = EffectSsExtra_Update;
        this->life = 50;
        this->rScoreIndex = params->scoreIdx;
        this->rScale = params->scale;
        this->rTimer = 5;
        this->rObjId = objIndex;

        gSegments[6] = segBackup;
        return 1;
    }
    return 0;
}

void EffectSsExtra_Draw(PlayState* play, u32 index, EffectSs* this) {
    s32 pad;
    f32 scale;
    void* storedSegment;

    scale = this->rScale / 100.0f;
    storedSegment = play->objectCtx.status[this->rObjId].segment;

    OPEN_DISPS(play->state.gfxCtx);

    gSegments[6] = VIRTUAL_TO_PHYSICAL(storedSegment);

    gSPSegment(POLY_XLU_DISP++, 0x06, storedSegment);

    Matrix_Translate(this->pos.x, this->pos.y, this->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    func_8012C2DC(play->state.gfxCtx);
    Matrix_ReplaceRotation(&play->billboardMtxF);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPSegment(POLY_XLU_DISP++, 0x08, Lib_SegmentedToVirtual(sPointTextures[this->rScoreIndex]));

    gSPDisplayList(POLY_XLU_DISP++, &gYabusamePointDL);

    CLOSE_DISPS(play->state.gfxCtx);
}

void EffectSsExtra_Update(PlayState* play, u32 index, EffectSs* this) {
    if (this->rTimer != 0) {
        this->rTimer--;
    } else {
        this->velocity.y = 0.0f;
    }

    if (this->rTimer == 1) {
        play->interfaceCtx.minigamePoints = sScores[this->rScoreIndex];
    }
}
