/*
 * File: z_boss_02.c
 * Overlay: ovl_Boss_02
 * Description: Twinmold
 */

#include "prevent_bss_reordering.h"
#include "z_boss_02.h"
#include "z64rumble.h"
#include "z64shrink_window.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "overlays/actors/ovl_Item_B_Heart/z_item_b_heart.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_4 | ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((Boss02*)thisx)

void Boss02_Init(Actor* thisx, PlayState* play);
void Boss02_Destroy(Actor* thisx, PlayState* play);
void Boss02_Twinmold_Update(Actor* thisx, PlayState* play);
void Boss02_Twinmold_Draw(Actor* thisx, PlayState* play2);

void func_809DAA74(Boss02* this, PlayState* play);
void func_809DAA98(Boss02* this, PlayState* play);
void func_809DAAA8(Boss02* this, PlayState* play);
void func_809DAB78(Boss02* this, PlayState* play);
void Boss02_Tail_Update(Actor* thisx, PlayState* play);
void Boss02_Static_Update(Actor* thisx, PlayState* play);
void Boss02_Static_Draw(Actor* thisx, PlayState* play);
void Boss02_UpdateEffects(PlayState* play);
void Boss02_DrawEffects(PlayState* play);
void func_809DD934(Boss02* this, PlayState* play);
void func_809DEAC4(Boss02* this, PlayState* play);

u8 D_809E0420;
u8 D_809E0421;
u8 sIsInGiantMode;
Boss02* sRedTwinmold;
Boss02* sBlueTwinmold;
Boss02* sTwinmoldStatic;
u8 sMusicStartTimer;
DoorWarp1* sBlueWarp;
TwinmoldEffect sEffects[150];

static DamageTable sBlueTwinmoldDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(1, 0xF),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0xF),
    /* Zora boomerang */ DMG_ENTRY(0, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0xF),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(0, 0x0),
    /* Goron punch    */ DMG_ENTRY(2, 0xF),
    /* Sword          */ DMG_ENTRY(1, 0xF),
    /* Goron pound    */ DMG_ENTRY(0, 0xF),
    /* Fire arrow     */ DMG_ENTRY(5, 0x2),
    /* Ice arrow      */ DMG_ENTRY(1, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Goron spikes   */ DMG_ENTRY(1, 0xF),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(0, 0x0),
    /* Deku launch    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(0, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0xF),
    /* Zora punch     */ DMG_ENTRY(1, 0xF),
    /* Spin attack    */ DMG_ENTRY(1, 0xF),
    /* Sword beam     */ DMG_ENTRY(2, 0xF),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0xF),
};

static DamageTable sRedTwinmoldDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(1, 0xF),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0xF),
    /* Zora boomerang */ DMG_ENTRY(0, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0xF),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(0, 0x0),
    /* Goron punch    */ DMG_ENTRY(2, 0xF),
    /* Sword          */ DMG_ENTRY(1, 0xF),
    /* Goron pound    */ DMG_ENTRY(0, 0xF),
    /* Fire arrow     */ DMG_ENTRY(1, 0x2),
    /* Ice arrow      */ DMG_ENTRY(5, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Goron spikes   */ DMG_ENTRY(1, 0xF),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(0, 0x0),
    /* Deku launch    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(0, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0xF),
    /* Zora punch     */ DMG_ENTRY(1, 0xF),
    /* Spin attack    */ DMG_ENTRY(1, 0xF),
    /* Sword beam     */ DMG_ENTRY(2, 0xF),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0xF),
};

ActorInit Boss_02_InitVars = {
    ACTOR_BOSS_02,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_BOSS02,
    sizeof(Boss02),
    (ActorFunc)Boss02_Init,
    (ActorFunc)Boss02_Destroy,
    (ActorFunc)Boss02_Twinmold_Update,
    (ActorFunc)Boss02_Twinmold_Draw,
};

f32 D_809DF5B0 = 1.0f;

s16 D_809DF5B4[] = {
    0, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 0,
};

s16 D_809DF5E4[] = {
    0, 196, 192, 188, 184, 180, 176, 172, 168, 164, 160, 156, 152, 148, 144, 140, 136, 132, 128, 124, 120, 116, 112,
};

static ColliderJntSphElementInit sJntSphElementsInit1[] = {
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 2, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 3, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 4, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 5, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 6, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 7, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 8, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 9, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 10, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 11, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 12, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 13, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 14, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 15, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 16, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 17, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 18, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 19, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 20, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 21, { { 0, 0, 0 }, 85 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 130 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit1 = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit1),
    sJntSphElementsInit1,
};

static ColliderJntSphElementInit sJntSphElementsInit2[] = {
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 130 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK3,
            { 0xF7CFFFFF, 0x04, 0x10 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 22, { { 0, 0, 0 }, 110 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit2 = {
    {
        COLTYPE_HIT0,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit2),
    sJntSphElementsInit2,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0xF7CFFFFF, 0x04, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 150, 200, 0, { 0, 0, 0 } },
};

Vec3f D_809DF9C0[] = {
    { 0.0f, -200.0f, 1000.0f },    { 0.0f, 500.0f, 1000.0f },        { 0.0f, 1000.0f, 1000.0f },
    { 1000.0f, 500.0f, 1000.0f },  { 1000.0f, 1000.0f, -1000.0f },   { -1000.0f, 500.0f, -1000.0f },
    { -1000.0f, 500.0f, 1000.0f }, { -1000.0f, -1000.0f, -1000.0f }, { -1000.0f, -1000.0f, -1000.0f },
};

Vec3f D_809DFA2C[] = {
    { 0.0f, -200.0f, -800.0f },  { 0.0f, 800.0f, -800.0f },   { 800.0f, 300.0f, -800.0f },
    { -800.0f, 800.0f, 0.0f },   { -800.0f, -1000.0f, 0.0f }, { -800.0f, -1000.0f, 0.0f },
    { -800.0f, -1000.0f, 0.0f }, { -800.0f, -1000.0f, 0.0f }, { -800.0f, -1000.0f, 0.0f },
};

void func_809DA1D0(PlayState* play, u8 red, u8 green, u8 blue, u8 alpha) {
    R_PLAY_FILL_SCREEN_ON = true;
    R_PLAY_FILL_SCREEN_R = red;
    R_PLAY_FILL_SCREEN_G = green;
    R_PLAY_FILL_SCREEN_B = blue;
    R_PLAY_FILL_SCREEN_ALPHA = alpha;
}

void func_809DA22C(PlayState* play, u8 alpha) {
    R_PLAY_FILL_SCREEN_ALPHA = alpha;
}

void func_809DA24C(PlayState* play) {
    R_PLAY_FILL_SCREEN_ON = false;
}

void Boss02_SpawnEffectSand(TwinmoldEffect* effects, Vec3f* pos, f32 scale) {
    s16 i;

    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effects++) {
        if (effects->type == TWINMOLD_EFFECT_NONE) {
            effects->type = TWINMOLD_EFFECT_SAND;
            effects->pos = *pos;
            effects->velocity.x = randPlusMinusPoint5Scaled(30.0f);
            effects->velocity.y = Rand_ZeroFloat(7.0f) + 7.0f;
            effects->velocity.z = randPlusMinusPoint5Scaled(30.0f);
            effects->accel.y = -0.3f;
            effects->scale = scale;
            effects->alpha = 255;
            effects->timer = 0;
            effects->targetScale = 2.0f * scale;
            effects->accel.x = effects->accel.z = 0.0f;
            break;
        }
    }
}

void Boss02_SpawnEffectFragment(TwinmoldEffect* effects, Vec3f* pos) {
    s16 i;

    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effects++) {
        if (effects->type == TWINMOLD_EFFECT_NONE) {
            effects->type = TWINMOLD_EFFECT_FRAGMENT;
            effects->pos = *pos;
            effects->timer = Rand_ZeroFloat(20.0f);
            effects->velocity.x = randPlusMinusPoint5Scaled(50.0f);
            effects->velocity.y = randPlusMinusPoint5Scaled(50.0f);
            effects->velocity.z = randPlusMinusPoint5Scaled(50.0f);
            effects->accel.z = 0.0f;
            effects->accel.x = 0.0f;
            effects->accel.y = -1.5f;
            effects->scale = Rand_ZeroFloat(0.04f) + 0.02f;
            effects->rotY = Rand_ZeroFloat(32767.0f);
            effects->rotX = Rand_ZeroFloat(32767.0f);
            break;
        }
    }
}

void Boss02_SpawnEffectFlash(TwinmoldEffect* effects, Vec3f* pos) {
    s16 i;

    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effects++) {
        if ((effects->type == TWINMOLD_EFFECT_NONE) || (effects->type == TWINMOLD_EFFECT_FRAGMENT)) {
            effects->type = TWINMOLD_EFFECT_FLASH;
            effects->pos = *pos;
            Math_Vec3f_Copy(&effects->velocity, &gZeroVec3f);
            Math_Vec3f_Copy(&effects->accel, &gZeroVec3f);
            effects->alpha = 255;
            effects->scale = 0.0f;
            break;
        }
    }
}

void func_809DA50C(s32 arg0, ColliderJntSph* collider, Vec3f* arg2) {
    collider->elements[arg0].dim.worldSphere.center.x = arg2->x;
    collider->elements[arg0].dim.worldSphere.center.y = arg2->y;
    collider->elements[arg0].dim.worldSphere.center.z = arg2->z;
    collider->elements[arg0].dim.worldSphere.radius =
        collider->elements[arg0].dim.modelSphere.radius * collider->elements[arg0].dim.scale * D_809DF5B0;
}

void Boss02_Init(Actor* thisx, PlayState* play) {
    Boss02* this = THIS;
    s32 i;
    s32 pad[2];

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_52_20) && (this->actor.params == TWINMOLD_RED)) {
        sBlueWarp = (DoorWarp1*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DOOR_WARP1, 0.0f, 60.0f,
                                                   0.0f, 0, 0, 0, 1);
        Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_B_HEART, 0.0f, 30.0f, -150.0f, 0, 1, 0, 0);
    }

    this->actor.targetMode = 10;
    this->subCamUp.z = this->subCamUp.x = 0.0f;
    this->subCamUp.y = 1.0f;
    if (this->actor.params == TWINMOLD_STATIC) {
        sTwinmoldStatic = this;
        play->specialEffects = (void*)sEffects;
        this->actor.update = Boss02_Static_Update;
        this->actor.draw = Boss02_Static_Draw;
        this->actor.flags &= ~ACTOR_FLAG_1;
        this->unk_1D70 = 0.00999999977648f;
        if ((KREG(64) != 0) || CHECK_EVENTINF(EVENTINF_55) || (sBlueWarp != NULL)) {
            this->unk_1D20 = 0;
            sMusicStartTimer = KREG(15) + 20;
        } else {
            this->unk_1D20 = 1;
        }
        R_MAGIC_CONSUME_TIMER_GIANTS_MASK = KREG(14) + 20;
        this->unk_01AC = 1.0f;
        Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_TANRON5, 0.0f, 1000.0f, 0.0f, 0, 0, 0, 0);
    } else if (this->actor.params == TWINMOLD_TAIL) {
        this->actor.update = Boss02_Tail_Update;
        this->actor.draw = NULL;
        this->actor.hintId = TATL_HINT_ID_TWINMOLD;
    } else {
        if (this->actor.params != TWINMOLD_BLUE) {
            this->actor.params = TWINMOLD_RED;
            Actor_Spawn(&play->actorCtx, play, ACTOR_BOSS_02, 0.0f, 0.0f, 0.0f, 0, 0, 0, TWINMOLD_STATIC);
            sRedTwinmold = this;
            sBlueTwinmold =
                (Boss02*)Actor_Spawn(&play->actorCtx, play, ACTOR_BOSS_02, this->actor.world.pos.x,
                                     this->actor.world.pos.y, this->actor.world.pos.z, this->actor.world.rot.x,
                                     this->actor.world.rot.y, this->actor.world.rot.z, TWINMOLD_BLUE);
            sRedTwinmold->actor.colChkInfo.damageTable = &sRedTwinmoldDamageTable;
            sBlueTwinmold->actor.colChkInfo.damageTable = &sBlueTwinmoldDamageTable;
            sRedTwinmold->otherTwinmold = sBlueTwinmold;
            sBlueTwinmold->otherTwinmold = sRedTwinmold;
        }

        this->actor.colChkInfo.mass = MASS_HEAVY;
        this->actor.colChkInfo.health = 20;
        Actor_SetScale(&this->actor, 0.01f);
        SkelAnime_Init(play, &this->skelAnime, &gTwinmoldHeadSkel, &gTwinmoldHeadFlyAnim, this->jointTable,
                       this->morphTable, TWINMOLD_HEAD_LIMB_MAX);
        Collider_InitAndSetJntSph(play, &this->colliderSphere1, &this->actor, &sJntSphInit1,
                                  this->colliderSphere1Elements);
        Collider_InitAndSetJntSph(play, &this->colliderSphere2, &this->actor, &sJntSphInit2,
                                  this->colliderSphere2Elements);
        Collider_InitAndSetCylinder(play, &this->colliderCylinder, &this->actor, &sCylinderInit);

        if (sBlueWarp != NULL) {
            func_809DAA74(this, play);
        } else {
            func_809DAAA8(this, play);
        }

        for (i = 0; i < ARRAY_COUNT(this->unk_01BC); i++) {
            this->unk_01BC[i].x = this->actor.world.pos.x;
            this->unk_01BC[i].y = this->actor.world.pos.y;
            this->unk_01BC[i].z = this->actor.world.pos.z;
        }

        this->unk_014C = Rand_ZeroFloat(1000.0f);
        this->unk_1678 = 22;
        Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_BOSS_02, 0.0f, 0.0f, 0.0f, 0, 0, 0,
                           TWINMOLD_TAIL);
    }
}

void Boss02_Destroy(Actor* thisx, PlayState* play) {
}

void func_809DAA74(Boss02* this, PlayState* play) {
    this->actionFunc = func_809DAA98;
    this->actor.world.pos.y = -3000.0f;
}

void func_809DAA98(Boss02* this, PlayState* play) {
}

void func_809DAAA8(Boss02* this, PlayState* play) {
    this->actionFunc = func_809DAB78;
    Animation_MorphToLoop(&this->skelAnime, &gTwinmoldHeadFlyAnim, 0.0f);
    if (sTwinmoldStatic->unk_1D20 != 0) {
        this->unk_0144 = 10;
    } else {
        this->unk_0144 = 100;
        this->unk_01A8 = 25.0f;
        sTwinmoldStatic->unk_1D7E = 100;
        this->actor.world.pos.x = 0.0f;
        if (sRedTwinmold == this) {
            this->actor.world.pos.z = -1000.0f;
        } else {
            this->actor.world.pos.z = 1400.0f;
        }
    }
    this->actor.world.pos.y = -500.0f;
}

Color_RGBA8 D_809DFA98 = { 185, 140, 70, 255 };

void func_809DAB78(Boss02* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);
    CollisionPoly* spDC;
    Vec3f spD0;
    f32 spCC;
    f32 spC8;
    f32 spC4;
    s32 i;
    f32 temp_f0;
    f32 phi_f2;
    s16 temp_s0;
    s16 temp_s2;
    Boss02* otherTwinmold = this->otherTwinmold;
    Vec3f spA4;
    f32 spA0;
    f32 sp9C;
    Vec3f sp90;
    CollisionPoly* sp8C;
    s32 sp88;
    Vec3f sp7C;
    Vec3f sp70;
    Vec3f sp64;

    spCC = this->unk_01B0.x - this->actor.world.pos.x;
    spC8 = this->unk_01B0.y - this->actor.world.pos.y;
    spC4 = this->unk_01B0.z - this->actor.world.pos.z;

    if ((this->unk_0144 != 10) && (this->unk_0144 <= 20)) {
        SkelAnime_Update(&this->skelAnime);
        temp_s0 = Math_Atan2S(spCC, spC4);
        temp_s2 = Math_Atan2S(spC8, sqrtf(SQ(spCC) + SQ(spC4)));
        Math_ApproachS(&this->actor.world.rot.y, temp_s0, 0x14, this->unk_0164);
        Math_ApproachS(&this->actor.shape.rot.x, temp_s2, 0x14, this->unk_0164);
        Math_ApproachS(&this->unk_0198, this->unk_019A, 1, 0x20);
        this->unk_0196 += this->unk_0198;
        Math_ApproachF(&this->unk_019C, this->unk_01A0, 0.1f, 100.0f);
        this->unk_01A4 = Math_SinS(this->unk_0196) * this->unk_019C;
        this->actor.world.rot.x = this->actor.shape.rot.x + this->unk_01A4;

        if (!(this->unk_014C & 0x1F) && (sTwinmoldStatic->unk_1D20 == 0)) {
            this->unk_01A0 = Rand_ZeroFloat(0x1000) + 0x800;
            this->unk_019A = Rand_ZeroFloat(0x400) + 0x200;
        }

        if (this->unk_0195 != 0) {
            Math_ApproachF(&this->unk_0164, this->unk_0168 * 1.25f, 1.0f, 62.5f);
        } else {
            Math_ApproachF(&this->unk_0164, this->unk_0168, 1.0f, 50.0f);
        }

        this->unk_0168 = 2000.0f;
        if (this->unk_0195 != 0) {
            this->actor.speedXZ = this->unk_01A8 * D_809DF5B0 * 1.25f;
            this->skelAnime.playSpeed = 2.0f;
        } else {
            this->actor.speedXZ = this->unk_01A8 * D_809DF5B0;
        }

        Actor_UpdateVelocityWithoutGravity(&this->actor);
        Actor_UpdatePos(&this->actor);

        spD0 = this->actor.world.pos;
        if (sIsInGiantMode) {
            spD0.y = 5000.0f;
        } else {
            spD0.y = 2000.0f;
        }

        temp_f0 = BgCheck_EntityRaycastFloor1(&play->colCtx, &spDC, &spD0);
        if (((this->unk_017C.y < temp_f0) && (temp_f0 <= this->unk_0188.y)) ||
            ((temp_f0 < this->unk_017C.y) && (this->unk_0188.y <= temp_f0))) {
            this->unk_0170 = this->unk_017C;
            this->unk_0170.y = temp_f0;
            this->unk_016C = 120;
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_INBOSS_ROAR_OLD);
        }

        this->actor.flags &= ~ACTOR_FLAG_1;
        if (this->unk_0195 != 0) {
            this->actor.world.rot.z = Math_SinS(this->unk_014C * 0x1200) * 0xE00;
        } else {
            this->actor.world.rot.z = Math_SinS(this->unk_014C * 0xC00) * 0xE00;
        }

        this->unk_014E++;
        if (this->unk_014E >= ARRAY_COUNT(this->unk_01BC)) {
            this->unk_014E = 0;
        }

        this->unk_01BC[this->unk_014E].x = this->actor.world.pos.x;
        this->unk_01BC[this->unk_014E].y = this->actor.world.pos.y;
        this->unk_01BC[this->unk_014E].z = this->actor.world.pos.z;

        this->unk_0B1C[this->unk_014E].x = BINANG_TO_RAD(this->actor.world.rot.x);
        this->unk_0B1C[this->unk_014E].y = BINANG_TO_RAD(this->actor.world.rot.y);
        this->unk_0B1C[this->unk_014E].z = BINANG_TO_RAD(this->actor.world.rot.z);
    }

    if ((this->unk_0144 < 10) && (otherTwinmold->unk_0144 >= 20)) {
        this->unk_01B0.y = -1000.0f * D_809DF5B0;
        if (sIsInGiantMode) {
            this->unk_01B0.y += 3150.0f;
        }
        this->unk_0144 = 3;
        this->unk_0146[0] = 50;
    }

    switch (this->unk_0144) {
        default:
            return;

        case 0:
            if ((this->actor.params == 0) && (Rand_ZeroOne() < 0.75f)) {
                this->actor.world.pos.x = player->actor.world.pos.x;
                this->actor.world.pos.z = player->actor.world.pos.z;
                this->actor.world.pos.y = player->actor.world.pos.y - (600.0f * D_809DF5B0);
            } else {
                this->actor.world.pos.x = randPlusMinusPoint5Scaled(5000.0f * D_809DF5B0);
                this->actor.world.pos.z = randPlusMinusPoint5Scaled(5000.0f * D_809DF5B0);
                this->actor.world.pos.y = -500.0f * D_809DF5B0;
                if (sIsInGiantMode) {
                    this->actor.world.pos.y += 3150.0f;
                }
            }

            if ((fabsf(this->actor.world.pos.x) < (500.0f * D_809DF5B0)) &&
                (fabsf(this->actor.world.pos.z) < (500.0f * D_809DF5B0))) {
                return;
            }

        case 100:
            this->actor.shape.rot.x = 0x4000;
            this->unk_01B0.x = this->actor.world.pos.x;
            this->unk_01B0.y = this->actor.world.pos.y + (1000.0f * D_809DF5B0);
            this->unk_01B0.z = this->actor.world.pos.z;
            this->unk_0146[0] = 100;
            this->unk_0144 = 1;

        case 1:
            if (this->unk_0146[0] == 0) {
                if (Rand_ZeroOne() < 0.3f) {
                    this->unk_0144 = 5;
                    this->unk_0146[0] = 150;
                } else {
                    this->unk_0144 = 2;
                    this->unk_01B0.x = randPlusMinusPoint5Scaled(3000.0f * D_809DF5B0);
                    this->unk_01B0.z = randPlusMinusPoint5Scaled(3000.0f * D_809DF5B0);
                    if ((fabsf(this->unk_01B0.x) < (500.0f * D_809DF5B0)) &&
                        (fabsf(this->unk_01B0.z) < (500.0f * D_809DF5B0))) {
                        this->unk_01B0.x = 500.0f;
                        this->unk_01B0.z = 500.0f;
                    }
                    this->unk_01B0.y = Rand_ZeroFloat(800.0f * D_809DF5B0) + (200.0f * D_809DF5B0);
                    if (sIsInGiantMode) {
                        this->unk_01B0.y += 3150.0f;
                    }
                }
                this->unk_0164 = 0.0f;
            }
            return;

        case 2:
            if (this->unk_0195 != 0) {
                phi_f2 = 700.0f;
            } else {
                phi_f2 = 500.0f;
            }
            if (sqrtf(SQ(spCC) + SQ(spC8) + SQ(spC4)) < (phi_f2 * D_809DF5B0)) {
                this->unk_0144 = 3;
                this->unk_01B0.y = -3000.0f * D_809DF5B0;
                if (sIsInGiantMode) {
                    this->unk_01B0.y += 3150.0f;
                }
                this->unk_0146[0] = 150;
                this->unk_0164 = 0.0f;
            }
            return;

        case 3:
            if (this->unk_0146[0] == 0) {
                this->unk_0144 = 0;
            }
            return;

        case 5:
            this->unk_01B0.x = player->actor.world.pos.x;
            this->unk_01B0.y = player->actor.world.pos.y + (100.0f * D_809DF5B0);
            this->unk_01B0.z = player->actor.world.pos.z;
            if (this->unk_0146[0] == 0) {
                this->unk_0144 = 3;
                this->unk_01B0.x = randPlusMinusPoint5Scaled(500.0f * D_809DF5B0) + this->actor.world.pos.x;
                this->unk_01B0.y = -3000.0f * D_809DF5B0;
                if (sIsInGiantMode) {
                    this->unk_01B0.y += 3150.0f;
                }
                this->unk_01B0.z = randPlusMinusPoint5Scaled(500.0f * D_809DF5B0) + this->actor.world.pos.z;
                this->unk_0146[0] = 150;
                this->unk_0164 = 0.0f;
            }
            return;

        case 10:
            if (this->unk_1678 != 0) {
                this->unk_019A = 0x500;
                this->unk_01A0 = 0x1200;
            } else {
                this->colliderCylinder.dim.radius = 150.0f * D_809DF5B0;
                this->colliderCylinder.dim.height = 200.0f * D_809DF5B0;
                this->colliderCylinder.dim.yShift = 0;
                Collider_UpdateCylinder(&this->actor, &this->colliderCylinder);
                CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderCylinder.base);
                CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinder.base);
            }
            return;

        case 11:
            if (this == sRedTwinmold) {
                this->unk_01B0.x = D_809DF9C0[this->unk_1D1A].x;
                this->unk_01B0.y = D_809DF9C0[this->unk_1D1A].y;
                this->unk_01B0.z = D_809DF9C0[this->unk_1D1A].z;
            } else {
                this->unk_01B0.x = D_809DFA2C[this->unk_1D1A].x;
                this->unk_01B0.y = D_809DFA2C[this->unk_1D1A].y;
                this->unk_01B0.z = D_809DFA2C[this->unk_1D1A].z;
            }
            this->actor.shape.rot.x = 0x4000;
            this->unk_0144 = 12;
            this->actor.world.pos.x = this->unk_01B0.x;
            this->actor.world.pos.y = -200.0f;
            this->actor.world.pos.z = this->unk_01B0.z;
            this->unk_01A8 = 25.0f;
            return;

        case 12:
            if (sqrtf(SQ(spCC) + SQ(spC8) + SQ(spC4)) < 500.0f) {
                this->unk_1D1A++;
                this->unk_0164 = 0.0f;
                if (this->unk_1D1A > 8) {
                    this->unk_1D1A = 8;
                }
                if (this == sRedTwinmold) {
                    this->unk_01B0.x = D_809DF9C0[this->unk_1D1A].x;
                    this->unk_01B0.y = D_809DF9C0[this->unk_1D1A].y;
                    this->unk_01B0.z = D_809DF9C0[this->unk_1D1A].z;
                } else {
                    this->unk_01B0.x = D_809DFA2C[this->unk_1D1A].x;
                    this->unk_01B0.y = D_809DFA2C[this->unk_1D1A].y;
                    this->unk_01B0.z = D_809DFA2C[this->unk_1D1A].z;
                }
            }
            return;

        case 20:
            this->unk_0152 = 15;
            if ((s16)(BREG(71) + 140) < this->unk_0146[1]) {
                if (this->unk_0146[0] == 0) {
                    Matrix_RotateYS(Math_Atan2S(-player->actor.world.pos.x, -player->actor.world.pos.z), MTXMODE_NEW);
                    Matrix_MultVecZ(1500.0f * D_809DF5B0, &spA4);
                    this->unk_0146[0] = 50;
                    this->unk_01B0.x = player->actor.world.pos.x + spA4.x;
                    this->unk_01B0.y = randPlusMinusPoint5Scaled(500.0f * D_809DF5B0) + (600.0f * D_809DF5B0);
                    if (sIsInGiantMode) {
                        this->unk_01B0.y += 3150.0f;
                    }
                    this->unk_01B0.z = player->actor.world.pos.z + spA4.z;
                }
                this->unk_0168 = 3000.0f;
            } else {
                this->unk_01B0.y += 10.0f * D_809DF5B0;
                this->unk_0168 = 5000.0f;
            }
            this->unk_019A = 0x1000;
            this->unk_01A8 = 25.0f;
            this->unk_01A0 = 0x1800;
            if (this->unk_0146[1] == 0) {
                this->unk_015C = 1;
                this->unk_0144 = 21;
                this->unk_0146[0] = 20;
                this->unk_0152 = 0;
                sTwinmoldStatic->unk_1D20 = 102;
                sTwinmoldStatic->subCamAtVel = 0.0f;
                play_sound(NA_SE_EN_INBOSS_DEAD_PRE2_OLD);
            } else if (!(this->unk_0146[1] & 0xF) && (Rand_ZeroOne() < 0.5f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_INBOSS_DAMAGE_OLD);
            }
            return;

        case 21:
            this->unk_01A8 = 0.0f;
            this->actor.speedXZ = 0.0f;
            if (this->unk_0146[0] == 0) {
                this->unk_0146[0] = 3;

                for (i = 0; i < 35; i++) {
                    Boss02_SpawnEffectFragment(play->specialEffects, &this->unk_147C[this->unk_1678]);
                }

                Boss02_SpawnEffectFlash(play->specialEffects, &this->unk_147C[this->unk_1678]);
                play_sound(NA_SE_EV_EXPLOSION);

                this->unk_1678--;
                if (this->unk_1678 <= 0) {
                    this->unk_0144 = 22;
                    this->actor.gravity = -1.0f * D_809DF5B0;
                    this->actor.velocity.y = 0.0f;
                    this->actor.terminalVelocity = -1000.0f * D_809DF5B0;
                    this->unk_0164 = randPlusMinusPoint5Scaled(0.05f);

                    spCC = player->actor.world.pos.x - this->actor.world.pos.x;
                    spC4 = player->actor.world.pos.z - this->actor.world.pos.z;
                    if (sqrtf(SQ(spCC) + SQ(spC4)) < (400.0f * D_809DF5B0)) {
                        this->actor.speedXZ = 15.0f * D_809DF5B0;
                    }

                    spCC = this->actor.world.pos.x;
                    spC4 = this->actor.world.pos.z;
                    if (sqrtf(SQ(spCC) + SQ(spC4)) < (400.0f * D_809DF5B0)) {
                        this->actor.speedXZ = 15.0f * D_809DF5B0;
                    }

                    if (otherTwinmold->unk_0144 >= 10) {
                        Audio_QueueSeqCmd(NA_BGM_CLEAR_BOSS | 0x8000);
                    }

                    Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_INBOSS_DEAD_OLD);
                }
            }
            return;

        case 22:
            i = (this->unk_014E + 196) % ARRAY_COUNT(this->unk_01BC);

            Math_Vec3f_Copy(&this->unk_01BC[i], &this->actor.world.pos);
            this->unk_0B1C[i].y += this->unk_0164;
            Math_ApproachF(&this->unk_0B1C[i].x, -(M_PI / 2), 0.1f, 0.07f);
            Actor_MoveWithGravity(&this->actor);
            Actor_UpdateBgCheckInfo(play, &this->actor, 50.0f, 150.0f, 100.0f, 4);

            if (this->actor.bgCheckFlags & 1) {
                this->unk_0144 = 23;
                this->actor.speedXZ = 0.0f;
                this->unk_0170 = this->unk_017C;
                this->unk_016C = 30;
                this->unk_0170.y = this->actor.floorHeight;
                sTwinmoldStatic->unk_1D20 = 103;
                sTwinmoldStatic->unk_1D1C = 0;
                sTwinmoldStatic->unk_0146[0] = 15;
                sTwinmoldStatic->unk_0150 = 0;
                play_sound(NA_SE_EV_LIGHTNING);

                for (i = 0; i < 30; i++) {
                    Boss02_SpawnEffectFragment(play->specialEffects, &this->unk_0170);
                }

                this->unk_0146[0] = 10;
            }
            break;

        case 23:
            i = (this->unk_014E + 196) % ARRAY_COUNT(this->unk_01BC);
            Math_Vec3f_Copy(&this->unk_01BC[i], &this->actor.world.pos);
            Math_ApproachF(&this->unk_0B1C[i].x, -(M_PI / 2), 0.05f, 0.07f);

            if (this->unk_0146[0] & 1) {
                sp9C = Rand_ZeroFloat(M_PI);

                for (i = 0; i < 15; i++) {
                    Matrix_RotateYF(((2.0f * (i * M_PI)) / 15.0f) + sp9C, MTXMODE_NEW);
                    Matrix_MultVecZ((10 - this->unk_0146[0]) * (D_809DF5B0 * 300.0f) * 0.1f, &sp90);
                    spD0.x = this->unk_0170.x + sp90.x;
                    spD0.y = this->unk_0170.y + (1000.0f * D_809DF5B0);
                    spD0.z = this->unk_0170.z + sp90.z;
                    if (BgCheck_EntityRaycastFloor3(&play->colCtx, &sp8C, &sp88, &spD0) != BGCHECK_Y_MIN) {
                        spA0 = BgCheck_EntityRaycastFloor1(&play->colCtx, &sp8C, &spD0);
                        Matrix_MultVecZ(5.0f * D_809DF5B0, &sp70);
                        sp70.y = 2.0f * D_809DF5B0;
                        sp64.y = 0.3f * D_809DF5B0;
                        sp64.z = 0.0f;
                        sp64.x = 0.0f;
                        sp7C.x = spD0.x;
                        sp7C.y = spA0;
                        sp7C.z = spD0.z;
                        func_800B0F18(play, &sp7C, &sp70, &sp64, &D_809DFA98, &D_809DFA98,
                                      (Rand_ZeroFloat(500.0f) + 1200.0f) * D_809DF5B0, 20.0f * D_809DF5B0,
                                      Rand_ZeroFloat(5.0f) + 14.0f);
                    }
                }
            }
    }

    this->colliderCylinder.dim.radius = 150.0f * D_809DF5B0;
    this->colliderCylinder.dim.height = 200.0f * D_809DF5B0;
    this->colliderCylinder.dim.yShift = 0;
    Collider_UpdateCylinder(&this->actor, &this->colliderCylinder);
    CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderCylinder.base);
}

void func_809DBFB4(Boss02* this, PlayState* play) {
    Boss02* otherTwinmold = this->otherTwinmold;
    s32 temp;
    s32 i;
    u8 damage;

    if (this->unk_0154 == 0) {
        if (sIsInGiantMode) {
            temp = 0;
        } else {
            temp = 1;
        }

        for (i = temp; i < ARRAY_COUNT(this->colliderSphere2Elements); i++) {
            if (this->colliderSphere2.elements[i].info.bumperFlags & BUMP_HIT) {
                this->colliderSphere2.elements[i].info.bumperFlags &= ~BUMP_HIT;
                this->unk_0154 = 15;
                this->unk_0156 = 15;

                if (i == 0) {
                    Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_INBOSS_DAMAGE_OLD);
                    this->unk_015C = 1;
                } else {
                    Audio_PlaySfxAtPos(&this->unk_167C, NA_SE_EN_INBOSS_DAMAGE_OLD);
                    this->unk_015C = 10;
                }

                if (sIsInGiantMode) {
                    this->unk_0152 = 15;
                } else {
                    this->unk_0152 = 12;
                }

                damage = this->actor.colChkInfo.damage;

                if (otherTwinmold->unk_0144 < 20) {
                    do {
                        do {
                            this->actor.colChkInfo.health -= damage;
                        } while (0);
                    } while (0);

                    if ((s8)this->actor.colChkInfo.health <= 0) {
                        Actor_Kill(this->actor.child);
                        this->actor.child = NULL;
                        Enemy_StartFinishingBlow(play, &this->actor);
                        this->skelAnime.playSpeed = 2.0f;
                        this->unk_0144 = 20;

                        if (otherTwinmold->unk_0144 >= 10) {
                            Audio_QueueSeqCmd(0x100100FF);
                        } else {
                            otherTwinmold->unk_0195 = 1;
                        }

                        this->unk_0146[1] = BREG(70) + 170;
                        this->unk_0146[0] = 0;
                        this->actor.shape.rot.x = 0x4000;

                        if (!sIsInGiantMode) {
                            if (this->actor.world.pos.y < -200.0f) {
                                this->actor.world.pos.y = -200.0f;
                            }
                        } else if (this->actor.world.pos.y < 3130.0f) {
                            this->actor.world.pos.y = 3130.0f;
                        }

                        sTwinmoldStatic->unk_1D20 = 100;

                        if (this == sRedTwinmold) {
                            sTwinmoldStatic->unk_0194 = 0;
                        } else {
                            sTwinmoldStatic->unk_0194 = 1;
                        }
                    }
                }
            }
        }
    }
}

void Boss02_Tail_Update(Actor* thisx, PlayState* play) {
    Boss02* this = THIS;
    s32 pad;
    Vec3f pos;
    CollisionPoly* outPoly;

    this->unk_014C++;
    if ((this->unk_014C + this->actor.params) & 1) {
        pos = this->actor.world.pos;
        this->actor.focus.pos = this->actor.world.pos;
        if (sIsInGiantMode) {
            pos.y = 5000.0f;
        } else {
            pos.y = 2000.0f;
        }

        if ((this->actor.focus.pos.y < BgCheck_EntityRaycastFloor1(&play->colCtx, &outPoly, &pos)) || sIsInGiantMode) {
            this->actor.flags &= ~ACTOR_FLAG_1;
        } else {
            this->actor.flags |= ACTOR_FLAG_1;
        }
    }
}

void Boss02_Twinmold_Update(Actor* thisx, PlayState* play) {
    Vec3f sp3C;
    Boss02* this = THIS;
    s32 pad;
    s16 i;

    if (!sIsInGiantMode) {
        Actor_SetScale(&this->actor, 0.6f);
    } else {
        Actor_SetScale(&this->actor, 0.060000001f);
    }

    if (sTwinmoldStatic->unk_1D18 == 0) {
        for (i = 0; i < ARRAY_COUNT(this->unk_0146); i++) {
            if (this->unk_0146[i] != 0) {
                this->unk_0146[i]--;
            }
        }

        if (this->unk_016C != 0) {
            this->unk_016C--;
        }

        if (this->unk_0154 != 0) {
            this->unk_0154--;
        }

        if (this->unk_0156 != 0) {
            this->unk_0156--;
        }

        if (this->unk_0152 != 0) {
            this->unk_0152--;
        }

        this->unk_014C++;

        this->actionFunc(this, play);

        if (this->unk_016C != 0) {
            if ((this->unk_016C == 60) && (this->unk_0144 < 20)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_INBOSS_ROAR_OLD);
            }
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_INBOSS_SAND_OLD - SFX_FLAG);

            if (this->unk_0144 > 20) {
                sp3C.x = randPlusMinusPoint5Scaled(100.0f * D_809DF5B0) + this->unk_0170.x;
                sp3C.y = randPlusMinusPoint5Scaled(50.0f * D_809DF5B0) + this->unk_0170.y;
                sp3C.z = randPlusMinusPoint5Scaled(100.0f * D_809DF5B0) + this->unk_0170.z;
                Boss02_SpawnEffectSand(play->specialEffects, &sp3C, Rand_ZeroFloat(3.0f) + 6.0f);
            }

            if ((this->unk_014C % 2) == 0) {
                sp3C.x = randPlusMinusPoint5Scaled(100.0f * D_809DF5B0) + this->unk_0170.x;
                sp3C.y = randPlusMinusPoint5Scaled(50.0f * D_809DF5B0) + this->unk_0170.y;
                sp3C.z = randPlusMinusPoint5Scaled(100.0f * D_809DF5B0) + this->unk_0170.z;
                Boss02_SpawnEffectSand(play->specialEffects, &sp3C, Rand_ZeroFloat(3.0f) + 6.0f);
            }
        }

        if ((this->unk_0144 < 20) && (this->unk_0144 != 10)) {
            func_809DBFB4(this, play);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderSphere1.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSphere1.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderSphere1.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderSphere2.base);
            CollisionCheck_SetAT(play, &play->colChkCtx, &this->colliderSphere2.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderSphere2.base);
        }

        switch (this->unk_015C) {
            case 1:
                this->unk_015A = 0;
                this->unk_0158 = this->unk_015A;
                this->unk_015C++;

            case 2:
                this->unk_015A += 2;
                if (this->unk_015A > 20) {
                    this->unk_015C++;
                }
                this->unk_0156 |= 4;
                break;

            case 3:
                this->unk_0158 += 2;
                if (this->unk_0158 > 20) {
                    this->unk_015C = 0;
                }
                this->unk_0156 |= 4;
                break;

            case 10:
                this->unk_015A = 21;
                this->unk_0158 = this->unk_015A;
                this->unk_015C++;

            case 11:
                this->unk_0158 -= 2;
                if (this->unk_0158 <= 0) {
                    this->unk_0158 = 0;
                    this->unk_015C++;
                }
                this->unk_0156 |= 4;
                break;

            case 12:
                this->unk_015A -= 2;
                if (this->unk_015A <= 0) {
                    this->unk_015A = 0;
                    this->unk_015C = 0;
                }
                this->unk_0156 |= 4;
                break;
        }
    }
}

void Boss02_Static_Update(Actor* thisx, PlayState* play) {
    Boss02* this = THIS;

    this->unk_01AC = D_809DF5B0;
    play->envCtx.sandstormState = 0xD;

    if (sBlueWarp != NULL) {
        this->unk_1D74 = KREG(23) + -15.0f;
        D_801F4E30 = 0;
        play->envCtx.lightSettingOverride = 1;
        play->skyboxId = 1;
    } else if (!sIsInGiantMode) {
        this->unk_1D74 = 0.0f;
        D_801F4E30 = this->unk_1D7C;
        play->envCtx.lightSettingOverride = 0;
        play->skyboxId = 2;
    } else {
        this->unk_1D74 = KREG(23) + -15.0f;
        D_801F4E30 = ((KREG(24) * 0.1f) + 1.0f) * this->unk_1D7C;
        play->envCtx.lightSettingOverride = 1;
        play->skyboxId = 1;
    }

    Math_ApproachS(&this->unk_1D7C, this->unk_1D7E, 1, 3);

    if (this->unk_0146[0] != 0) {
        this->unk_0146[0]--;
    }

    if (sBlueWarp == NULL) {
        if (sMusicStartTimer != 0) {
            sMusicStartTimer--;
            if (sMusicStartTimer == 0) {
                Audio_QueueSeqCmd(NA_BGM_BOSS | 0x8000);
            }
        }
    }

    Boss02_UpdateEffects(play);
    func_809DEAC4(this, play);
    func_809DD934(this, play);
}

Gfx* D_809DFA9C[] = {
    gTwinmoldBodySegment1DL,  gTwinmoldBodySegment2DL,  gTwinmoldBodySegment3DL,  gTwinmoldBodySegment4DL,
    gTwinmoldBodySegment5DL,  gTwinmoldBodySegment6DL,  gTwinmoldBodySegment7DL,  gTwinmoldBodySegment8DL,
    gTwinmoldBodySegment9DL,  gTwinmoldBodySegment10DL, gTwinmoldBodySegment11DL, gTwinmoldBodySegment12DL,
    gTwinmoldBodySegment13DL, gTwinmoldBodySegment14DL, gTwinmoldBodySegment15DL, gTwinmoldBodySegment16DL,
    gTwinmoldBodySegment17DL, gTwinmoldBodySegment18DL, gTwinmoldBodySegment19DL, gTwinmoldBodySegment20DL,
    gTwinmoldBodySegment21DL, gTwinmoldBodyTailDL,
};

Vec3f D_809DFAF4 = { -10000.0f, -100000.0f, -100000.0f };

void Boss02_Twinmold_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    Boss02* this = THIS;
    s32 i;
    s32 idx;
    Mtx* mtxHead = GRAPH_ALLOC(play->state.gfxCtx, 23 * sizeof(Mtx));
    Mtx* mtx = mtxHead;
    s32 phi_v0;
    f32 phi_f12;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 sp98;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C28C(play->state.gfxCtx);

    if (this->actor.params == 0) {
        gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(gTwinmoldRedSkinTex));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(gTwinmoldBlueSkinTex));
    }

    gSPSegment(POLY_OPA_DISP++, 0x0D, mtxHead);

    if (!sIsInGiantMode) {
        sp98 = -500.0f;
    } else {
        sp98 = 3100.0f;
    }

    sp9C = this->unk_0152 * (M_PI / 4) * (1.0f / 15);
    if (this->unk_0144 < 20) {
        spAC = Math_SinS(this->unk_014C * 0x2200) * sp9C * 0.5f;
        spA8 = Math_CosS(this->unk_014C * 0x3200) * sp9C;
    } else {
        spAC = Math_SinS(this->unk_014C * 0x1000) * sp9C * 0.5f;
        spA8 = Math_CosS(this->unk_014C * 0x1500) * sp9C;
    }

    idx = (this->unk_014E + 196) % ARRAY_COUNT(this->unk_01BC);
    Matrix_Translate(this->unk_01BC[idx].x, this->unk_01BC[idx].y, this->unk_01BC[idx].z, MTXMODE_NEW);
    Matrix_RotateYF(this->unk_0B1C[idx].y + spA8, MTXMODE_APPLY);
    Matrix_RotateXFApply(spAC - this->unk_0B1C[idx].x);
    Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 0.0f, 250.0f, MTXMODE_APPLY);
    Matrix_MultVecZ(150.0f, &this->unk_147C[0]);
    this->unk_0188 = this->unk_017C;
    Matrix_MultVecZ(300.0f, &this->unk_017C);
    Math_Vec3f_Copy(&this->actor.focus.pos, &this->unk_017C);

    if ((this->unk_0156 & 1) && (this->unk_0158 == 0)) {
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 255, 0x384, 0x44B);
    }

    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, NULL, &this->actor);
    POLY_OPA_DISP = func_801660B8(play, POLY_OPA_DISP);

    spA4 = 0.0f;
    spA0 = 0.0f;
    for (i = 0; i < ARRAY_COUNT(D_809DFA9C); i++, mtx++) {
        if (this->unk_0195 != 0) {
            phi_v0 = (D_809DF5E4[i + 1] + this->unk_014E) % ARRAY_COUNT(this->unk_01BC);
        } else {
            phi_v0 = (D_809DF5B4[i + 1] + this->unk_014E) % ARRAY_COUNT(this->unk_01BC);
        }

        if ((i == 21) && (this->unk_0144 < 20)) {
            spA4 = spAC;
            spA0 = spA8;
        }

        Matrix_Translate(this->unk_01BC[phi_v0].x, this->unk_01BC[phi_v0].y, this->unk_01BC[phi_v0].z, MTXMODE_NEW);
        Matrix_RotateYF(this->unk_0B1C[phi_v0].y + spA0, MTXMODE_APPLY);
        Matrix_RotateXFApply(spA4 - this->unk_0B1C[phi_v0].x);
        Matrix_RotateZF(this->unk_0B1C[phi_v0].z, MTXMODE_APPLY);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
        Matrix_RotateYF(M_PI / 2, MTXMODE_APPLY);
        Matrix_RotateXFApply(-(M_PI / 2));
        Matrix_ToMtx(mtx);

        gSPMatrix(POLY_OPA_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if ((this->unk_0156 & 1) && (i >= this->unk_0158) && (this->unk_015A >= i)) {
            POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 0, 0, 255, 0x384, 0x44B);
        }

        if (i < this->unk_1678) {
            if (sp98 < this->unk_01BC[phi_v0].y) {
                gSPDisplayList(POLY_OPA_DISP++, D_809DFA9C[i]);
            }
            POLY_OPA_DISP = func_801660B8(play, POLY_OPA_DISP);
        }

        if (i == 21) {
            phi_f12 = 600.0f;
        } else {
            phi_f12 = 200.0f;
        }

        Matrix_MultVecX(phi_f12, &this->unk_147C[i + 1]);

        if (i == 21) {
            Actor* child = this->actor.child;

            if (child != NULL) {
                Matrix_MultVecX(500.0f, &this->actor.child->world.pos);
            }

            func_809DA50C(1, &this->colliderSphere2, &this->unk_147C[i + 1]);
            SkinMatrix_Vec3fMtxFMultXYZW(&play->viewProjectionMtxF, &this->unk_147C[i + 1], &this->unk_167C,
                                         &this->actor.projectedW);
        } else {
            func_809DA50C(i, &this->colliderSphere1, &this->unk_147C[i + 1]);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);

    if (!sIsInGiantMode) {
        func_809DA50C(0, &this->colliderSphere2, &D_809DFAF4);
        func_809DA50C(21, &this->colliderSphere1, &this->unk_147C[0]);
    } else {
        func_809DA50C(0, &this->colliderSphere2, &this->unk_147C[0]);
        func_809DA50C(21, &this->colliderSphere1, &D_809DFAF4);
    }
}

void Boss02_Static_Draw(Actor* thisx, PlayState* play) {
    Boss02_DrawEffects(play);
}

void Boss02_UpdateEffects(PlayState* play) {
    TwinmoldEffect* effect = (TwinmoldEffect*)play->specialEffects;
    f32 floorY;
    s16 i;

    if (!sIsInGiantMode) {
        floorY = 0.0f;
    } else {
        floorY = 3150.0f;
    }

    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type) {
            effect->timer++;
            effect->pos.x += effect->velocity.x * D_809DF5B0;
            effect->pos.y += effect->velocity.y * D_809DF5B0;
            effect->pos.z += effect->velocity.z * D_809DF5B0;
            effect->velocity.y += effect->accel.y;

            if (effect->type < TWINMOLD_EFFECT_FRAGMENT) {
                Math_ApproachF(&effect->scale, effect->targetScale, 0.1f, 0.1f);
                if (effect->type == TWINMOLD_EFFECT_BLACK_DUST) {
                    effect->alpha -= 18;
                } else {
                    effect->alpha -= 15;
                }

                if (effect->alpha <= 0) {
                    effect->type = TWINMOLD_EFFECT_NONE;
                }
            } else if (effect->type == TWINMOLD_EFFECT_FRAGMENT) {
                effect->rotX += 0x1000;
                effect->rotY += 0x1500;
                if ((effect->pos.y < floorY) || (effect->timer > 50)) {
                    effect->type = TWINMOLD_EFFECT_NONE;
                }
            } else if (effect->type == TWINMOLD_EFFECT_FLASH) {
                Math_ApproachF(&effect->scale, 80.0f, 0.2f, 20.0f);
                effect->alpha -= 15;
                if (effect->alpha <= 0) {
                    effect->type = TWINMOLD_EFFECT_NONE;
                }
            }
        }
    }
}

void Boss02_DrawEffects(PlayState* play) {
    u8 flag = false;
    TwinmoldEffect* effect = (TwinmoldEffect*)play->specialEffects;
    s16 i;
    u8 alpha;

    OPEN_DISPS(play->state.gfxCtx);

    func_8012C28C(play->state.gfxCtx);
    func_8012C2DC(play->state.gfxCtx);

    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type == TWINMOLD_EFFECT_SAND) {
            if (!flag) {
                gSPDisplayList(POLY_XLU_DISP++, gTwinmoldDustMaterialDL);
                gDPSetEnvColor(POLY_XLU_DISP++, 185, 140, 70, 128);
                flag++;
            }

            alpha = effect->alpha;
            if (effect->alpha > 255) {
                alpha = 255;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 185, 140, 70, alpha);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(play->state.gfxCtx, 0, effect->timer + (i * 3), (effect->timer + (i * 3)) * 5,
                                        32, 64, 1, 0, 0, 32, 32));

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(effect->scale * D_809DF5B0, effect->scale * D_809DF5B0, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gTwinmoldDustModelDL);
        }
    }

    effect = (TwinmoldEffect*)play->specialEffects;
    for (i = 0, flag = false; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type == TWINMOLD_EFFECT_FRAGMENT) {
            if (!flag) {
                gDPSetCombineLERP(POLY_OPA_DISP++, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE,
                                  0, SHADE, 0, PRIMITIVE, 0);
                gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x01, 100, 100, 120, 255);
                flag++;
            }

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_RotateYS(effect->rotY, MTXMODE_APPLY);
            Matrix_RotateXS(effect->rotX, MTXMODE_APPLY);
            Matrix_Scale(effect->scale * D_809DF5B0, effect->scale * D_809DF5B0, effect->scale * D_809DF5B0,
                         MTXMODE_APPLY);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gEffFragments1DL);
        }
    }

    effect = (TwinmoldEffect*)play->specialEffects;
    for (i = 0, flag = false; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type == TWINMOLD_EFFECT_FLASH) {
            if (!flag) { //! @bug - dev forgot to set flag to 1, should only apply to first entry?
                gSPDisplayList(POLY_XLU_DISP++, gLightOrbMaterial1DL);
                gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 128);
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 200, (u8)effect->alpha);

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(effect->scale * D_809DF5B0, effect->scale * D_809DF5B0, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gLightOrbModelDL);
        }
    }

    effect = (TwinmoldEffect*)play->specialEffects;
    for (i = 0, flag = false; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type == TWINMOLD_EFFECT_BLACK_DUST) {
            if (!flag) {
                gSPDisplayList(POLY_XLU_DISP++, gTwinmoldDustMaterialDL);
                gDPSetEnvColor(POLY_XLU_DISP++, 30, 30, 30, 128);
                flag++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 30, 30, 30, (u8)effect->alpha);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(play->state.gfxCtx, 0, effect->timer + (i * 3), (effect->timer + (i * 3)) * 5,
                                        32, 64, 1, 0, 0, 32, 32));

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_ReplaceRotation(&play->billboardMtxF);
            Matrix_Scale(effect->scale * D_809DF5B0, effect->scale * D_809DF5B0, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gTwinmoldDustModelDL);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void func_809DD934(Boss02* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 i;
    Actor* temp_a0_5;
    Vec3f sp58;
    u8 sp57 = 0;
    f32 phi_f0_2;
    s16 alpha;

    this->unk_1D14++;

    switch (this->unk_1D18) {
        case 0:
            if (player->stateFlags1 & PLAYER_STATE1_100) {
                Cutscene_Start(play, &play->csCtx);
                this->subCamId = Play_CreateSubCamera(play);
                Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STATUS_WAIT);
                Play_ChangeCameraStatus(play, this->subCamId, CAM_STATUS_ACTIVE);
                func_8016566C(150);
                this->unk_1D14 = 0;
                this->subCamAtVel = 0.0f;
                this->unk_1D58 = 0.0f;
                if (!sIsInGiantMode) {
                    this->unk_1D18 = 1;
                    this->unk_1D68 = 10.0f;
                    this->unk_1D64 = 60.0f;
                    this->unk_1D6C = 23.0f;
                    this->unk_1D70 = 0.01f;
                    this->unk_1D74 = 0.0f;
                    goto label1;
                } else {
                    this->unk_1D18 = 10;
                    this->unk_1D68 = 10.0f;
                    this->unk_1D64 = 200.0f;
                    this->unk_1D6C = 273.0f;
                    this->unk_1D70 = 0.1f;
                    this->unk_1D74 = -100.0f;
                    sp57 = 1;
                    goto label2;
                }
            }
            break;

        case 1:
            if ((this->unk_1D14 < 80) && (D_809E0420 != 0) &&
                CHECK_BTN_ANY(CONTROLLER1(&play->state)->press.button,
                              BTN_A | BTN_B | BTN_CUP | BTN_CDOWN | BTN_CLEFT | BTN_CRIGHT)) {
                this->unk_1D18++;
                this->unk_1D78 = 1;
                this->unk_1D14 = 0;
            } else {
            label1:
                if (this->unk_1D14 >= 50) {
                    if (this->unk_1D14 == (u32)(BREG(43) + 60)) {
                        play_sound(NA_SE_PL_TRANSFORM_GIANT);
                    }
                    Math_ApproachF(&this->unk_1D64, 200.0f, 0.1f, this->subCamAtVel * 640.0f);
                    Math_ApproachF(&this->unk_1D6C, 273.0f, 0.1f, this->subCamAtVel * 150.0f);
                    Math_ApproachF(&this->unk_1D70, 0.1f, 0.2f, this->subCamAtVel * 0.1f);
                    Math_ApproachF(&this->unk_1D74, -100.0f, 1.0f, this->subCamAtVel * 100.0f);
                    Math_ApproachF(&this->subCamAtVel, 1.0f, 1.0f, 0.001f);
                } else {
                    Math_ApproachF(&this->unk_1D64, 30.0f, 0.1f, 1.0f);
                }

                if (this->unk_1D14 > 50) {
                    Math_ApproachZeroF(&this->unk_1D58, 1.0f, 0.06f);
                } else {
                    Math_ApproachF(&this->unk_1D58, 0.4f, 1.0f, 0.02f);
                }

                if (this->unk_1D14 == 107) {
                    this->unk_1D78 = 1;
                }

                if (this->unk_1D14 < 121) {
                    break;
                }

                sp57 = 1;
                D_809E0420 = 1;
                goto block_38;
            }
            break;

        case 2:
            if (this->unk_1D14 < 8) {
                break;
            }
            sp57 = 1;
            goto block_38;

        case 10:
            if ((this->unk_1D14 < 30) && (D_809E0421 != 0) &&
                CHECK_BTN_ANY(CONTROLLER1(&play->state)->press.button,
                              BTN_A | BTN_B | BTN_CUP | BTN_CDOWN | BTN_CLEFT | BTN_CRIGHT)) {
                this->unk_1D18++;
                this->unk_1D78 = 1;
                this->unk_1D14 = 0;
                break;
            }

        label2:
            if (this->unk_1D14 != 0) {
                if (this->unk_1D14 == (u32)(BREG(44) + 10)) {
                    play_sound(NA_SE_PL_TRANSFORM_NORAML);
                }
                Math_ApproachF(&this->unk_1D64, 60.0f, 0.1f, this->subCamAtVel * 640.0f);
                Math_ApproachF(&this->unk_1D6C, 23.0f, 0.1f, this->subCamAtVel * 150.0f);
                Math_ApproachF(&this->unk_1D70, 0.01f, 0.1f, 0.003f);
                Math_ApproachF(&this->unk_1D74, 0.0f, 1.0f, this->subCamAtVel * 100.0f);
                Math_ApproachF(&this->subCamAtVel, 2.0f, 1.0f, 0.01f);
            }

            if (this->unk_1D14 == 42) {
                this->unk_1D78 = 1;
            }

            if (this->unk_1D14 > 50) {
                D_809E0421 = 1;
                goto block_38;
            }
            break;

        case 11:
            if (this->unk_1D14 < 8) {
                break;
            }

        block_38:
        case 20:
            this->unk_1D18 = 0;
            func_80169AFC(play, this->subCamId, 0);
            this->subCamId = SUB_CAM_ID_DONE;
            Cutscene_End(play, &play->csCtx);
            this->actor.flags |= ACTOR_FLAG_1;
            player->stateFlags1 &= ~PLAYER_STATE1_100;
            this->unk_1D70 = 0.01f;
            func_80165690();
            break;
    }

    if (player->transformation == PLAYER_FORM_FIERCE_DEITY) {
        Actor_SetScale(&player->actor, this->unk_1D70 * 1.5f);
    } else {
        Actor_SetScale(&player->actor, this->unk_1D70);
    }

    play->envCtx.lightSettings.fogNear = this->unk_1D74;

    if (sp57) {
        sIsInGiantMode = 1 - sIsInGiantMode;
        if (!sIsInGiantMode) {
            D_809DF5B0 = 1.0f;
        } else {
            D_809DF5B0 = 0.1f;
        }

        this->unk_01AC = D_809DF5B0;
        if (!sIsInGiantMode) {
            if (sBlueWarp != NULL) {
                sBlueWarp->unk_203 = 0;
                sBlueWarp->unk_204 = 1.0f;
                sBlueWarp->dyna.actor.world.pos.y = 60.0f;
            }

            player->actor.world.pos.x *= 10.0f;
            player->actor.world.pos.y -= 3150.0f;
            player->actor.world.pos.y *= 10.0f;
            player->fallStartHeight = player->actor.world.pos.y;
            player->actor.world.pos.z *= 10.0f;

            if ((sBlueWarp != NULL) && ((SQ(player->actor.world.pos.z) + SQ(player->actor.world.pos.x)) < SQ(60.0f))) {
                player->actor.world.pos.z = 60.0f;
                player->actor.world.pos.x = 60.0f;
            }

            sRedTwinmold->actor.world.pos.x *= 10.0f;
            sRedTwinmold->actor.world.pos.y -= 3150.0f;
            sRedTwinmold->actor.world.pos.y *= 10.0f;
            sRedTwinmold->actor.world.pos.z *= 10.0f;

            sRedTwinmold->unk_0170.x *= 10.0f;
            sRedTwinmold->unk_0170.y -= 3150.0f;
            sRedTwinmold->unk_0170.y *= 10.0f;
            sRedTwinmold->unk_0170.z *= 10.0f;

            for (i = 0; i < ARRAY_COUNT(this->unk_01BC); i++) {
                sRedTwinmold->unk_01BC[i].x *= 10.0f;
                sRedTwinmold->unk_01BC[i].y -= 3150.0f;
                sRedTwinmold->unk_01BC[i].y *= 10.0f;
                sRedTwinmold->unk_01BC[i].z *= 10.0f;
            }

            sRedTwinmold->unk_01B0.x *= 10.0f;
            sRedTwinmold->unk_01B0.y -= 3150.0f;
            sRedTwinmold->unk_01B0.y *= 10.0f;
            sRedTwinmold->unk_01B0.z *= 10.0f;

            sRedTwinmold->unk_017C.y -= 3150.0f;
            sRedTwinmold->unk_017C.y *= 10.0f;
            sRedTwinmold->unk_0188.y -= 3150.0f;
            sRedTwinmold->unk_0188.y *= 10.0f;

            sBlueTwinmold->actor.world.pos.x *= 10.0f;
            sBlueTwinmold->actor.world.pos.y -= 3150.0f;
            sBlueTwinmold->actor.world.pos.y *= 10.0f;
            sBlueTwinmold->actor.world.pos.z *= 10.0f;

            sBlueTwinmold->unk_0170.x *= 10.0f;
            sBlueTwinmold->unk_0170.y -= 3150.0f;
            sBlueTwinmold->unk_0170.y *= 10.0f;
            sBlueTwinmold->unk_0170.z *= 10.0f;

            for (i = 0; i < ARRAY_COUNT(this->unk_01BC); i++) {
                sBlueTwinmold->unk_01BC[i].x *= 10.0f;
                sBlueTwinmold->unk_01BC[i].y -= 3150.0f;
                sBlueTwinmold->unk_01BC[i].y *= 10.0f;
                sBlueTwinmold->unk_01BC[i].z *= 10.0f;
            }

            sBlueTwinmold->unk_01B0.x *= 10.0f;
            sBlueTwinmold->unk_01B0.y -= 3150.0f;
            sBlueTwinmold->unk_01B0.y *= 10.0f;
            sBlueTwinmold->unk_01B0.z *= 10.0f;

            sBlueTwinmold->unk_017C.y -= 3150.0f;
            sBlueTwinmold->unk_017C.y *= 10.0f;
            sBlueTwinmold->unk_0188.y -= 3150.0f;
            sBlueTwinmold->unk_0188.y *= 10.0f;

        } else {
            if (sBlueWarp != 0) {
                sBlueWarp->unk_203 = 1;
                sBlueWarp->unk_204 = 0.1f;
                sBlueWarp->dyna.actor.world.pos.y = 3155.0f;
            }

            player->actor.world.pos.x *= 0.1f;
            player->actor.world.pos.y *= 0.1f;
            player->actor.world.pos.y += 3150.0f;
            player->fallStartHeight = player->actor.world.pos.y;
            player->actor.world.pos.z *= 0.1f;

            sRedTwinmold->actor.world.pos.x *= 0.1f;
            sRedTwinmold->actor.world.pos.y *= 0.1f;
            sRedTwinmold->actor.world.pos.y += 3150.0f;
            sRedTwinmold->actor.world.pos.z *= 0.1f;

            sRedTwinmold->unk_0170.x *= 0.1f;
            sRedTwinmold->unk_0170.y *= 0.1f;
            sRedTwinmold->unk_0170.y += 3150.0f;
            sRedTwinmold->unk_0170.z *= 0.1f;

            for (i = 0; i < ARRAY_COUNT(this->unk_01BC); i++) {
                sRedTwinmold->unk_01BC[i].x *= 0.1f;
                sRedTwinmold->unk_01BC[i].y *= 0.1f;
                sRedTwinmold->unk_01BC[i].y += 3150.0f;
                sRedTwinmold->unk_01BC[i].z *= 0.1f;
            }

            sRedTwinmold->unk_01B0.x *= 0.1f;
            sRedTwinmold->unk_01B0.y *= 0.1f;
            sRedTwinmold->unk_01B0.y += 3150.0f;
            sRedTwinmold->unk_01B0.z *= 0.1f;

            sRedTwinmold->unk_017C.y *= 0.1f;
            sRedTwinmold->unk_017C.y += 3150.0f;
            sRedTwinmold->unk_0188.y *= 0.1f;
            sRedTwinmold->unk_0188.y += 3150.0f;

            sBlueTwinmold->actor.world.pos.x *= 0.1f;
            sBlueTwinmold->actor.world.pos.y *= 0.1f;
            sBlueTwinmold->actor.world.pos.y += 3150.0f;
            sBlueTwinmold->actor.world.pos.z *= 0.1f;

            sBlueTwinmold->unk_0170.x *= 0.1f;
            sBlueTwinmold->unk_0170.y *= 0.1f;
            sBlueTwinmold->unk_0170.y += 3150.0f;
            sBlueTwinmold->unk_0170.z *= 0.1f;

            for (i = 0; i < ARRAY_COUNT(this->unk_01BC); i++) {
                sBlueTwinmold->unk_01BC[i].x *= 0.1f;
                sBlueTwinmold->unk_01BC[i].y *= 0.1f;
                sBlueTwinmold->unk_01BC[i].y += 3150.0f;
                sBlueTwinmold->unk_01BC[i].z *= 0.1f;
            }

            sBlueTwinmold->unk_01B0.x *= 0.1f;
            sBlueTwinmold->unk_01B0.y *= 0.1f;
            sBlueTwinmold->unk_01B0.y += 3150.0f;
            sBlueTwinmold->unk_01B0.z *= 0.1f;

            sBlueTwinmold->unk_017C.y *= 0.1f;
            sBlueTwinmold->unk_017C.y += 3150.0f;
            sBlueTwinmold->unk_0188.y *= 0.1f;
            sBlueTwinmold->unk_0188.y += 3150.0f;
        }

        player->actor.home.pos = player->actor.world.pos;
        player->actor.prevPos = player->actor.world.pos;

        temp_a0_5 = play->actorCtx.actorLists[ACTORCAT_BG].first;
        while (temp_a0_5 != NULL) {
            if (temp_a0_5->id == ACTOR_BG_INIBS_MOVEBG) {
                Actor_Kill(temp_a0_5);
                break;
            }
            temp_a0_5 = temp_a0_5->next;
        }
        {
            f32 tmp = sIsInGiantMode ? 3150.0f : 0.0f;

            Actor_Spawn(&play->actorCtx, play, ACTOR_BG_INIBS_MOVEBG, 0, tmp, 0, 0, 0, 0, sIsInGiantMode);
        }

        temp_a0_5 = play->actorCtx.actorLists[ACTORCAT_BOSS].first;
        while (temp_a0_5 != NULL) {
            if ((temp_a0_5->id == ACTOR_EN_TANRON5) || (temp_a0_5->id == ACTOR_ITEM_B_HEART)) {
                if (!sIsInGiantMode) {
                    temp_a0_5->world.pos.y -= 3150.0f;
                    temp_a0_5->world.pos.y *= 10.0f;

                    temp_a0_5->floorHeight -= 3150.0f;
                    temp_a0_5->floorHeight *= 10.0f;

                    phi_f0_2 = 10.0f;
                } else {
                    temp_a0_5->world.pos.y *= 0.1f;
                    temp_a0_5->world.pos.y += 3150.0f;

                    temp_a0_5->floorHeight *= 0.1f;
                    temp_a0_5->floorHeight += 3150.0f;

                    phi_f0_2 = 0.1f;
                }

                temp_a0_5->world.pos.x *= phi_f0_2;
                temp_a0_5->world.pos.z *= phi_f0_2;

                temp_a0_5->speedXZ *= phi_f0_2;

                temp_a0_5->velocity.x *= phi_f0_2;
                temp_a0_5->velocity.y *= phi_f0_2;
                temp_a0_5->velocity.z *= phi_f0_2;

                temp_a0_5->gravity *= phi_f0_2;
                temp_a0_5->terminalVelocity *= phi_f0_2;

                temp_a0_5->scale.x *= phi_f0_2;
                temp_a0_5->scale.y *= phi_f0_2;
                temp_a0_5->scale.z *= phi_f0_2;

                if (temp_a0_5->id == ACTOR_ITEM_B_HEART) {
                    ((ItemBHeart*)temp_a0_5)->unk_168 *= phi_f0_2;
                }
            }
            temp_a0_5 = temp_a0_5->next;
        }
    }

    switch (this->unk_1D78) {
        case 0:
            break;

        case 1:
            this->unk_1D7A = 0;
            func_809DA1D0(play, 255, 255, 255, 0);
            this->unk_1D78 = 2;
            play_sound(NA_SE_SY_TRANSFORM_MASK_FLASH);

        case 2:
            this->unk_1D7A += 40;
            if (this->unk_1D7A >= 400) {
                this->unk_1D78 = 3;
            }
            alpha = this->unk_1D7A;
            alpha = CLAMP_MAX(alpha, 255);
            func_809DA22C(play, alpha);
            break;

        case 3:
            this->unk_1D7A -= 40;
            if (this->unk_1D7A <= 0) {
                this->unk_1D7A = 0;
                this->unk_1D78 = 0;
                func_809DA24C(play);
            } else {
                alpha = this->unk_1D7A;
                alpha = CLAMP_MAX(alpha, 255);
                func_809DA22C(play, alpha);
            }
            break;
    }

    if ((this->unk_1D18 != 0) && (this->subCamId != SUB_CAM_ID_DONE)) {
        Matrix_RotateYS(player->actor.shape.rot.y, MTXMODE_NEW);
        Matrix_MultVecZ(this->unk_1D64, &sp58);

        this->subCamEye.x = player->actor.world.pos.x + sp58.x;
        this->subCamEye.y = player->actor.world.pos.y + sp58.y + this->unk_1D68;
        this->subCamEye.z = player->actor.world.pos.z + sp58.z;

        this->subCamAt.x = player->actor.world.pos.x;
        this->subCamAt.y = player->actor.world.pos.y + this->unk_1D6C;
        this->subCamAt.z = player->actor.world.pos.z;

        this->unk_1D54 = Math_SinS(this->unk_1D14 * 1512) * this->unk_1D58;
        Matrix_RotateZF(this->unk_1D54, MTXMODE_APPLY);
        Matrix_MultVecY(1.0f, &this->subCamUp);
        Play_SetCameraAtEyeUp(play, this->subCamId, &this->subCamAt, &this->subCamEye, &this->subCamUp);
        ShrinkWindow_Letterbox_SetSizeTarget(27);
    }
}

void func_809DEAC4(Boss02* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Boss02* sp68;
    Vec3f subCamEye;
    f32 sp58 = 0.0f;

    this->unk_1D1C++;
    if (this->unk_0194 == 0) {
        sp68 = sRedTwinmold;
    } else {
        sp68 = sBlueTwinmold;
    }

    switch (this->unk_1D20) {
        case 0:
            break;

        case 1:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_52_20) || ((u32)(KREG(13) + 15) >= this->unk_1D1C)) {
                break;
            }
            Cutscene_Start(play, &play->csCtx);
            this->subCamId = Play_CreateSubCamera(play);
            Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STATUS_WAIT);
            Play_ChangeCameraStatus(play, this->subCamId, CAM_STATUS_ACTIVE);
            this->unk_1D20 = 2;
            this->unk_1D1C = 0;

        case 2:
            player->actor.shape.rot.y = -0x8000;
            player->actor.world.rot.y = player->actor.shape.rot.y;
            this->subCamEye.x = player->actor.world.pos.x - 20.0f;
            this->subCamEye.y = (Player_GetHeight(player) + player->actor.world.pos.y) - 29.0f;
            this->subCamEye.z = player->actor.world.pos.z - 50;

            this->subCamAt.x = player->actor.world.pos.x;
            this->subCamAt.y = (Player_GetHeight(player) + player->actor.world.pos.y) - 17.0f;
            this->subCamAt.z = player->actor.world.pos.z;
            if (this->unk_1D1C >= 30) {
                if (this->unk_1D1C == 30) {
                    Rumble_Override(0.0f, 50, 200, 1);
                }
                this->unk_0150 += 0x4000;
                sp58 = (Math_SinS(this->unk_0150) * (BREG(19) + 5)) * 0.1f;
                Matrix_RotateZF(Math_SinS(this->unk_1D1C * 0x3000) * ((KREG(28) * 0.001f) + 0.017f), MTXMODE_NEW);
                Matrix_MultVecY(1.0f, &this->subCamUp);
                func_8019F128(NA_SE_EV_EARTHQUAKE_LAST - SFX_FLAG);
            }

            if (this->unk_1D1C == 20) {
                this->unk_1D7E = 100;
            }

            if (this->unk_1D1C == 45) {
                func_800B7298(play, &this->actor, PLAYER_CSMODE_21);
                sMusicStartTimer = KREG(91) + 43;
            }

            if (this->unk_1D1C == 85) {
                sRedTwinmold->unk_0144 = 11;
                sRedTwinmold->unk_014C = 0;
            }

            if (this->unk_1D1C == 92) {
                Rumble_Override(0.0f, 255, 30, 100);
            }

            if (this->unk_1D1C == 100) {
                func_800B7298(play, &this->actor, PLAYER_CSMODE_115);
            }

            if (this->unk_1D1C == 112) {
                this->unk_1D20 = 10;
                this->unk_1D1C = 0;
            }
            break;

        case 10:
            if (this->unk_1D1C < 310) {
                this->subCamAtNext = sp68->actor.world.pos;
                this->subCamEye.x = (sp68->actor.world.pos.x * 50.0f) * 0.001f;
                this->subCamEye.y = (sp68->actor.world.pos.y * 50.0f) * 0.001f;
                if (this->subCamEye.y < 100.0f) {
                    this->subCamEye.y = 100.0f;
                }
                this->subCamEye.z = (sp68->actor.world.pos.z * 100) * 0.001f;
            }

            Math_ApproachF(&this->subCamAt.x, this->subCamAtNext.x, 0.1f, 3000.0f);
            Math_ApproachF(&this->subCamAt.y, this->subCamAtNext.y, 0.1f, 3000.0f);
            Math_ApproachF(&this->subCamAt.z, this->subCamAtNext.z, 0.1f, 3000.0f);

            if (this->unk_1D1C == 100) {
                sBlueTwinmold->unk_0144 = 11;
                sBlueTwinmold->unk_014C = 0;
                this->unk_0194 = 1;
            }

            if (this->unk_1D1C == (u32)(KREG(92) + 125)) {
                TitleCard_InitBossName(&play->state, &play->actorCtx.titleCtxt,
                                       Lib_SegmentedToVirtual(gTwinmoldTitleCardTex), 160, 180, 128, 40);
            }

            if (this->unk_1D1C == (u32)(BREG(27) + 335)) {
                func_80169AFC(play, this->subCamId, 0);
                this->subCamId = SUB_CAM_ID_DONE;
                Cutscene_End(play, &play->csCtx);
                func_800B7298(play, &this->actor, PLAYER_CSMODE_6);
                this->actor.flags |= ACTOR_FLAG_1;
                this->unk_1D20 = 0;
                sRedTwinmold->unk_0144 = sBlueTwinmold->unk_0144 = 3;
                sRedTwinmold->unk_0146[0] = sBlueTwinmold->unk_0146[0] = 60;
                SET_EVENTINF(EVENTINF_55);
            }
            break;

        case 100:
            if (ActorCutscene_GetCurrentIndex() == -1) {
                Cutscene_Start(play, &play->csCtx);
                this->subCamId = Play_CreateSubCamera(play);
                Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STATUS_WAIT);
                Play_ChangeCameraStatus(play, this->subCamId, CAM_STATUS_ACTIVE);
                this->unk_1D20 = 101;
                this->unk_1D1C = 0;
                this->subCamAtVel = 1.0f;
                this->subCamAt = sp68->actor.world.pos;
            } else {
                break;
            }

        case 101:
        case 102:
        case_10x:
            if ((this->unk_1D20 == 101) || (this->unk_1D20 == 103)) {
                this->subCamAtNext = sp68->actor.world.pos;
            } else {
                this->subCamAtNext = sp68->unk_147C[sp68->unk_1678];
            }

            this->subCamEye.x = player->actor.world.pos.x;
            this->subCamEye.y = player->actor.world.pos.y + 100.0f;
            if (!sIsInGiantMode) {
                if (this->subCamEye.y < 100.0f) {
                    this->subCamEye.y = 100.0f;
                }
            } else if (this->subCamEye.y < 3160.0f) {
                this->subCamEye.y = 3160.0f;
            }

            if (this->subCamAtNext.y < (100.0f * D_809DF5B0)) {
                this->subCamAtNext.y = (100.0f * D_809DF5B0);
            }

            this->subCamEye.z = player->actor.world.pos.z;
            Math_ApproachF(&this->subCamAt.x, this->subCamAtNext.x, 0.3f, this->subCamAtVel * 500.0f);
            Math_ApproachF(&this->subCamAt.y, this->subCamAtNext.y, 0.3f, this->subCamAtVel * 500.0f);
            Math_ApproachF(&this->subCamAt.z, this->subCamAtNext.z, 0.3f, this->subCamAtVel * 500.0f);
            Math_ApproachF(&this->subCamAtVel, 1.0f, 1.0f, 0.02f);
            break;

        case 103:
            this->unk_0150 += 0x4000;
            sp58 = Math_SinS(this->unk_0150);
            sp58 = (sp58 * this->unk_0146[0]) * 1.5f;
            if (this->unk_1D1C == 30) {
                func_80169AFC(play, this->subCamId, 0);
                this->subCamId = SUB_CAM_ID_DONE;
                Cutscene_End(play, &play->csCtx);
                func_800B7298(play, &this->actor, PLAYER_CSMODE_6);
                this->unk_1D20 = 0;
                this->actor.flags |= ACTOR_FLAG_1;
                sp68->unk_0144 = 10;
                if ((sRedTwinmold->unk_0144 >= 10) && (sBlueTwinmold->unk_0144 >= 10)) {
                    f32 phi_f0;

                    this->unk_1D7E = 0;
                    if (!sIsInGiantMode) {
                        Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_B_HEART, 0.0f, 30.0f, -150.0f, 0, 1, 0, 0);
                        phi_f0 = 60.0f;
                    } else {
                        Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_B_HEART, 0.0f, 3153.0f, -15.0f, 0, 1, 0, 35);
                        phi_f0 = 3155.0f;
                    }
                    sBlueWarp = (DoorWarp1*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DOOR_WARP1,
                                                               0.0f, phi_f0, 0.0f, 0, 0, 0, 1);

                    if (!sIsInGiantMode) {
                        sBlueWarp->unk_203 = 0;
                        sBlueWarp->unk_204 = 1.0f;
                    } else {
                        sBlueWarp->unk_203 = 1;
                        sBlueWarp->unk_204 = 0.1f;
                    }
                }
            }
            goto case_10x;
    }

    if ((this->unk_1D20 != 0) && (this->subCamId != SUB_CAM_ID_DONE)) {
        subCamEye = this->subCamEye;
        subCamEye.y += sp58 * D_809DF5B0;
        Play_SetCameraAtEyeUp(play, this->subCamId, &this->subCamAt, &subCamEye, &this->subCamUp);
        this->subCamUp.z = this->subCamUp.x = 0.0f;
        this->subCamUp.y = 1.0f;
        ShrinkWindow_Letterbox_SetSizeTarget(27);
    }
}
