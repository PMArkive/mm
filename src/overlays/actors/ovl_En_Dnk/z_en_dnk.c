/*
 * File: z_en_dnk.c
 * Overlay: ovl_En_Dnk
 * Description: Hallucinatory Mad Scrubs (Deku curse and healing cutscenes)
 */

#include "z_en_dnk.h"
#include "objects/object_dnk/object_dnk.h"
#include "objects/object_hintnuts/object_hintnuts.h"
#include "objects/object_dekunuts/object_dekunuts.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8)

#define THIS ((EnDnk*)thisx)

void EnDnk_Init(Actor* thisx, PlayState* play);
void EnDnk_Destroy(Actor* thisx, PlayState* play);
void EnDnk_Update(Actor* thisx, PlayState* play);

void func_80A51890(EnDnk* this, PlayState* play);
void EnDnk_DoNothing(EnDnk* this, PlayState* play);
void func_80A52018(Actor* thisx, PlayState* play);
void func_80A52134(EnDnk* this, PlayState* play);

static s16 D_80A521A0 = 0;

ActorInit En_Dnk_InitVars = {
    ACTOR_EN_DNK,
    ACTORCAT_NPC,
    FLAGS,
    GAMEPLAY_KEEP,
    sizeof(EnDnk),
    (ActorFunc)EnDnk_Init,
    (ActorFunc)EnDnk_Destroy,
    (ActorFunc)EnDnk_Update,
    (ActorFunc)NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 18, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 1, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(1, 0x0),
    /* Deku Stick     */ DMG_ENTRY(1, 0x0),
    /* Horse trample  */ DMG_ENTRY(1, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0x0),
    /* Zora boomerang */ DMG_ENTRY(1, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(1, 0x0),
    /* Hookshot       */ DMG_ENTRY(1, 0x0),
    /* Goron punch    */ DMG_ENTRY(1, 0x0),
    /* Sword          */ DMG_ENTRY(1, 0x0),
    /* Goron pound    */ DMG_ENTRY(1, 0x0),
    /* Fire arrow     */ DMG_ENTRY(1, 0x0),
    /* Ice arrow      */ DMG_ENTRY(1, 0x0),
    /* Light arrow    */ DMG_ENTRY(1, 0x0),
    /* Goron spikes   */ DMG_ENTRY(1, 0x0),
    /* Deku spin      */ DMG_ENTRY(1, 0x0),
    /* Deku bubble    */ DMG_ENTRY(1, 0x0),
    /* Deku launch    */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(1, 0x0),
    /* Zora barrier   */ DMG_ENTRY(1, 0x0),
    /* Normal shield  */ DMG_ENTRY(1, 0x0),
    /* Light ray      */ DMG_ENTRY(1, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0x0),
    /* Zora punch     */ DMG_ENTRY(1, 0x0),
    /* Spin attack    */ DMG_ENTRY(1, 0x0),
    /* Sword beam     */ DMG_ENTRY(1, 0x0),
    /* Normal Roll    */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(1, 0x0),
    /* Unblockable    */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(1, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0x0),
};

static AnimationInfoS sAnimationInfo[] = {
    { &gDekuPalaceGuardStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gDekuPalaceGuardStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gDekuPalaceGuardWaitAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuPalaceGuardAttackAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuPalaceGuardDigAnim, 1.0f, -1, -1, ANIMMODE_ONCE, 0 },
    { &gDekuPalaceGuardDigAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gDekuPalaceGuardJumpAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gDekuPalaceGuardWalkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gDekuPalaceGuardWalkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuPalaceGuardHappyAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_hintnuts_Anim_000168, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &object_hintnuts_Anim_0024CC, 1.0f, -1, -1, ANIMMODE_ONCE, 0 },
    { &object_hintnuts_Anim_0024CC, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &object_hintnuts_Anim_0026C4, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &object_hintnuts_Anim_002894, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &object_hintnuts_Anim_002B90, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &object_hintnuts_Anim_002F7C, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_hintnuts_Anim_002F7C, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_hintnuts_Anim_003128, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &object_hintnuts_Anim_003128, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_hintnuts_Anim_0029BC, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &object_hintnuts_Anim_002E84, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuScrubSpitAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuScrubDamageAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gDekuScrubBurrowAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gDekuScrubBurrowAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gDekuScrubDieAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gDekuScrubUnburrowAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gDekuScrubLookAroundAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuScrubUpAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gDekuScrubIdleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gDekuScrubIdleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuScrubPantingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuScrubRunAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gDekuScrubRunAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gDekuScrubStandingIdleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
};

s32 func_80A514F0(SkelAnime* skelAnime, s16 animIndex) {
    s16 frame;
    s16 frameCount;
    s32 sp30 = false;

    if (animIndex >= 0) {
        if (animIndex < ARRAY_COUNT(sAnimationInfo)) {
            sp30 = true;
            frameCount = sAnimationInfo[animIndex].frameCount;
            if (frameCount < 0) {
                frameCount = Animation_GetLastFrame(sAnimationInfo[animIndex].animation);
            }
            frame = sAnimationInfo[animIndex].startFrame;
            if (frame < 0) {
                frame = frameCount;
            }
            Animation_Change(skelAnime, sAnimationInfo[animIndex].animation, sAnimationInfo[animIndex].playSpeed, frame,
                             frameCount, sAnimationInfo[animIndex].mode, sAnimationInfo[animIndex].morphFrames);
        }
    }
    return sp30;
}

s32 func_80A515C4(EnDnk* this) {
    s32 ret = false;

    if (DECR(this->unk_29E) == 0) {
        this->unk_2A0++;
        if (this->unk_2A0 >= 3) {
            this->unk_29E = Rand_S16Offset(20, 20);
            this->unk_2A0 = 0;
        }
        ret = true;
    }
    return ret;
}

void func_80A51648(EnDnk* this, PlayState* play) {
    if (SubS_IsObjectLoaded(this->unk_28E, play) == true) {
        gSegments[0x06] = VIRTUAL_TO_PHYSICAL(play->objectCtx.status[this->unk_28E].segment);
        this->actor.draw = func_80A52018;
        this->actor.objBankIndex = this->unk_28E;
        ActorShape_Init(&this->actor.shape, 0.0f, NULL, 18.0f);

        switch (ENDNK_GET_3(&this->actor)) {
            case ENDNK_GET_3_0:
                SkelAnime_Init(play, &this->skelAnime, &gDekuPalaceGuardSkel, NULL, this->jointTable, this->morphTable,
                               DEKU_PALACE_GUARD_LIMB_MAX);
                func_80A514F0(&this->skelAnime, 7);
                break;

            case ENDNK_GET_3_1:
                SkelAnime_Init(play, &this->skelAnime, &object_hintnuts_Skel_0023B8.sh, NULL, this->jointTable,
                               this->morphTable, 10);
                func_80A514F0(&this->skelAnime, 18);
                break;

            case ENDNK_GET_3_2:
                SkelAnime_Init(play, &this->skelAnime, &gDekuScrubSkel, NULL, this->jointTable, this->morphTable,
                               DEKU_SCRUB_LIMB_MAX);
                func_80A514F0(&this->skelAnime, 35);
                break;
        }

        Collider_InitCylinder(play, &this->collider);
        Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
        CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
        if (ENDNK_GET_3C(&this->actor) == 4) {
            this->actor.flags &= ~ACTOR_FLAG_1;
            this->actor.flags |= (ACTOR_FLAG_10 | ACTOR_FLAG_20);
            this->actionFunc = func_80A51890;
            Actor_SetScale(&this->actor, 0.1f);
        } else {
            this->actor.flags &= ~ACTOR_FLAG_1;
            this->actionFunc = EnDnk_DoNothing;
            Actor_SetScale(&this->actor, 0.01f);
        }
    }
}

void func_80A51890(EnDnk* this, PlayState* play) {
    if (Cutscene_CheckActorAction(play, 126)) {
        Cutscene_ActorTranslateAndYaw(&this->actor, play, Cutscene_GetActorActionIndex(play, 126));
    }
}

void EnDnk_DoNothing(EnDnk* this, PlayState* play) {
}

void EnDnk_Init(Actor* thisx, PlayState* play) {
    EnDnk* this = THIS;

    this->unk_28E = -1;
    switch (ENDNK_GET_3(&this->actor)) {
        case ENDNK_GET_3_1:
            this->unk_28E = SubS_GetObjectIndex(OBJECT_HINTNUTS, play);
            break;

        case ENDNK_GET_3_0:
            this->unk_28E = SubS_GetObjectIndex(OBJECT_DNK, play);
            break;

        case ENDNK_GET_3_2:
            this->unk_28E = SubS_GetObjectIndex(OBJECT_DEKUNUTS, play);
            break;
    }

    if (this->unk_28E >= 0) {
        this->actionFunc = func_80A51648;
    } else {
        Actor_Kill(&this->actor);
    }

    this->unk_2A2 = D_80A521A0;
    D_80A521A0++;
}

void EnDnk_Destroy(Actor* thisx, PlayState* play) {
    EnDnk* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
}

void EnDnk_Update(Actor* thisx, PlayState* play) {
    EnDnk* this = THIS;
    s32 pad;

    this->actionFunc(this, play);
    SkelAnime_Update(&this->skelAnime);
    func_80A515C4(this);
    Actor_SetFocus(&this->actor, 34.0f);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    func_80A52134(this, play);
}

s32 func_80A51A78(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnDnk* this = THIS;

    this->unk_260[limbIndex] = *dList;
    *dList = NULL;
    return false;
}

void func_80A51AA4(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnDnk* this = THIS;
    MtxF sp5C;
    Vec3f sp50 = gZeroVec3f;
    Vec3f sp44;
    Vec3s sp3C;

    if (limbIndex == 2) {
        Matrix_MultVec3f(&sp50, &sp44);
        Matrix_Get(&sp5C);
        Matrix_MtxFToYXZRot(&sp5C, &sp3C, false);
        Matrix_Translate(sp44.x, sp44.y, sp44.z, MTXMODE_NEW);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
        if (this->unk_28C & 0x10) {
            if (this->unk_28C & 0x20) {
                sp3C.x = this->unk_296;
                sp3C.y = this->unk_298;
                sp3C.y += this->actor.shape.rot.y;
                sp3C.z = 0;
                Math_SmoothStepToS(&this->unk_290, sp3C.x, 4, 0x1FFE, 1);
                Math_SmoothStepToS(&this->unk_292, sp3C.y, 4, 0x1FFE, 1);
                Math_SmoothStepToS(&this->unk_294, sp3C.z, 4, 0x1FFE, 1);
            } else {
                this->unk_290 = sp3C.x;
                this->unk_292 = sp3C.y;
                this->unk_294 = sp3C.z;
            }
        } else {
            this->unk_28C |= 0x10;
            this->unk_290 = sp3C.x;
            this->unk_292 = sp3C.y;
            this->unk_294 = sp3C.z;
        }

        Matrix_RotateYS(this->unk_292, MTXMODE_APPLY);
        Matrix_RotateXS(this->unk_290, MTXMODE_APPLY);
        Matrix_RotateZS(this->unk_294, MTXMODE_APPLY);
    }

    OPEN_DISPS(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, this->unk_260[limbIndex]);

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80A51CB8(EnDnk* this, PlayState* play) {
    static TexturePtr D_80A5245C[] = {
        gDekuPalaceGuardEyeOpenTex,
        gDekuPalaceGuardEyeHalfTex,
        gDekuPalaceGuardEyeClosedTex,
    };
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C28C(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(D_80A5245C[this->unk_2A0]));
    gDPPipeSync(POLY_OPA_DISP++);

    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, func_80A51A78, func_80A51AA4,
                      &this->actor);

    CLOSE_DISPS(play->state.gfxCtx);
}

s32 func_80A51D78(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnDnk* this = THIS;

    this->unk_260[limbIndex] = *dList;
    *dList = NULL;
    return false;
}

void func_80A51DA4(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnDnk* this = THIS;
    MtxF sp5C;
    Vec3f sp50 = gZeroVec3f;
    Vec3f sp44;
    Vec3s sp3C;

    if (limbIndex == 2) {
        Matrix_MultVec3f(&sp50, &sp44);
        Matrix_Get(&sp5C);
        Matrix_MtxFToYXZRot(&sp5C, &sp3C, false);
        Matrix_Translate(sp44.x, sp44.y, sp44.z, MTXMODE_NEW);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);

        if (this->unk_28C & 0x10) {
            if (this->unk_28C & 0x20) {
                sp3C.z = this->unk_296 + 0x4000;
                sp3C.y = this->unk_298 + 0x4000;
                sp3C.y += this->actor.shape.rot.y;
                sp3C.x = 0;
                Math_SmoothStepToS(&this->unk_290, sp3C.x, 4, 0x1FFE, 1);
                Math_SmoothStepToS(&this->unk_292, sp3C.y, 4, 0x1FFE, 1);
                Math_SmoothStepToS(&this->unk_294, sp3C.z, 4, 0x1FFE, 1);
            } else {
                this->unk_290 = sp3C.x;
                this->unk_292 = sp3C.y;
                this->unk_294 = sp3C.z;
            }
        } else {
            this->unk_28C |= 0x10;
            this->unk_290 = sp3C.x;
            this->unk_292 = sp3C.y;
            this->unk_294 = sp3C.z;
        }

        Matrix_RotateYS(this->unk_292, MTXMODE_APPLY);
        Matrix_RotateXS(this->unk_290, MTXMODE_APPLY);
        Matrix_RotateZS(this->unk_294, MTXMODE_APPLY);
    }

    OPEN_DISPS(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, this->unk_260[limbIndex]);

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80A51FC0(EnDnk* this, PlayState* play) {
    func_8012C28C(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, func_80A51D78, func_80A51DA4,
                      &this->actor);
}

void func_80A52018(Actor* thisx, PlayState* play) {
    EnDnk* this = THIS;

    switch (ENDNK_GET_3(thisx)) {
        case ENDNK_GET_3_0:
            func_80A51CB8(this, play);
            break;

        case ENDNK_GET_3_1:
        case ENDNK_GET_3_2:
            func_80A51FC0(this, play);
            break;
    }
}

void func_80A52074(EnDnk* this, PlayState* play) {
    switch (play->csCtx.frames) {
        case 80:
            func_8019F128(NA_SE_EN_DEKNUTS_DANCE1);
            break;

        case 123:
            func_8019F128(NA_SE_EN_DEKNUTS_DANCE2);
            break;

        case 438:
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_DEKNUTS_DANCE_BIG);
            break;

        case 493:
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_STALKIDS_APPEAR);
            break;
    }

    if ((play->csCtx.frames >= 198) && (play->csCtx.frames < 438)) {
        func_8019F128(NA_SE_EN_DEKNUTS_DANCE - SFX_FLAG);
    }
}

void func_80A52134(EnDnk* this, PlayState* play) {
    if ((play->csCtx.state != 0) && (ENDNK_GET_3C(&this->actor) == 4) && (play->sceneId == SCENE_SPOT00) &&
        (gSaveContext.sceneLayer == 2)) {
        func_80A52074(this, play);
    }
}
