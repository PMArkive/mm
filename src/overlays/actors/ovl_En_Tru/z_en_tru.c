/*
 * File: z_en_tru.c
 * Overlay: ovl_En_Tru
 * Description: Koume in Woods of Mystery
 */

#include "z_en_tru.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((EnTru*)thisx)

void EnTru_Init(Actor* thisx, PlayState* play);
void EnTru_Destroy(Actor* thisx, PlayState* play);
void EnTru_Update(Actor* thisx, PlayState* play);
void EnTru_Draw(Actor* thisx, PlayState* play);

s32 func_80A875AC(Actor* thisx, PlayState* play);
s32 func_80A8777C(Actor* thisx, PlayState* play);
s32 func_80A87880(Actor* thisx, PlayState* play);
s32 func_80A87B48(Actor* thisx, PlayState* play);
s32 func_80A87DC0(Actor* thisx, PlayState* play);
void func_80A87FD0(EnTru* this, PlayState* play);
void func_80A881E0(EnTru* this, PlayState* play);

typedef enum {
    /* 0x00 */ KOUME_ANIM_INJURED_LYING_DOWN,
    /* 0x01 */ KOUME_ANIM_INJURED_LYING_DOWN_MORPH,
    /* 0x02 */ KOUME_ANIM_TRY_GET_UP,
    /* 0x03 */ KOUME_ANIM_INJURED_RAISE_HEAD,
    /* 0x04 */ KOUME_ANIM_INJURED_TALK,
    /* 0x05 */ KOUME_ANIM_INJURED_HEAD_UP,
    /* 0x06 */ KOUME_ANIM_INJURED_HEAD_UP_MORPH,
    /* 0x07 */ KOUME_ANIM_TAKE,
    /* 0x08 */ KOUME_ANIM_SHAKE, // Unused
    /* 0x09 */ KOUME_ANIM_DRINK,
    /* 0x0A */ KOUME_ANIM_FINISHED_DRINKING,
    /* 0x0B */ KOUME_ANIM_HEALED,
    /* 0x0C */ KOUME_ANIM_HOVER1,
    /* 0x0D */ KOUME_ANIM_TAKE_OFF,
    /* 0x0E */ KOUME_ANIM_FLY,
    /* 0x0F */ KOUME_ANIM_HOVER2,
    /* 0x10 */ KOUME_ANIM_MAX
} KoumeAnimation;

static UNK_TYPE D_80A88910[] = {
    0x0E08520C,
    0x16100000,
};

static UNK_TYPE D_80A88918[] = {
    0x0900000E,
    0x08630C12,
    0x16100000,
};

static UNK_TYPE D_80A88924[] = {
    0x0E08660C,
    0x10000000,
};

static UNK_TYPE D_80A8892C[] = {
    0x09000004, 0x00050E08, 0x650C1000, 0x1010000B, 0x0E08530C, 0x0F08540C, 0x1900040E, 0x08560C11,
    0x10100E00, 0xFF2B0000, 0x001E0027, 0x2C08640C, 0x2F00000C, 0x15090000, 0x0E08570C, 0x15090000,
    0x0E08580C, 0x15090000, 0x12102C08, 0x550C2F00, 0x000C100E, 0x08550C10,
};

static UNK_TYPE D_80A88984[] = {
    0x2CFFFF09, 0x00000E08, 0x640C1509, 0x00000E08, 0x570C1509, 0x00000E08, 0x580C1509, 0x00001210,
};

static UNK_TYPE D_80A889A4[] = {
    0x2CFFFF09,
    0x00000E08,
    0x550C1000,
};

ActorInit En_Tru_InitVars = {
    ACTOR_EN_TRU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TRU,
    sizeof(EnTru),
    (ActorFunc)EnTru_Init,
    (ActorFunc)EnTru_Destroy,
    (ActorFunc)EnTru_Update,
    (ActorFunc)EnTru_Draw,
};

#include "overlays/ovl_En_Tru/ovl_En_Tru.c"

static Vec3f D_80A8B250 = { 0.0f, 0.02f, 0.0f };

static Color_RGBA8 D_80A8B25C[] = {
    { 255, 20, 60, 0 },   { 255, 235, 160, 0 }, { 20, 60, 255, 0 },
    { 100, 100, 100, 0 }, { 255, 255, 200, 0 }, { 100, 100, 100, 0 },
};

static f32 D_80A8B274[] = { 60.0f, 255.0f, 60.0f };

static TexturePtr sDustTextures[] = {
    gEffDust8Tex, gEffDust7Tex, gEffDust6Tex, gEffDust5Tex, gEffDust4Tex, gEffDust3Tex, gEffDust2Tex, gEffDust1Tex,
};

static ColliderSphereInit sSphereInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_SPHERE,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 0, { { 0, 0, 0 }, 32 }, 100 },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 1, 20, 0, 0, MASS_IMMOVABLE };

static AnimationInfoS sAnimationInfo[] = {
    { &gKoumeInjuredLyingDownAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gKoumeInjuredLyingDownAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gKoumeTryGetUpAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gKoumeInjuredRaiseHeadAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gKoumeInjuredTalkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gKoumeInjuredHeadUpAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gKoumeInjuredHeadUpAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gKoumeTakeAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gKoumeShakeAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gKoumeDrinkAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gKoumeFinishedDrinkingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gKoumeHealedAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gKoumeHoverAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gKoumeTakeOffAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -4 },
    { &gKoumeFlyAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gKoumeHoverAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
};

static Vec3f D_80A8B3D8 = { 0.0f, 24.0f, 16.0f };
static Vec3f D_80A8B3E4 = { 0.0f, -3.0f, 3.0f };
static Vec3f D_80A8B3F0 = { 0.0f, 0.5f, 0.0f };
static Vec3f D_80A8B3FC = { 3000.0f, -800.0f, 0.0f };

void func_80A85620(EnTruUnkStruct* arg0, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4) {
    s16 i;

    for (i = 0; i < 30; i++, arg0++) {
        if (arg0->unk_00 == 0) {
            arg0->unk_02 = Rand_ZeroFloat(20.0f) + arg4;
            arg0->unk_01 = arg0->unk_02;
            arg0->unk_00 = 1;
            arg0->unk_04 = *arg1;
            arg0->unk_1C = gZeroVec3f;
            arg0->unk_10 = D_80A8B250;
            arg0->unk_28 = arg2;
            arg0->unk_2C = arg3;
            break;
        }
    }
}

void func_80A85788(EnTruUnkStruct* arg0, PlayState* play) {
    s32 i;
    s32 flag = false;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C2DC(play->state.gfxCtx);

    for (i = 0; i < 30; i++, arg0++) {
        if (arg0->unk_00 == 1) {
            f32 alpha;

            if (!flag) {
                gSPDisplayList(POLY_XLU_DISP++, D_80A89000);
                flag = true;
            }

            do {
                alpha = (f32)arg0->unk_02 / arg0->unk_01;
                alpha *= 255.0f;
            } while (0);

            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, 128);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (u8)alpha);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, -arg0->unk_02 * 5, 32, 64, 1, 0, 0, 32, 32));

            Matrix_Translate(arg0->unk_04.x, arg0->unk_04.y, arg0->unk_04.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(arg0->unk_28, arg0->unk_28, 1.0f, MTXMODE_APPLY);
            Matrix_Translate(0.0f, 14.0f, 0.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, D_80A890A8);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80A85AA4(EnTruUnkStruct* arg0, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4) {
    s16 i;

    for (i = 0; i < 30; i++, arg0++) {
        if (arg0->unk_00 == 0) {
            arg0->unk_02 = arg4;
            arg0->unk_01 = arg0->unk_02;
            arg0->unk_00 = 2;
            arg0->unk_04 = *arg1;
            arg0->unk_1C = gZeroVec3f;
            arg0->unk_10 = gZeroVec3f;
            arg0->unk_28 = arg2;
            arg0->unk_2C = arg3;
            break;
        }
    }
}

void func_80A85BCC(EnTruUnkStruct* arg0, PlayState* play) {
    s32 i;
    s32 flag = false;

    OPEN_DISPS(play->state.gfxCtx);
    if (play) {}

    func_8012C2DC(play->state.gfxCtx);

    for (i = 0; i < 30; i++, arg0++) {
        if (arg0->unk_00 == 2) {
            f32 alpha;

            if (!flag) {
                gDPPipeSync(POLY_XLU_DISP++);
                gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 200, 0);
                flag = true;
            }

            do {
                alpha = (f32)arg0->unk_02 / arg0->unk_01;
                alpha *= 255.0f;
            } while (0);

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 200, (u8)alpha);

            Matrix_Translate(arg0->unk_04.x, arg0->unk_04.y, arg0->unk_04.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(arg0->unk_28, arg0->unk_28, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, D_80A8A108);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_80A85E2C(EnTruUnkStruct* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, f32 arg4, f32 arg5, f32 arg6, u8 arg7) {
    s32 i;

    for (i = 0; i < 30; i++, arg0++) {
        if (arg0->unk_00 == 0) {
            arg0->unk_02 = Rand_ZeroFloat(4.0f) + arg6;
            arg0->unk_01 = arg0->unk_02;
            arg0->unk_00 = arg7;
            arg0->unk_04 = *arg1;
            arg0->unk_1C = *arg3;
            arg0->unk_10 = *arg2;
            arg0->unk_28 = arg4;
            arg0->unk_2C = arg5;
            break;
        }
    }
}

#ifdef NON_MATCHING
// s8/s6 flipped, same deal as above functions but the same fixes don't work
void func_80A85F84(EnTruUnkStruct* arg0, PlayState* play) {
    u8 flag = false;
    s32 i;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C2DC(play->state.gfxCtx);

    for (i = 0; i < 30; i++, arg0++) {
        f32 alpha;
        s32 index;

        if ((arg0->unk_00 == 3) || (arg0->unk_00 == 4) || (arg0->unk_00 == 5)) {
            if (!flag) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);
                gSPDisplayList(POLY_XLU_DISP++, gKoumeDustMaterialDL);
                flag = true;
            }

            Matrix_Push();

            do {
                alpha = (f32)arg0->unk_02 / arg0->unk_01;
                alpha *= D_80A8B274[arg0->unk_00 - 3];
            } while (0);

            gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0,
                              COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED);
            gDPSetRenderMode(POLY_XLU_DISP++, G_RM_FOG_SHADE_A, G_RM_ZB_CLD_SURF2);
            gSPSetGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, D_80A8B25C[arg0->unk_00 - 3].r, D_80A8B25C[arg0->unk_00 - 3].g,
                            D_80A8B25C[arg0->unk_00 - 3].b, (u8)alpha);
            gDPSetEnvColor(POLY_XLU_DISP++, D_80A8B25C[arg0->unk_00 - 3].r, D_80A8B25C[arg0->unk_00 - 3].g,
                           D_80A8B25C[arg0->unk_00 - 3].b, 0);

            Matrix_Translate(arg0->unk_04.x, arg0->unk_04.y, arg0->unk_04.z, MTXMODE_NEW);
            Matrix_Scale(arg0->unk_28, arg0->unk_28, 1.0f, MTXMODE_APPLY);
            Matrix_ReplaceRotation(&play->billboardMtxF);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            index = ((f32)arg0->unk_02 / arg0->unk_01) * 8.0f;
            gSPSegment(POLY_XLU_DISP++, 0x08, Lib_SegmentedToVirtual(sDustTextures[index]));
            gSPDisplayList(POLY_XLU_DISP++, gKoumeDustModelDL);

            Matrix_Pop();
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
#else
void func_80A85F84(EnTruUnkStruct* arg0, PlayState* play);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/ovl_En_Tru/func_80A85F84.s")
#endif

s32 func_80A86384(EnTruUnkStruct* arg0, Vec3f* arg1) {
    s32 i;

    for (i = 0; i < 30; i++, arg0++) {
        if (arg0->unk_02 != 0) {
            if (arg0->unk_00 == 2) {
                arg0->unk_04 = *arg1;
            } else {
                arg0->unk_04.x += arg0->unk_1C.x;
                arg0->unk_04.y += arg0->unk_1C.y;
                arg0->unk_04.z += arg0->unk_1C.z;
            }

            arg0->unk_1C.x += arg0->unk_10.x;
            arg0->unk_1C.y += arg0->unk_10.y;
            arg0->unk_1C.z += arg0->unk_10.z;

            arg0->unk_02--;
            arg0->unk_28 += arg0->unk_2C;
        } else {
            arg0->unk_00 = 0;
        }
    }

    return false;
}

s32 func_80A86460(EnTru* this) {
    s32 i;
    Vec3f spB0;
    Vec3f spA4;
    Vec3f sp98;
    Vec3f sp8C;
    f32 temp_f6;
    s16 phi_s1;

    Math_Vec3f_Copy(&spB0, &gZeroVec3f);
    Math_Vec3f_Copy(&sp8C, &gZeroVec3f);
    Math_Vec3f_Copy(&spA4, &gZeroVec3f);
    Math_Vec3f_Copy(&sp98, &gZeroVec3f);
    phi_s1 = (Rand_ZeroOne() * 360.0f) * 182.0f;
    spB0.z = 20.0f;
    Lib_Vec3f_TranslateAndRotateY(&this->actor.world.pos, this->actor.world.rot.y, &spB0, &sp8C);

    for (i = 0; i < 8; i++, phi_s1 += 0x1FFE) {
        Math_Vec3f_Copy(&spB0, &gZeroVec3f);
        spB0.y = 1.0f;
        spB0.z = Rand_ZeroOne() + 3.0f;
        Lib_Vec3f_TranslateAndRotateY(&gZeroVec3f, phi_s1, &spB0, &sp98);
        Math_Vec3f_Copy(&spB0, &gZeroVec3f);
        spB0.z = (Rand_ZeroOne() * 4.0f) + 12.0f;
        Lib_Vec3f_TranslateAndRotateY(&sp8C, phi_s1, &spB0, &spA4);
        func_80A85E2C(this->unk_394, &spA4, &gZeroVec3f, &sp98, 0.4f, 0.06f, 12.0f, 4);
    }

    return false;
}

s32 func_80A86674(EnTru* this) {
    s32 pad;
    Vec3f sp40;
    Vec3f sp34;

    Lib_Vec3f_TranslateAndRotateY(&this->actor.world.pos, this->actor.world.rot.y, &D_80A8B3D8, &sp40);
    Lib_Vec3f_TranslateAndRotateY(&gZeroVec3f, this->actor.world.rot.y, &D_80A8B3E4, &sp34);
    if (this->unk_390 == 1) {
        func_80A85E2C(this->unk_394, &sp40, &D_80A8B3F0, &sp34, 0.2f, 0.1f, 12.0f, 3);
    } else if (this->unk_390 == 2) {
        func_80A85E2C(this->unk_394, &sp40, &D_80A8B3F0, &sp34, 0.2f, 0.1f, 12.0f, 5);
    }

    return false;
}

s32 func_80A86770(EnTru* this) {
    s32 pad;
    Vec3f sp98;
    Vec3f sp8C;
    s32 i;
    s16 phi_s0 = Rand_ZeroOne() * 360.0f * 182.0f;

    for (i = 0; i < 4; i++, phi_s0 += 0x3FFC) {
        Lib_Vec3f_TranslateAndRotateY(&this->actor.world.pos, phi_s0, &gZeroVec3f, &sp98);
        sp98.y = this->actor.floorHeight + 1.0f;
        sp8C.x = Rand_ZeroOne() - 0.5f;
        sp8C.z = Rand_ZeroOne() - 0.5f;
        sp8C.y = Rand_ZeroOne() * 0.2f;
        func_80A85E2C(this->unk_394, &sp98, &sp8C, &gZeroVec3f, 1.0f, 0.04f, 28.0f, 4);
    }

    return false;
}

void EnTru_UpdateSkelAnime(EnTru* this) {
    this->skelAnime.playSpeed = this->playSpeed;
    SkelAnime_Update(&this->skelAnime);
}

s32 EnTru_ChangeAnim(EnTru* this, s32 animIndex) {
    s32 didChange = false;

    if (animIndex != this->animIndex) {
        this->animIndex = animIndex;
        didChange = SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, animIndex);
        this->playSpeed = this->skelAnime.playSpeed;
    }

    return didChange;
}

void func_80A8697C(EnTru* this, PlayState* play) {
    this->collider.dim.worldSphere.radius = this->collider.dim.modelSphere.radius * this->collider.dim.scale;
    if (this->actor.draw != NULL) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }
}

s32 func_80A869DC(EnTru* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f sp38;
    Vec3f sp2C;

    Math_ApproachS(&this->unk_368, this->actor.yawTowardsPlayer - this->actor.shape.rot.y, 4, 0x2AA8);
    this->unk_368 = CLAMP(this->unk_368, -0x1FFE, 0x1FFE);
    Math_Vec3f_Copy(&sp38, &player->actor.world.pos);
    sp38.y = player->bodyPartsPos[PLAYER_BODYPART_HEAD].y + 3.0f;
    Math_Vec3f_Copy(&sp2C, &this->actor.focus.pos);
    sp2C.y -= 30.0f;
    Math_ApproachS(&this->unk_366, Math_Vec3f_Pitch(&sp2C, &sp38), 4, 0x2AA8);
    this->unk_366 = CLAMP(this->unk_366, -0x1C70, 0x1C70);

    return true;
}

s32 func_80A86B0C(EnTru* this, PlayState* play) {
    if (this->unk_34E & 8) {
        this->unk_34E &= ~0x10;
        this->unk_34E |= 0x20;
        func_80A869DC(this, play);
    } else if (this->unk_34E & 0x20) {
        this->unk_34E &= ~0x20;
        this->unk_366 = 0;
        this->unk_368 = 0;
        this->unk_36A = 20;
    } else if (DECR(this->unk_36A) == 0) {
        this->unk_34E |= 0x10;
        this->unk_36A = 20;
    }

    return true;
}

s32 func_80A86BAC(EnTru* this, PlayState* play) {
    if (this->unk_34E & 0x400) {
        Matrix_Push();
        func_8012C28C(play->state.gfxCtx);

        OPEN_DISPS(play->state.gfxCtx);

        switch (this->unk_390) {
            case 1:
                gDPSetEnvColor(POLY_OPA_DISP++, 200, 0, 0, 0);
                break;

            case 2:
                gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 200, 0);
                break;

            default:
                gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 0);
                break;
        }

        Matrix_RotateXS(-0x4000, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gKoumePotionDL);

        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    }

    if (this->unk_34E & 0x800) {
        Matrix_Push();
        func_8012C2DC(play->state.gfxCtx);

        OPEN_DISPS(play->state.gfxCtx);

        gDPPipeSync(POLY_XLU_DISP++);

        Matrix_RotateXS(-0x4000, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gKoumeBottleDL);

        Matrix_Pop();

        CLOSE_DISPS(play->state.gfxCtx);
    }

    return false;
}

s32 func_80A86DB8(EnTru* this) {
    if (this->unk_34E & 0x80) {
        this->eyeTexIndex = 2;
        return false;
    }

    switch (this->animIndex) {
        case KOUME_ANIM_INJURED_LYING_DOWN:
        case KOUME_ANIM_INJURED_LYING_DOWN_MORPH:
            if (DECR(this->blinkTimer) == 0) {
                s16 rand = Rand_S16Offset(40, 20);

                if (this->eyeTexIndex == 2) {
                    this->blinkTimer = 8;
                } else {
                    this->blinkTimer = rand;
                }

                if (this->eyeTexIndex == 2) {
                    this->eyeTexIndex = 1;
                } else {
                    this->eyeTexIndex = 2;
                }
            }
            return false;

        case KOUME_ANIM_TRY_GET_UP:
            if (Animation_OnFrame(&this->skelAnime, 10.0f)) {
                this->eyeTexIndex = 1;
            } else if ((this->skelAnime.curFrame >= 11.0f) && (this->skelAnime.curFrame <= 32.0f)) {
                this->eyeTexIndex = 0;
            } else {
                this->eyeTexIndex = 2;
            }
            return false;

        case KOUME_ANIM_INJURED_RAISE_HEAD:
            if (Animation_OnFrame(&this->skelAnime, 31.0f)) {
                this->eyeTexIndex = 1;
            } else if (this->skelAnime.curFrame <= 32.0f) {
                this->eyeTexIndex = 2;
            } else {
                this->eyeTexIndex = 0;
            }
            return false;

        case KOUME_ANIM_DRINK:
            if (Animation_OnFrame(&this->skelAnime, 57.0f)) {
                this->blinkTimer = 0;
                this->eyeTexIndex = 0;
            }

            if (this->skelAnime.curFrame < 57.0f) {
                if (DECR(this->blinkTimer) == 0) {
                    this->blinkTimer = Rand_S16Offset(8, 8);
                    this->eyeTexIndex = 2;
                } else {
                    this->eyeTexIndex = 1;
                }
            } else if (DECR(this->blinkTimer) == 0) {
                this->eyeTexIndex++;
                if (this->eyeTexIndex >= 4) {
                    this->blinkTimer = Rand_S16Offset(20, 10);
                    this->eyeTexIndex = 0;
                }
            }
            return false;

        case KOUME_ANIM_FINISHED_DRINKING:
            this->eyeTexIndex = 0;
            return false;

        case KOUME_ANIM_HEALED:
            if (Animation_OnFrame(&this->skelAnime, 19.0f) || Animation_OnFrame(&this->skelAnime, 45.0f)) {
                this->eyeTexIndex = 1;
            } else if ((this->skelAnime.curFrame >= 19.0f) && (this->skelAnime.curFrame <= 45.0f)) {
                this->eyeTexIndex = 2;
            } else {
                this->eyeTexIndex = 3;
            }
            return false;

        case KOUME_ANIM_TAKE_OFF:
            if (Animation_OnFrame(&this->skelAnime, 19.0f)) {
                this->eyeTexIndex = 1;
            } else if (this->skelAnime.curFrame >= 19.0f) {
                this->eyeTexIndex = 2;
            } else {
                this->eyeTexIndex = 0;
            }
            return false;

        default:
            if (DECR(this->blinkTimer) == 0) {
                if ((this->eyeTexIndex != 2) || !(this->unk_34E & 0x80)) {
                    this->eyeTexIndex++;
                }

                if (this->eyeTexIndex >= 4) {
                    this->blinkTimer = Rand_S16Offset(30, 30);
                    this->eyeTexIndex = 0;
                }
            }
            return false;
    }

    return false;
}

UNK_TYPE* func_80A871E0(EnTru* this, PlayState* play) {
    if (this->unk_34E & 0x2000) {
        if (this->unk_38C == 35) {
            this->unk_390 = 1;
        } else {
            this->unk_390 = 2;
        }
        return D_80A88984;
    }

    if (this->unk_34E & 0x4000) {
        return D_80A889A4;
    }

    if (this->unk_34E & 0x80) {
        return D_80A88924;
    }

    if (!(this->unk_34E & 0x40) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_16_10)) {
        return D_80A88918;
    }

    if ((this->unk_34E & 0x1000) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_16_10)) {
        return D_80A88910;
    }

    return D_80A8892C;
}

s32 func_80A872AC(EnTru* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 ret = false;

    if (this->unk_34E & 7) {
        if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
            if (player->transformation == PLAYER_FORM_HUMAN) {
                this->unk_34E &= ~0x80;
            }
            this->unk_34E &= ~(0x4000 | 0x2000);

            if ((player->exchangeItemId == 35) || (player->exchangeItemId == 36)) {
                this->unk_34E |= 0x2000;
                this->unk_38C = player->exchangeItemId;
            } else if (player->exchangeItemId != 0) {
                this->unk_34E |= 0x4000;
            }

            this->unk_378 = func_80A875AC;
            this->unk_390 = 0;
            this->unk_364 = 0;
            this->unk_354 = func_80A871E0(this, play);
            SubS_UpdateFlags(&this->unk_34E, 0, 7);
            this->actionFunc = func_80A881E0;
            ret = true;
        }
    }

    return ret;
}

s32 func_80A873B8(EnTru* this) {
    s16 yDiff = this->actor.yawTowardsPlayer - this->actor.world.rot.y;
    s32 ret;

    if ((u16)ABS_ALT(yDiff) < 0x1600) {
        ret = true;
    } else {
        ret = false;
    }
    return ret;
}

s32 func_80A87400(EnTru* this, PlayState* play) {
    Vec3s* sp4C;
    Vec3f sp40;
    Vec3f sp34;
    s16 phi_a1;
    s32 ret = false;

    this->actor.velocity.y = Math_SinS(this->unk_360) * this->unk_35C;
    this->unk_360 = CLAMP(this->unk_360 + 2000, 0, 0x4000);

    Math_ApproachF(&this->unk_35C, 30.0f, 0.08f, 1000.0f);
    Math_ApproachF(&this->actor.speedXZ, 30.0f, 0.2f, 1000.0f);

    if (this->path != NULL) {
        sp4C = Lib_SegmentedToVirtual(this->path->points);
        if (SubS_HasReachedPoint(&this->actor, this->path, this->unk_384)) {
            if (this->unk_384 > this->unk_384 + 1) {
                this->unk_384 = this->path->count - 2;
                ret = true;
            }
            this->unk_384++;
        }
        Math_Vec3s_ToVec3f(&sp34, &sp4C[this->unk_384]);
        Math_Vec3f_Copy(&sp40, &this->actor.world.pos);
        phi_a1 = Math_Vec3f_Yaw(&sp40, &sp34);
    } else {
        phi_a1 = this->actor.world.rot.y;
    }

    Math_ApproachS(&this->actor.world.rot.y, phi_a1, 4, 3640);
    this->actor.shape.rot.y = this->actor.world.rot.y;
    Actor_MoveWithGravity(&this->actor);
    if ((s32)(this->actor.floorHeight + 80.0f) >= (s32)this->actor.world.pos.y) {
        func_80A86770(this);
    }

    return ret;
}

s32 func_80A875AC(Actor* thisx, PlayState* play) {
    EnTru* this = THIS;
    s32 ret = false;

    switch (this->unk_364) {
        case 0:
            if ((this->unk_34E & 0x40) || CHECK_WEEKEVENTREG(WEEKEVENTREG_16_10)) {
                this->unk_374 = this->actor.cutscene;
                this->unk_364++;
            } else {
                this->unk_364++;
                this->unk_364++;
                break;
            }

        case 1:
            if (ActorCutscene_GetCurrentIndex() == 0x7C) {
                ActorCutscene_Stop(0x7C);
                ActorCutscene_SetIntentToPlay(this->unk_374);
            } else if (ActorCutscene_GetCanPlayNext(this->unk_374)) {
                ActorCutscene_StartAndSetUnkLinkFields(this->unk_374, &this->actor);
                this->unk_364++;
            } else {
                ActorCutscene_SetIntentToPlay(this->unk_374);
            }
            break;

        case 2:
            if ((this->animIndex != KOUME_ANIM_INJURED_HEAD_UP) &&
                (this->animIndex != KOUME_ANIM_INJURED_HEAD_UP_MORPH)) {
                EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_RAISE_HEAD);
                this->unk_364++;
            } else {
                EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_TALK);
                ret = true;
            }
            break;

        case 3:
            if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                this->unk_364++;
                EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_TALK);
                ret = true;
            }
            break;
    }

    if (ret == true) {
        if (this->unk_390 != 0) {
            this->unk_34E |= 8;
            this->unk_378 = func_80A87880;
        } else {
            this->unk_378 = func_80A8777C;
        }
        this->unk_364 = 0;
    }

    return ret;
}

s32 func_80A8777C(Actor* thisx, PlayState* play) {
    EnTru* this = THIS;
    s32 ret = 0;
    PlayerItemAction itemAction;

    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_CHOICE:
        case TEXT_STATE_5:
            if (!Message_ShouldAdvance(play)) {
                break;
            }
        // Fallthrough
        case TEXT_STATE_16:
            itemAction = func_80123810(play);

            if ((itemAction == PLAYER_IA_BOTTLE_POTION_RED) || (itemAction == PLAYER_IA_BOTTLE_POTION_BLUE)) {
                this->unk_34E |= 8;
                if (itemAction == PLAYER_IA_BOTTLE_POTION_RED) {
                    this->unk_390 = 1;
                } else {
                    this->unk_390 = 2;
                }
                this->unk_378 = func_80A87880;
                this->unk_364 = 0;
                ret = 1;
            } else if (itemAction <= PLAYER_IA_MINUS1) {
                ret = 3;
            } else if (itemAction != PLAYER_IA_NONE) {
                ret = 2;
            }
            break;
    }

    return ret;
}

s32 func_80A87880(Actor* thisx, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnTru* this = THIS;
    s32 ret = false;

    switch (this->unk_364) {
        case 0:
            ActorCutscene_Stop(this->unk_374);
            this->unk_374 = ActorCutscene_GetAdditionalCutscene(this->unk_374);
            this->unk_364++;

        case 1:
            if (ActorCutscene_GetCurrentIndex() == 0x7C) {
                ActorCutscene_Stop(0x7C);
                ActorCutscene_SetIntentToPlay(this->unk_374);
            } else if (ActorCutscene_GetCanPlayNext(this->unk_374)) {
                ActorCutscene_StartAndSetUnkLinkFields(this->unk_374, &this->actor);
                this->unk_364++;
            } else {
                ActorCutscene_SetIntentToPlay(this->unk_374);
            }
            break;

        case 2:
            this->unk_34E |= 0x10;
            this->unk_34E &= ~0x8;
            this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
            this->actor.world.rot.y = this->actor.yawTowardsPlayer;
            EnTru_ChangeAnim(this, KOUME_ANIM_TAKE);
            this->unk_364++;
            break;

        case 3:
            if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                this->unk_364++;
                EnTru_ChangeAnim(this, KOUME_ANIM_DRINK);
                this->actor.world.rot.y += 0x4000;
            } else if (Animation_OnFrame(&this->skelAnime, 12.0f) && !(this->unk_34E & 0x800)) {
                this->unk_34E |= 0x400;
                this->unk_34E |= 0x800;
                Player_SetModels(player, PLAYER_MODELGROUP_DEFAULT);
            }
            break;

        case 4:
            if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                this->unk_364++;
                EnTru_ChangeAnim(this, KOUME_ANIM_FINISHED_DRINKING);
                ret = true;
            } else if (Animation_OnFrame(&this->skelAnime, 18.0f) || Animation_OnFrame(&this->skelAnime, 32.0f) ||
                       Animation_OnFrame(&this->skelAnime, 52.0f)) {
                if (Animation_OnFrame(&this->skelAnime, 52.0f)) {
                    this->unk_34E &= ~0x400;
                    Player_UpdateBottleHeld(play, player, ITEM_BOTTLE, PLAYER_IA_BOTTLE);
                }
                Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_KOUME_DRINK);
            } else if (Animation_OnFrame(&this->skelAnime, 90.0f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_KOUME_REGAIN);
            }

            if ((this->skelAnime.curFrame > 90.0f) && (this->skelAnime.curFrame < 95.0f)) {
                func_80A86674(this);
            }
            break;
    }

    if (ret == true) {
        this->unk_378 = func_80A87B48;
        this->unk_364 = 0;
    }

    return ret;
}

s32 func_80A87B48(Actor* thisx, PlayState* play) {
    EnTru* this = THIS;
    Player* player = GET_PLAYER(play);
    Vec3f sp4C;
    Vec3f sp40;
    s16 sp3E;
    s32 pad2;
    s32 ret = false;

    switch (this->unk_364) {
        case 0:
            EnTru_ChangeAnim(this, KOUME_ANIM_HEALED);
            this->unk_364++;
            break;

        case 1:
            if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                sp3E = BINANG_ROT180(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)));
                Math_Vec3f_Copy(&sp4C, &gZeroVec3f);
                sp4C.z = 40.0f;
                Lib_Vec3f_TranslateAndRotateY(&this->actor.world.pos, sp3E, &sp4C, &sp40);
                func_80A85620(this->unk_394, &sp40, 2.0f, 0.08f, 60.0f);
                Play_FillScreen(&play->state, true, 160, 160, 160, 0);
                this->unk_370 = 20;
                this->unk_372 = 10;
                this->unk_364++;
            } else if (Animation_OnFrame(&this->skelAnime, 22.0f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_KOUME_MAGIC);
                func_80A85AA4(this->unk_394, &this->unk_1F8, 1.0f, 0.1f, 40.0f);
            }
            break;

        case 2:
            if (DECR(this->unk_370) != 0) {
                R_PLAY_FILL_SCREEN_ALPHA = 255.0f - ((fabsf(10.0f - this->unk_370) / 10) * 255.0f);
                if (this->unk_370 == 9) {
                    this->actor.shape.shadowDraw = NULL;
                    this->unk_34E |= (0x200 | 0x8);
                    this->unk_34E &= ~0x800;
                    if (player->exchangeItemId != 0) {
                        player->exchangeItemId = 0;
                    }
                    EnTru_ChangeAnim(this, KOUME_ANIM_HOVER1);
                }
            } else {
                R_PLAY_FILL_SCREEN_ON = false;
                ret = true;
            }
            break;
    }

    if (ret == true) {
        this->unk_378 = func_80A87DC0;
        this->unk_364 = 0;
    }

    return ret;
}

s32 func_80A87DC0(Actor* thisx, PlayState* play) {
    EnTru* this = THIS;
    s32 ret = false;

    switch (this->unk_364) {
        case 0:
            ActorCutscene_Stop(this->unk_374);
            this->unk_374 = ActorCutscene_GetAdditionalCutscene(this->unk_374);
            this->unk_364++;

        case 1:
            if (ActorCutscene_GetCurrentIndex() == 0x7C) {
                ActorCutscene_Stop(0x7C);
                ActorCutscene_SetIntentToPlay(this->unk_374);
            } else if (ActorCutscene_GetCanPlayNext(this->unk_374)) {
                ActorCutscene_StartAndSetUnkLinkFields(this->unk_374, &this->actor);
                this->unk_364++;
            } else {
                ActorCutscene_SetIntentToPlay(this->unk_374);
            }
            break;

        case 2:
            AudioSfx_StopById(NA_SE_EN_KOUME_MAGIC);
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_KOUME_AWAY);
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_KOUME_LAUGH);
            EnTru_ChangeAnim(this, KOUME_ANIM_TAKE_OFF);
            this->skelAnime.baseTransl.y = 0;
            this->skelAnime.moveFlags = 2;
            this->unk_34E &= ~0x8;
            this->unk_34E |= 0x10;
            this->unk_364++;
            break;

        case 3:
            if (!Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                AnimationContext_SetMoveActor(play, &this->actor, &this->skelAnime, 1.0f);
                break;
            } else {
                EnTru_ChangeAnim(this, KOUME_ANIM_FLY);
                this->actor.shape.rot.y = this->actor.world.rot.y;
                this->unk_362 = 20;
                this->unk_364++;
            }

        case 4:
            if (func_80A87400(this, play) || (DECR(this->unk_362) == 0)) {
                ret = true;
                SET_WEEKEVENTREG(WEEKEVENTREG_12_08);
            }
            break;
    }

    if (ret == true) {
        this->actor.flags &= ~ACTOR_FLAG_1;
        this->actor.draw = NULL;
        this->unk_378 = NULL;
        this->unk_34E = 0;
        this->unk_364 = 0;
    }
    return ret;
}

void func_80A87FD0(EnTru* this, PlayState* play) {
    if (this->actor.draw != NULL) {
        if ((this->unk_34E & 0x80) || CHECK_WEEKEVENTREG(WEEKEVENTREG_16_10)) {
            if (func_80A873B8(this)) {
                SubS_UpdateFlags(&this->unk_34E, 3, 7);
            } else {
                SubS_UpdateFlags(&this->unk_34E, 0, 7);
            }
        } else if (this->unk_34E & 0x40) {
            if (func_80A873B8(this)) {
                SubS_UpdateFlags(&this->unk_34E, 3, 7);
            } else {
                SubS_UpdateFlags(&this->unk_34E, 0, 7);
            }

            if ((this->animIndex == KOUME_ANIM_TRY_GET_UP) &&
                Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                this->unk_362 = Rand_S16Offset(40, 20);
                EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_LYING_DOWN_MORPH);
                func_80A86460(this);
            } else if (this->animIndex != KOUME_ANIM_TRY_GET_UP) {
                if (DECR(this->unk_362) == 0) {
                    EnTru_ChangeAnim(this, KOUME_ANIM_TRY_GET_UP);
                }
            }
        } else if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_16_10) && (fabsf(this->actor.playerHeightRel) < 10.0f) &&
                   (this->actor.xzDistToPlayer < 140.0f)) {
            SubS_UpdateFlags(&this->unk_34E, 4, 7);
            this->unk_34E |= 0x1040;
            this->unk_362 = Rand_S16Offset(40, 20);
        } else if (func_80A873B8(this)) {
            SubS_UpdateFlags(&this->unk_34E, 3, 7);
        } else {
            SubS_UpdateFlags(&this->unk_34E, 0, 7);
        }
    }
}

void func_80A881E0(EnTru* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (func_8010BF58(&this->actor, play, this->unk_354, this->unk_378, &this->unk_1E8)) {
        if (player->transformation != PLAYER_FORM_HUMAN) {
            this->unk_34E |= 0x80;
        }

        if (ActorCutscene_GetCurrentIndex() != -1) {
            ActorCutscene_Stop(ActorCutscene_GetCurrentIndex());
        }

        if (!(this->unk_34E & 0x40) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_16_10)) {
            EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_LYING_DOWN);
        } else if (this->unk_34E & 0x80) {
            EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_LYING_DOWN);
            func_80A86460(this);
        } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_16_10)) {
            EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_HEAD_UP_MORPH);
        }

        SubS_UpdateFlags(&this->unk_34E, 0, 7);
        this->unk_34E &= ~(0x1000 | 0x8);
        this->unk_34E |= 0x10;
        this->actor.shape.rot.y = this->actor.world.rot.y;
        this->actor.flags &= ~ACTOR_FLAG_100;
        this->unk_1E8 = 0;
        this->actionFunc = func_80A87FD0;
    }
}

void EnTru_Init(Actor* thisx, PlayState* play) {
    EnTru* this = THIS;

    if ((gSaveContext.save.entrance != ENTRANCE(WOODS_OF_MYSTERY, 0)) || CHECK_WEEKEVENTREG(WEEKEVENTREG_12_08)) {
        Actor_Kill(&this->actor);
        return;
    }

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gKoumeSkel, NULL, this->jointTable, this->morphTable, KOUME_LIMB_MAX);
    Collider_InitAndSetSphere(play, &this->collider, &this->actor, &sSphereInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, DamageTable_Get(0x16), &sColChkInfoInit);
    this->animIndex = -1;
    EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_LYING_DOWN);
    this->path = SubS_GetDayDependentPath(play, ENTRU_GET_PATH(&this->actor), 255, &this->unk_384);
    if (this->path != NULL) {
        this->unk_384 = 1;
    }

    this->actor.targetMode = 0;
    Actor_SetScale(&this->actor, 0.008f);
    this->unk_34E = 0;

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_16_10)) {
        EnTru_ChangeAnim(this, KOUME_ANIM_INJURED_HEAD_UP);
    } else {
        this->unk_388 = PLAYER_IA_NONE;
    }

    this->actionFunc = func_80A87FD0;
    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, 4);
}

void EnTru_Destroy(Actor* thisx, PlayState* play) {
    EnTru* this = THIS;

    Collider_DestroySphere(play, &this->collider);
}

void EnTru_Update(Actor* thisx, PlayState* play) {
    EnTru* this = THIS;
    f32 radius;

    func_80A872AC(this, play);

    this->actionFunc(this, play);

    EnTru_UpdateSkelAnime(this);
    func_80A86B0C(this, play);
    func_80A86DB8(this);

    radius = this->collider.dim.worldSphere.radius + 30;
    this->unk_388 = !(this->unk_34E & 0x80) ? PLAYER_IA_NONE : PLAYER_IA_NONE;

    func_8013C964(&this->actor, play, radius, 20.0f, this->unk_388, this->unk_34E & 7);
    func_80A8697C(this, play);
    func_80A86384(this->unk_394, &this->unk_1F8);
}

s32 EnTru_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    s32 pad;
    EnTru* this = THIS;

    if (limbIndex == KOUME_LIMB_HEAD) {
        Matrix_MultZero(&this->actor.focus.pos);
        Math_Vec3f_ToVec3s(&this->collider.dim.worldSphere.center, &this->actor.focus.pos);
        this->actor.focus.pos.x = (this->actor.focus.pos.x / 10.0f) * 10.0f;
        this->actor.focus.pos.y = ((this->actor.focus.pos.y + 10.0f) / 10.0f) * 10.0f;
        this->actor.focus.pos.z = (this->actor.focus.pos.z / 10.0f) * 10.0f;
        Math_Vec3s_Copy(&this->actor.focus.rot, &this->actor.world.rot);
        Matrix_MultVec3f(&D_80A8B3FC, &this->unk_1F8);
    }

    if (!(this->unk_34E & 0x200) && (limbIndex == KOUME_LIMB_BROOM)) {
        *dList = NULL;
    }

    return false;
}

void EnTru_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnTru* this = THIS;

    if (limbIndex == KOUME_LIMB_RIGHT_HAND) {
        func_80A86BAC(this, play);
    }
}

void EnTru_TransformLimbDraw(PlayState* play, s32 limbIndex, Actor* thisx) {
    EnTru* this = THIS;
    s32 pad[3];
    s32 overrideRot;
    s32 stepRot;

    if (this->unk_34E & 0x10) {
        stepRot = false;
    } else {
        stepRot = true;
    }

    if (this->unk_34E & 0x20) {
        overrideRot = true;
    } else {
        overrideRot = false;
    }

    if (!stepRot) {
        overrideRot = false;
    }

    if (limbIndex == KOUME_LIMB_HEAD) {
        SubS_UpdateLimb(this->unk_366, this->unk_368 + this->actor.shape.rot.y, &this->unk_1EC, &this->unk_204, stepRot,
                        overrideRot);
        Matrix_Pop();
        Matrix_Translate(this->unk_1EC.x, this->unk_1EC.y, this->unk_1EC.z, MTXMODE_NEW);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
        if (overrideRot) {
            s16 oldZ = this->unk_204.z;

            this->unk_204.z = this->unk_204.x;
            this->unk_204.x = oldZ;
        }
        Matrix_RotateYS(this->unk_204.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->unk_204.x, MTXMODE_APPLY);
        Matrix_RotateZS(this->unk_204.z, MTXMODE_APPLY);
        Matrix_Push();
    }
}

void EnTru_Draw(Actor* thisx, PlayState* play) {
    static TexturePtr sEyeTextures[] = {
        gKoumeEyeOpenTex,
        gKoumeEyeHalfTex,
        gKoumeEyeClosedTex,
        gKoumeEyeHalfTex,
    };
    s32 pad;
    EnTru* this = THIS;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C28C(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(sEyeTextures[this->eyeTexIndex]));
    gSPSegment(POLY_OPA_DISP++, 0x09, Lib_SegmentedToVirtual(sEyeTextures[this->eyeTexIndex]));

    SkelAnime_DrawTransformFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                   this->skelAnime.dListCount, EnTru_OverrideLimbDraw, EnTru_PostLimbDraw,
                                   EnTru_TransformLimbDraw, &this->actor);
    func_80A85788(this->unk_394, play);
    func_80A85BCC(this->unk_394, play);
    func_80A85F84(this->unk_394, play);

    CLOSE_DISPS(play->state.gfxCtx);
}
