/*
 * File: z_dm_bal.c
 * Overlay: ovl_Dm_Bal
 * Description: Tingle (cutscene)
 */

#include "z_dm_bal.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10 | ACTOR_FLAG_2000000)

#define THIS ((DmBal*)thisx)

void DmBal_Init(Actor* thisx, PlayState* play);
void DmBal_Destroy(Actor* thisx, PlayState* play);
void DmBal_Update(Actor* thisx, PlayState* play);
void DmBal_Draw(Actor* thisx, PlayState* play);

void DmBal_SetupDoNothing(DmBal* this);
void DmBal_DoNothing(DmBal* this, PlayState* play);

ActorInit Dm_Bal_InitVars = {
    ACTOR_DM_BAL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_BAL,
    sizeof(DmBal),
    (ActorFunc)DmBal_Init,
    (ActorFunc)DmBal_Destroy,
    (ActorFunc)DmBal_Update,
    (ActorFunc)DmBal_Draw,
};

static AnimationInfo sAnimationInfo[] = {
    { &object_bal_Anim_0005FC, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },
    { &object_bal_Anim_000840, 1.5f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },
    { &object_bal_Anim_000840, 1.5f, 0.0f, 0.0f, ANIMMODE_ONCE, -4.0f },
    { &object_bal_Anim_00A7DC, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -4.0f },
    { &object_bal_Anim_00B1E8, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },
    { &object_bal_Anim_00B604, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },
    { &object_bal_Anim_00C498, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -8.0f },
    { &object_bal_Anim_00C8D8, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },
    { &object_bal_Anim_00C8D8, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -8.0f },
    { &object_bal_Anim_00C498, 1.0f, 23.0f, 0.0f, ANIMMODE_ONCE, -8.0f },
    { &object_bal_Anim_00D530, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },
    { &object_bal_Anim_000C78, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -2.0f },
    { &object_bal_Anim_00CB78, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -2.0f },
    { &object_bal_Anim_001804, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -2.0f },
};

void DmBal_Init(Actor* thisx, PlayState* play) {
    DmBal* this = THIS;

    this->actor.targetMode = 1;
    this->actor.uncullZoneForward = 3000.0f;
    Actor_SetScale(&this->actor, 0.02f);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &object_bal_Skel_00A6D0, &object_bal_Anim_0005FC, this->jointTable,
                       this->morphTable, OBJECT_BAL_LIMB_MAX);
    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, 4);
    this->timer = 60;
    this->eyeIndex = 0;
    this->keepEyesShut = false;
    DmBal_SetupDoNothing(this);
}

void DmBal_Destroy(Actor* thisx, PlayState* play) {
}

void DmBal_SetupDoNothing(DmBal* this) {
    this->actionFunc = DmBal_DoNothing;
}

void DmBal_DoNothing(DmBal* this, PlayState* play) {
}

void func_80C1EAE8(DmBal* this, PlayState* play) {
    static u16 D_80C1F2C0 = 0x63;
    s32 actionIndex;

    if (Cutscene_CheckActorAction(play, 0x238)) {
        actionIndex = Cutscene_GetActorActionIndex(play, 0x238);

        if (D_80C1F2C0 != play->csCtx.actorActions[actionIndex]->action) {
            D_80C1F2C0 = play->csCtx.actorActions[actionIndex]->action;
            switch (play->csCtx.actorActions[actionIndex]->action) {
                case 1:
                    this->keepEyesShut = false;
                    this->eyeIndex = 0;
                    Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, 0);
                    break;
                case 2:
                    this->keepEyesShut = true;
                    Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, 12);
                    break;
                case 3:
                    Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, 13);
                    break;
            }
        } else if (D_80C1F2C0 == 3) {
            if (Animation_OnFrame(&this->skelAnime, 0.0f)) {
                this->keepEyesShut = true;
            } else if (Animation_OnFrame(&this->skelAnime, 29.0f)) {
                this->keepEyesShut = false;
                this->eyeIndex = 0;
            }
        }
        Cutscene_ActorTranslateAndYaw(&this->actor, play, actionIndex);
        this->actor.home.pos = this->actor.world.pos;
    } else {
        this->keepEyesShut = false;
        this->eyeIndex = 0;
        D_80C1F2C0 = 0x63;
    }
}

void func_80C1EC60(DmBal* this, PlayState* play) {
    f32 scaleX;

    this->unk_338 += 0x320;
    this->unk_33A += 0x3E8;
    this->scale.y = this->scale.z = Math_CosS(this->unk_338) * 0.1f + 1.0f;
    scaleX = (Math_SinS(this->unk_338) * 0.1f) + 1.0f;
    this->scale.x = SQ(scaleX);
    this->actor.world.pos.y = this->actor.home.pos.y + (Math_SinS(this->unk_338) * 25.0f);
}

void DmBal_UpdateEyes(DmBal* this) {
    if (this->keepEyesShut == true) {
        this->eyeIndex = 1;
    } else if (this->timer >= 4) {
        this->timer--;
    } else if (this->timer != 0) {
        this->eyeIndex = 1;
        this->timer--;
    } else {
        this->eyeIndex = 0;
        this->timer = 60;
    }
}

void DmBal_SpawnPaper(DmBal* this, PlayState* play, Vec3f* pos, Vec3f* vel, f32 gravity) {
    Actor* paper = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_PAPER, pos->x, pos->y, pos->z, 0, 0, 0, 0);

    if (paper != NULL) {
        paper->velocity = *vel;
        paper->gravity = gravity;
    }
}

void DmBal_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    DmBal* this = THIS;

    // Throw confetti
    if (Animation_OnFrame(&this->skelAnime, 29.0f) && (this->skelAnime.animation == &object_bal_Anim_001804)) {
        Vec3f pos = this->actor.world.pos;
        Vec3f vel = { 0.0f, 9.0f, 0.0f };

        pos.x += 7.0f * Math_SinS(this->actor.shape.rot.y);
        pos.y += 2.5f;
        pos.z += 7.0f * Math_CosS(this->actor.shape.rot.y);
        vel.x = Math_SinS(this->actor.shape.rot.y) * 5.0f;
        vel.z = Math_CosS(this->actor.shape.rot.y) * 5.0f;
        DmBal_SpawnPaper(this, play, &pos, &vel, -0.4f);
        DmBal_SpawnPaper(this, play, &pos, &vel, -0.5f);
    }
    this->actionFunc(this, play);
    func_80C1EAE8(this, play);
    func_80C1EC60(this, play);
    DmBal_UpdateEyes(this);
    SkelAnime_Update(&this->skelAnime);
}

s32 DmBal_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    DmBal* this = THIS;
    Vec3s rots;

    if (limbIndex == 6) {
        rots.x = Math_SinS(this->unk_33A) * (0x10000 / 18);
        rots.z = Math_CosS(this->unk_33A) * (0x10000 / 18);
        Matrix_RotateZYX(rots.x, 0, rots.z, MTXMODE_APPLY);
        Matrix_Scale(this->scale.x, this->scale.y, this->scale.z, MTXMODE_APPLY);
        Matrix_RotateZS(-rots.z, MTXMODE_APPLY);
        Matrix_RotateXS(-rots.x, MTXMODE_APPLY);
    }
    return false;
}

void DmBal_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
}

static TexturePtr sEyeTextures[] = { object_bal_Tex_006050, object_bal_Tex_0094D0 };

void DmBal_Draw(Actor* thisx, PlayState* play) {
    DmBal* this = THIS;

    OPEN_DISPS(play->state.gfxCtx);
    func_8012C28C(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[this->eyeIndex]));
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          DmBal_OverrideLimbDraw, DmBal_PostLimbDraw, &this->actor);
    CLOSE_DISPS(play->state.gfxCtx);
}
