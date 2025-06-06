/*
 * File: z_en_ma4.c
 * Overlay: ovl_En_Ma4
 * Description: Romani
 */

#include "z_en_ma4.h"

#define FLAGS (ACTOR_FLAG_1 | ACTOR_FLAG_8 | ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_2000000)

#define THIS ((EnMa4*)thisx)

void EnMa4_Init(Actor* thisx, PlayState* play);
void EnMa4_Destroy(Actor* thisx, PlayState* play);
void EnMa4_Update(Actor* thisx, PlayState* play);
void EnMa4_Draw(Actor* thisx, PlayState* play);

void EnMa4_SetupWait(EnMa4* this);
void EnMa4_Wait(EnMa4* this, PlayState* play);
void EnMa4_HandlePlayerChoice(EnMa4* this, PlayState* play);
void EnMa4_ChooseNextDialogue(EnMa4* this, PlayState* play);
void EnMa4_SetupDialogueHandler(EnMa4* this);
void EnMa4_DialogueHandler(EnMa4* this, PlayState* play);
void EnMa4_SetupBeginHorsebackGame(EnMa4* this);
void EnMa4_BeginHorsebackGame(EnMa4* this, PlayState* play);
void EnMa4_HorsebackGameTalking(EnMa4* this, PlayState* play);
void EnMa4_InitHorsebackGame(EnMa4* this, PlayState* play);
void EnMa4_SetupHorsebackGameWait(EnMa4* this, PlayState* play);
void EnMa4_HorsebackGameWait(EnMa4* this, PlayState* play);
void EnMa4_SetupHorsebackGameEnd(EnMa4* this, PlayState* play);
void EnMa4_HorsebackGameEnd(EnMa4* this, PlayState* play);
void EnMa4_SetupBeginEponasSongCs(EnMa4* this);
void EnMa4_BeginEponasSongCs(EnMa4* this, PlayState* play);
void EnMa4_SetupEponasSongCs(EnMa4* this);
void EnMa4_EponasSongCs(EnMa4* this, PlayState* play);
void EnMa4_SetupEndEponasSongCs(EnMa4* this);
void EnMa4_EndEponasSongCs(EnMa4* this, PlayState* play);
void EnMa4_SetupBeginDescribeThemCs(EnMa4* this);
void EnMa4_BeginDescribeThemCs(EnMa4* this, PlayState* play);
void EnMa4_StartDialogue(EnMa4* this, PlayState* play);
void EnMa4_SetFaceExpression(EnMa4* this, s16 overrideEyeTexIndex, s16 mouthTexIndex);
void EnMa4_InitFaceExpression(EnMa4* this);

typedef enum {
    /* 0 */ MA4_TYPE_DAY1,
    /* 1 */ MA4_TYPE_ALIENS_DEFEATED,
    /* 2 */ MA4_TYPE_ALIENS_WON,
} EnMa4Type;

typedef enum {
    /* 0 */ MA4_STATE_DEFAULT,
    /* 1 */ MA4_STATE_HORSEBACKGAME,
    /* 2 */ MA4_STATE_AFTERHORSEBACKGAME,
    /* 3 */ MA4_STATE_AFTERDESCRIBETHEMCS,
} EnMa4State;

ActorInit En_Ma4_InitVars = {
    ACTOR_EN_MA4,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MA1,
    sizeof(EnMa4),
    (ActorFunc)EnMa4_Init,
    (ActorFunc)EnMa4_Destroy,
    (ActorFunc)EnMa4_Update,
    (ActorFunc)EnMa4_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 18, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 D_80AC00DC = { 0, 0, 0, 0, MASS_IMMOVABLE };

static TexturePtr sEyeTextures[] = {
    gRomaniEyeOpenTex, gRomaniEyeHalfTex, gRomaniEyeClosedTex, gRomaniEyeHappyTex, gRomaniEyeSadTex,
};

static TexturePtr sMouthTextures[] = {
    gRomaniMouthHappyTex,
    gRomaniMouthFrownTex,
    gRomaniMouthHangingOpenTex,
    gRomaniMouthSmileTex,
};

void EnMa4_UpdateEyes(EnMa4* this) {
    if (this->overrideEyeTexIndex != 0) {
        this->eyeTexIndex = this->overrideEyeTexIndex;
    } else if (DECR(this->blinkTimer) == 0) {
        this->eyeTexIndex++;
        if (this->eyeTexIndex >= 3) {
            this->blinkTimer = Rand_S16Offset(30, 30);
            this->eyeTexIndex = 0;
        }
    }
}

static AnimationSpeedInfo sAnimationInfo[] = {
    { &gRomaniIdleAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gRomaniIdleAnim, 1.0f, ANIMMODE_LOOP, -6.0f },
    { &gRomaniLookAroundAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gRomaniLookAroundAnim, 1.0f, ANIMMODE_LOOP, -6.0f },
    { &gRomaniSingStartAnim, 1.0f, ANIMMODE_ONCE, 0.0f },
    { &gRomaniSingStartAnim, 1.0f, ANIMMODE_ONCE, -6.0f },
    { &gRomaniSingLoopAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gRomaniSingLoopAnim, 1.0f, ANIMMODE_LOOP, -6.0f },
    { &gRomaniRunAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gRomaniRunAnim, 1.0f, ANIMMODE_LOOP, -6.0f },
    { &gRomaniWalkAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gRomaniWalkAnim, 1.0f, ANIMMODE_LOOP, -6.0f },
    { &gRomaniShootBowAnim, 1.0f, ANIMMODE_ONCE, 0.0f },
    { &gRomaniShootBowAnim, 1.0f, ANIMMODE_ONCE, -6.0f },
    { &gRomaniSittingAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gRomaniSittingAnim, 1.0f, ANIMMODE_LOOP, -6.0f },
    { &gRomaniSittingHeadShakeAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gRomaniSittingHeadShakeAnim, 1.0f, ANIMMODE_LOOP, -6.0f },
    { &gRomaniSittingLookDownAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gRomaniSittingLookDownAnim, 1.0f, ANIMMODE_LOOP, -6.0f },
};

void EnMa4_ChangeAnim(EnMa4* this, s32 animIndex) {
    Animation_Change(&this->skelAnime, sAnimationInfo[animIndex].animation, 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationInfo[animIndex].animation), sAnimationInfo[animIndex].mode,
                     sAnimationInfo[animIndex].morphFrames);
}

void func_80ABDD9C(EnMa4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 flag;

    if (this->unk_1D8.unk_00 == 0 &&
        ((this->skelAnime.animation == &gRomaniRunAnim) || (this->skelAnime.animation == &gRomaniLookAroundAnim) ||
         (this->skelAnime.animation == &gRomaniShootBowAnim))) {
        flag = 1;
    } else {
        flag = (this->type == MA4_TYPE_ALIENS_WON && this->actionFunc != EnMa4_DialogueHandler) ? 1 : 0;
    }

    this->unk_1D8.unk_18 = player->actor.world.pos;
    this->unk_1D8.unk_18.y -= -10.0f;
    func_800BD888(&this->actor, &this->unk_1D8, 0, flag);
}

void EnMa4_InitPath(EnMa4* this, PlayState* play) {
    Path* path;
    Vec3f nextPoint;

    path = &play->setupPathList[(this->actor.params & 0xFF00) >> 8];
    this->pathPoints = Lib_SegmentedToVirtual(path->points);
    this->pathIndex = 0;
    this->pathPointsCount = path->count;

    this->actor.home.pos.x = this->pathPoints[0].x;
    this->actor.home.pos.y = this->pathPoints[0].y;
    this->actor.home.pos.z = this->pathPoints[0].z;
    nextPoint.x = this->pathPoints[1].x;
    nextPoint.y = this->pathPoints[1].y;
    nextPoint.z = this->pathPoints[1].z;

    this->actor.world.pos = this->actor.home.pos;
    this->actor.shape.rot.y = this->actor.world.rot.y = Math_Vec3f_Yaw(&this->actor.world.pos, &nextPoint);
}

void EnMa4_Init(Actor* thisx, PlayState* play) {
    EnMa4* this = THIS;
    s32 pad;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gRomaniSkel, NULL, this->jointTable, this->morphTable, ROMANI_LIMB_MAX);

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, DamageTable_Get(0x16), &D_80AC00DC);

    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, 4);
    Actor_SetScale(&this->actor, 0.01f);

    this->actor.targetMode = 0;
    this->unk_1D8.unk_00 = 0;
    this->unk_334 = 0;
    this->hasBow = true;
    this->mouthTexIndex = 0;
    this->animTimer = 0;

    if (CURRENT_DAY == 1) {
        this->type = MA4_TYPE_DAY1;
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_22_01)) {
        this->type = MA4_TYPE_ALIENS_DEFEATED;
    } else {
        this->type = MA4_TYPE_ALIENS_WON;
        this->hasBow = false;
    }

    if (Cutscene_GetSceneLayer(play) != 0) {
        EnMa4_ChangeAnim(this, 0);
        this->state = MA4_STATE_HORSEBACKGAME;
        EnMa4_InitHorsebackGame(this, play);
    } else {
        EnMa4_InitPath(this, play);

        if (gSaveContext.save.entrance == ENTRANCE(ROMANI_RANCH, 1)) {
            EnMa4_ChangeAnim(this, 0);
            this->state = MA4_STATE_AFTERHORSEBACKGAME;
        } else if (gSaveContext.save.entrance == ENTRANCE(ROMANI_RANCH, 10)) {
            EnMa4_ChangeAnim(this, 0);
            this->state = MA4_STATE_AFTERDESCRIBETHEMCS;
        } else {
            if (this->type == MA4_TYPE_ALIENS_WON) {
                EnMa4_ChangeAnim(this, 14);
                this->actor.shape.shadowScale = 0.0f;
            } else {
                EnMa4_ChangeAnim(this, 8);
            }
            this->state = MA4_STATE_DEFAULT;
        }

        EnMa4_SetupWait(this);
    }
}

void EnMa4_Destroy(Actor* thisx, PlayState* play) {
    EnMa4* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_08_01);
}

// Running in circles in the ranch
void EnMa4_RunInCircles(EnMa4* this, PlayState* play) {
    static s32 sAnimIndex = 9;
    static s32 D_80AC0250 = 0;
    static s16 D_80AC0254 = 10;
    Vec3f sp34;
    s32 pad;
    s16 sp2E;

    if (sAnimIndex != 9 && Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
        if (sAnimIndex == 3) {
            if (D_80AC0250 < 3) {
                D_80AC0250++;
            } else {
                D_80AC0250 = 0;
                EnMa4_ChangeAnim(this, 13);
                sAnimIndex = 13;
            }
        } else {
            this->actor.speedXZ = 2.7f;
            EnMa4_ChangeAnim(this, 9);
            sAnimIndex = 9;
        }
    }

    if (sAnimIndex == 13 && Animation_OnFrame(&this->skelAnime, 37.0f)) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_EV_ROMANI_BOW_FLICK);
    }

    sp34.x = this->pathPoints[this->pathIndex].x;
    sp34.y = this->pathPoints[this->pathIndex].y;
    sp34.z = this->pathPoints[this->pathIndex].z;
    sp2E = Math_Vec3f_Yaw(&this->actor.world.pos, &sp34);
    if (Math_Vec3f_DistXZ(&this->actor.world.pos, &sp34) > 50.0f) {
        Math_SmoothStepToS(&this->actor.world.rot.y, sp2E, 10, 0x3000, 0x100);
        Math_SmoothStepToS(&this->actor.shape.rot.y, sp2E, 5, 0x3000, 0x100);
    } else {
        if ((D_80AC0254 == 0) && ((Rand_Next() % 4) == 0)) {
            this->actor.speedXZ = 0.0f;
            D_80AC0254 = 2;
            EnMa4_ChangeAnim(this, 3);
            sAnimIndex = 3;
        } else {
            if (D_80AC0254 > 0) {
                D_80AC0254--;
            }
        }

        if (this->pathIndex < this->pathPointsCount - 1) {
            this->pathIndex++;
        } else {
            this->pathIndex = 0;
        }
    }

    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, 4);
    Actor_MoveWithGravity(&this->actor);
    if (this->skelAnime.animation == &gRomaniRunAnim) {
        if (Animation_OnFrame(&this->skelAnime, 0.0f) || Animation_OnFrame(&this->skelAnime, 4.0f)) {
            Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_ROMANI_WALK);
        }
    }
}

void EnMa4_SetupWait(EnMa4* this) {
    if ((this->state != MA4_STATE_AFTERHORSEBACKGAME) && (this->state != MA4_STATE_AFTERDESCRIBETHEMCS)) {
        if (this->type != MA4_TYPE_ALIENS_WON) {
            EnMa4_ChangeAnim(this, 9);
            this->actor.speedXZ = 2.7f;
        } else {
            EnMa4_ChangeAnim(this, 15);
            this->actor.speedXZ = 0.0f;
        }
    } else {
        EnMa4_ChangeAnim(this, 1);
        this->actor.speedXZ = 0.0f;
    }

    this->actor.gravity = -0.2f;
    this->animTimer = 0;
    EnMa4_InitFaceExpression(this);
    this->actionFunc = EnMa4_Wait;
}

void EnMa4_Wait(EnMa4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 yaw = this->actor.shape.rot.y - this->actor.yawTowardsPlayer;

    if ((this->state == MA4_STATE_AFTERHORSEBACKGAME) || (this->state == MA4_STATE_AFTERDESCRIBETHEMCS)) {
        this->actor.flags |= ACTOR_FLAG_10000;
    } else if (this->type != MA4_TYPE_ALIENS_WON) {
        EnMa4_RunInCircles(this, play);
    } else if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
        this->animTimer++;
        if (this->animTimer == 5) {
            EnMa4_ChangeAnim(this, 17); // Traumatized anim
        } else if (this->animTimer == 8) {
            this->animTimer = 0;
            EnMa4_ChangeAnim(this, 15); // Sitting anim
        }
    }

    if (Actor_ProcessTalkRequest(&this->actor, &play->state) != 0) {
        EnMa4_StartDialogue(this, play);
        EnMa4_SetupDialogueHandler(this);
    } else if (this->type != MA4_TYPE_ALIENS_WON || ABS_ALT(yaw) < 0x4000) {
        if (!(player->stateFlags1 & PLAYER_STATE1_800000)) {
            func_800B8614(&this->actor, play, 100.0f);
        }
    }
}

// Chooses the next dialogue based on player's selection
void EnMa4_HandlePlayerChoice(EnMa4* this, PlayState* play) {
    if (Message_ShouldAdvance(play)) {
        switch (this->textId) {
            case 0x3339:
                if (play->msgCtx.choiceIndex == 0) {
                    func_8019F208();
                    Message_StartTextbox(play, 0x333A, &this->actor);
                    this->textId = 0x333A;
                } else {
                    func_8019F208();
                    Message_StartTextbox(play, 0x333B, &this->actor);
                    this->textId = 0x333B;
                }
                break;

            case 0x3341:
                if (play->msgCtx.choiceIndex == 0) {
                    func_8019F208();
                    SET_WEEKEVENTREG(WEEKEVENTREG_21_20);
                    Message_StartTextbox(play, 0x3343, &this->actor);
                    this->textId = 0x3343;
                } else {
                    func_8019F230();
                    EnMa4_SetFaceExpression(this, 0, 1);
                    Message_StartTextbox(play, 0x3342, &this->actor);
                    this->textId = 0x3342;
                    this->state = MA4_STATE_DEFAULT;
                    func_80151BB4(play, 5);
                }
                break;

            case 0x3346:
                if (play->msgCtx.choiceIndex == 0) {
                    func_8019F208();
                    SET_WEEKEVENTREG(WEEKEVENTREG_21_20);
                    Message_StartTextbox(play, 0x3343, &this->actor);
                    this->textId = 0x3343;
                } else {
                    func_8019F230();
                    EnMa4_SetFaceExpression(this, 0, 1);
                    Message_StartTextbox(play, 0x3342, &this->actor);
                    this->textId = 0x3342;
                }
                break;

            case 0x3347:
                if (play->msgCtx.choiceIndex == 0) {
                    func_8019F208();
                    Message_StartTextbox(play, 0x3349, &this->actor);
                    this->textId = 0x3349;
                } else {
                    func_8019F230();
                    Message_StartTextbox(play, 0x3348, &this->actor);
                    this->textId = 0x3348;
                    func_80151BB4(play, 5);
                }
                break;

            case 0x334D:                             // Asks if you understood her explanation.
                if (play->msgCtx.choiceIndex == 0) { // Yes
                    s32 aux;

                    func_8019F208();
                    Message_StartTextbox(play, 0x334E, &this->actor);
                    this->textId = 0x334E;
                    if (CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
                        func_80151BB4(play, 0x1C);
                    }
                    func_80151BB4(play, 5);
                } else { // No.
                    func_8019F230();
                    EnMa4_SetFaceExpression(this, 0, 0);
                    Message_StartTextbox(play, 0x334C, &this->actor);
                    this->textId = 0x334C;
                }
                break;

            case 0x3354:
                if (play->msgCtx.choiceIndex == 0) {
                    func_8019F208();
                    Message_StartTextbox(play, 0x3349, &this->actor);
                    this->textId = 0x3349;
                } else {
                    func_8019F230();
                    EnMa4_SetFaceExpression(this, 1, 0);
                    Message_StartTextbox(play, 0x3355, &this->actor);
                    this->textId = 0x3355;
                    func_80151BB4(play, 5);
                }
                break;

            case 0x3356:
                // "Try again?"
                if (play->msgCtx.choiceIndex == 0) { // Yes
                    func_8019F208();
                    func_801477B4(play);
                    EnMa4_SetupBeginHorsebackGame(this);
                } else { // No
                    if (this->type == MA4_TYPE_ALIENS_DEFEATED) {
                        func_8019F230();
                        EnMa4_SetFaceExpression(this, 3, 3);
                        Message_StartTextbox(play, 0x3357, &this->actor);
                        this->textId = 0x3357;
                        func_80151BB4(play, 5);
                    } else {
                        func_8019F230();
                        EnMa4_SetFaceExpression(this, 4, 2);
                        Message_StartTextbox(play, 0x335B, &this->actor);
                        this->textId = 0x335B;
                        func_80151BB4(play, 5);
                    }
                }
                break;

            case 0x3359:
                if (play->msgCtx.choiceIndex == 0) {
                    func_8019F208();
                    Message_StartTextbox(play, 0x3349, &this->actor);
                    this->textId = 0x3349;
                } else {
                    func_8019F230();
                    EnMa4_SetFaceExpression(this, 4, 2);
                    Message_StartTextbox(play, 0x335A, &this->actor);
                    this->textId = 0x335A;
                    func_80151BB4(play, 5);
                }
                break;
        }
    }
}

void EnMa4_ChooseNextDialogue(EnMa4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 aux;

    if (Message_ShouldAdvance(play)) {
        switch (this->textId) {
            case 0x2390:
                func_801477B4(play);
                EnMa4_SetupBeginHorsebackGame(this);
                break;

            case 0x3335:
                EnMa4_SetFaceExpression(this, 0, 3);
                Message_StartTextbox(play, 0x3336, &this->actor);
                this->textId = 0x3336;
                func_80151BB4(play, 5);
                break;

            case 0x3338:
                Message_StartTextbox(play, 0x3339, &this->actor);
                this->textId = 0x3339;
                break;

            case 0x333A:
            case 0x333B:
                EnMa4_SetFaceExpression(this, 0, 3);
                Message_StartTextbox(play, 0x333C, &this->actor);
                this->textId = 0x333C;
                break;

            case 0x333C:
                EnMa4_SetFaceExpression(this, 0, 2);
                Message_StartTextbox(play, 0x333D, &this->actor);
                this->textId = 0x333D;
                break;

            case 0x333D:
                Message_StartTextbox(play, 0x333E, &this->actor);
                this->textId = 0x333E;
                break;

            case 0x333E:
                func_801477B4(play);
                EnMa4_SetupBeginDescribeThemCs(this);
                break;

            case 0x333F:
                Message_StartTextbox(play, 0x3340, &this->actor);
                this->textId = 0x3340;
                break;

            case 0x3340:
                EnMa4_SetFaceExpression(this, 0, 3);
                Message_StartTextbox(play, 0x3341, &this->actor);
                this->textId = 0x3341;
                break;

            case 0x3343:
                EnMa4_SetFaceExpression(this, 0, 0);
                Message_StartTextbox(play, 0x3344, &this->actor);
                this->textId = 0x3344;
                break;

            case 0x3344:
                Message_StartTextbox(play, 0x3345, &this->actor);
                this->textId = 0x3345;
                break;

            case 0x3345:
                Message_StartTextbox(play, 0x3349, &this->actor);
                this->textId = 0x3349;
                break;

            case 0x3349:
                Message_StartTextbox(play, 0x334A, &this->actor);
                this->textId = 0x334A;
                break;

            case 0x334A:
                func_801477B4(play);
                EnMa4_SetupBeginHorsebackGame(this);
                break;

            case 0x334C:
                Message_StartTextbox(play, 0x334D, &this->actor);
                this->textId = 0x334D;
                break;

            case 0x3350:
            case 0x3351:
            case 0x335D:
            case 0x335E:
                if (this->type == MA4_TYPE_DAY1) {
                    Message_StartTextbox(play, 0x3352, &this->actor);
                    this->textId = 0x3352;
                } else {
                    Message_StartTextbox(play, 0x3356, &this->actor);
                    this->textId = 0x3356;
                }
                break;

            case 0x3352: // Win minigame
            case 0x336D: // Lose minigame
                EnMa4_SetFaceExpression(this, 0, 0);

                // Check if player has Epona's song
                if (CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
                    Message_StartTextbox(play, 0x334C, &this->actor);
                    this->textId = 0x334C;
                } else {
                    func_801477B4(play);
                    player->stateFlags1 |= PLAYER_STATE1_20;
                    EnMa4_SetupBeginEponasSongCs(this);
                    EnMa4_BeginEponasSongCs(this, play);
                }
                break;

            case 0x3358:
                if ((gSaveContext.save.playerForm != PLAYER_FORM_HUMAN) || !CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
                    Message_StartTextbox(play, 0x335C, &this->actor);
                    this->textId = 0x335C;
                    func_80151BB4(play, 5);
                } else {
                    Message_StartTextbox(play, 0x3359, &this->actor);
                    this->textId = 0x3359;
                }
                break;
        }
    }
}

void EnMa4_SetupDialogueHandler(EnMa4* this) {
    if (this->type != MA4_TYPE_ALIENS_WON) {
        EnMa4_ChangeAnim(this, 1);
    } else {
        EnMa4_ChangeAnim(this, 15);
    }
    this->actionFunc = EnMa4_DialogueHandler;
}

void EnMa4_DialogueHandler(EnMa4* this, PlayState* play) {
    switch (Message_GetState(&play->msgCtx)) {
        default:
            break;

        case TEXT_STATE_CHOICE: // Player answered a question
            EnMa4_HandlePlayerChoice(this, play);
            break;

        case TEXT_STATE_5: // End message block
            EnMa4_ChooseNextDialogue(this, play);
            break;

        case TEXT_STATE_DONE: // End conversation
            if (Message_ShouldAdvance(play)) {
                if ((play->msgCtx.unk120B1 == 0) || !CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
                    EnMa4_SetupWait(this);
                }
            }

        case TEXT_STATE_NONE:
        case TEXT_STATE_1:
        case TEXT_STATE_CLOSING:
        case TEXT_STATE_3:
            break;
    }

    if (this->type != MA4_TYPE_ALIENS_WON) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 5, 2000, 1000);
    }
}

void EnMa4_SetupBeginHorsebackGame(EnMa4* this) {
    this->actionFunc = EnMa4_BeginHorsebackGame;
}

void EnMa4_BeginHorsebackGame(EnMa4* this, PlayState* play) {
    play->nextEntrance = ENTRANCE(ROMANI_RANCH, 0);
    gSaveContext.nextCutsceneIndex = 0xFFF0;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_80;
    gSaveContext.nextTransitionType = TRANS_TYPE_03;
}

void EnMa4_HorsebackGameCheckPlayerInteractions(EnMa4* this, PlayState* play) {
    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        // "You're feeling confident"
        Message_StartTextbox(play, 0x336E, &this->actor);
        this->actionFunc = EnMa4_HorsebackGameTalking;
    } else if (gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] < SECONDS_TO_TIMER(115)) {
        func_800B8614(&this->actor, play, 100.0f);
    }
}

void EnMa4_HorsebackGameTalking(EnMa4* this, PlayState* play) {
    if (Actor_TextboxIsClosing(&this->actor, play)) {
        this->actionFunc = EnMa4_HorsebackGameWait;
    }
}

void EnMa4_InitHorsebackGame(EnMa4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    play->interfaceCtx.minigameState = MINIGAME_STATE_COUNTDOWN_SETUP_3;
    Interface_StartTimer(TIMER_ID_MINIGAME_2, 0);
    SET_WEEKEVENTREG(WEEKEVENTREG_08_01);
    Interface_InitMinigame(play);
    player->stateFlags1 |= PLAYER_STATE1_20;
    this->actionFunc = EnMa4_SetupHorsebackGameWait;
}

void EnMa4_SetupHorsebackGameWait(EnMa4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->interfaceCtx.minigameState == MINIGAME_STATE_COUNTDOWN_GO) {
        this->actionFunc = EnMa4_HorsebackGameWait;
        player->stateFlags1 &= ~PLAYER_STATE1_20;
    }
}

void EnMa4_HorsebackGameWait(EnMa4* this, PlayState* play) {
    static s16 D_80AC0258 = 0;
    Player* player = GET_PLAYER(play);

    player->stateFlags3 |= PLAYER_STATE3_400;
    EnMa4_HorsebackGameCheckPlayerInteractions(this, play);

    if (this->poppedBalloonCounter != D_80AC0258) {
        D_80AC0258 = this->poppedBalloonCounter;
        play->interfaceCtx.minigamePoints = 1;
    }

    if ((gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] >= SECONDS_TO_TIMER(120)) ||
        (this->poppedBalloonCounter == 10)) {
        gSaveContext.timerStates[TIMER_ID_MINIGAME_2] = TIMER_STATE_6;
        EnMa4_SetupHorsebackGameEnd(this, play);
        D_80AC0258 = 0;
    }
}

void EnMa4_SetupHorsebackGameEnd(EnMa4* this, PlayState* play) {
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_08_01);
    this->actionFunc = EnMa4_HorsebackGameEnd;
    Audio_QueueSeqCmd(NA_BGM_STOP);
    Audio_QueueSeqCmd(NA_BGM_HORSE_GOAL | 0x8000);
}

void EnMa4_HorsebackGameEnd(EnMa4* this, PlayState* play) {
    static s32 sFrameCounter = 0;
    Player* player = GET_PLAYER(play);

    if (player->stateFlags1 & PLAYER_STATE1_100000) {
        play->actorCtx.unk268 = 1;
        play->actorCtx.unk_26C.press.button = BTN_A;
    } else {
        play->actorCtx.unk268 = 1;
    }

    if (sFrameCounter == 25) {
        if (this->poppedBalloonCounter == 10) {
            Message_StartTextbox(play, 0x334F, &this->actor);
            this->textId = 0x334F;
        } else {
            Message_StartTextbox(play, 0x334B, &this->actor);
            this->textId = 0x334B;
        }
    } else if (sFrameCounter == 50) {
        play->actorCtx.unk268 = 0;
        play->nextEntrance = ENTRANCE(ROMANI_RANCH, 1);
        gSaveContext.nextCutsceneIndex = 0;
        sFrameCounter = 0;
        play->transitionTrigger = TRANS_TRIGGER_START;

        if (this->poppedBalloonCounter == 10) {
            play->transitionType = TRANS_TYPE_80;
            gSaveContext.nextTransitionType = TRANS_TYPE_03;
        } else {
            play->transitionType = TRANS_TYPE_64;
            gSaveContext.nextTransitionType = TRANS_TYPE_02;
        }

        this->poppedBalloonCounter = 0;
    }

    sFrameCounter++;
}

void EnMa4_SetupBeginEponasSongCs(EnMa4* this) {
    this->actionFunc = EnMa4_BeginEponasSongCs;
}

// Epona's Song cutscene is an ActorCutscene
void EnMa4_BeginEponasSongCs(EnMa4* this, PlayState* play) {
    s16 cutsceneIndex = this->actor.cutscene;

    if (ActorCutscene_GetCanPlayNext(cutsceneIndex) != 0) {
        ActorCutscene_Start(cutsceneIndex, &this->actor);
        EnMa4_SetupEponasSongCs(this);
    } else {
        if (ActorCutscene_GetCurrentIndex() == 0x7C) {
            ActorCutscene_Stop(0x7C);
        }
        ActorCutscene_SetIntentToPlay(cutsceneIndex);
    }
}

void EnMa4_SetupEponasSongCs(EnMa4* this) {
    this->animTimer = 0;
    this->actionFunc = EnMa4_EponasSongCs;
}

static u16 D_80AC0260 = 99;
void EnMa4_EponasSongCs(EnMa4* this, PlayState* play) {
    if (Cutscene_CheckActorAction(play, 120)) {
        s32 actionIndex = Cutscene_GetActorActionIndex(play, 120);

        if (play->csCtx.frames == play->csCtx.actorActions[actionIndex]->startFrame) {
            if (play->csCtx.actorActions[actionIndex]->action != D_80AC0260) {
                D_80AC0260 = play->csCtx.actorActions[actionIndex]->action;
                this->animTimer = 0;

                switch (play->csCtx.actorActions[actionIndex]->action) {
                    case 1:
                        this->hasBow = true;
                        EnMa4_ChangeAnim(this, 1);
                        break;

                    case 2:
                        this->hasBow = false;
                        EnMa4_ChangeAnim(this, 4);
                        break;
                }
            }
        }

        Cutscene_ActorTranslateAndYaw(&this->actor, play, actionIndex);
        if (D_80AC0260 == 2 && this->animTimer == 0 && Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
            EnMa4_ChangeAnim(this, 7);
        }
    } else {
        Player* player = GET_PLAYER(play);

        player->stateFlags1 |= PLAYER_STATE1_20;
        func_800B85E0(&this->actor, play, 200.0f, PLAYER_IA_MINUS1);
        D_80AC0260 = 99;
        this->hasBow = true;
        EnMa4_SetupEndEponasSongCs(this);
    }
}

void EnMa4_SetupEndEponasSongCs(EnMa4* this) {
    this->actionFunc = EnMa4_EndEponasSongCs;
}

void EnMa4_EndEponasSongCs(EnMa4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->actor.flags |= ACTOR_FLAG_10000;
    if (Actor_ProcessTalkRequest(&this->actor, &play->state) != 0) {
        player->stateFlags1 &= ~PLAYER_STATE1_20;
        Message_StartTextbox(play, 0x334C, &this->actor);
        this->textId = 0x334C;
        this->actor.flags &= ~ACTOR_FLAG_10000;
        EnMa4_SetupDialogueHandler(this);
    } else {
        func_800B85E0(&this->actor, play, 200.0f, PLAYER_IA_MINUS1);
    }
}

void EnMa4_SetupBeginDescribeThemCs(EnMa4* this) {
    this->actionFunc = EnMa4_BeginDescribeThemCs;
}

void EnMa4_BeginDescribeThemCs(EnMa4* this, PlayState* play) {
    play->nextEntrance = ENTRANCE(ROMANI_RANCH, 0);
    gSaveContext.nextCutsceneIndex = 0xFFF5;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_64;
    gSaveContext.nextTransitionType = TRANS_TYPE_02;
}

void EnMa4_StartDialogue(EnMa4* this, PlayState* play) {
    s32 pad;
    OSTime time;

    switch (this->type) {
        case MA4_TYPE_DAY1:
            if (gSaveContext.save.playerForm != PLAYER_FORM_HUMAN) {
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_21_80)) {
                    EnMa4_SetFaceExpression(this, 3, 3);
                    Message_StartTextbox(play, 0x3337, &this->actor);
                    this->textId = 0x3337;
                    func_80151BB4(play, 5);
                } else {
                    Message_StartTextbox(play, 0x3335, &this->actor);
                    this->textId = 0x3335;
                    SET_WEEKEVENTREG(WEEKEVENTREG_21_80);
                }
            } else if (this->state == MA4_STATE_DEFAULT) {
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_21_40)) {
                    if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_21_20)) {
                        Message_StartTextbox(play, 0x3346, &this->actor);
                        this->textId = 0x3346;
                    } else {
                        Message_StartTextbox(play, 0x3347, &this->actor);
                        this->textId = 0x3347;
                    }
                } else {
                    Message_StartTextbox(play, 0x3338, &this->actor);
                    this->textId = 0x3338;
                    SET_WEEKEVENTREG(WEEKEVENTREG_21_40);
                }
            } else if (this->state == MA4_STATE_AFTERHORSEBACKGAME) {
                if (gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] >= SECONDS_TO_TIMER(120)) {
                    // "Too bad Grasshopper"
                    EnMa4_SetFaceExpression(this, 0, 0);
                    Message_StartTextbox(play, 0x336D, &this->actor);
                    this->textId = 0x336D;
                } else {
                    time = gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2];
                    if ((s32)time < (s32)gSaveContext.save.horseBackBalloonHighScore) {
                        // [Score] New record!
                        gSaveContext.save.horseBackBalloonHighScore = time;
                        EnMa4_SetFaceExpression(this, 0, 3);
                        Message_StartTextbox(play, 0x3350, &this->actor);
                        this->textId = 0x3350;
                    } else {
                        // [Score] Great.
                        Message_StartTextbox(play, 0x3351, &this->actor);
                        this->textId = 0x3351;
                    }
                }
                this->state = MA4_STATE_DEFAULT;
                this->actor.flags &= ~ACTOR_FLAG_10000;
            } else if (this->state == MA4_STATE_AFTERDESCRIBETHEMCS) {
                // "Cremia doesn't believe me..."
                Message_StartTextbox(play, 0x3340, &this->actor);
                this->textId = 0x3340;
                this->actor.flags &= ~ACTOR_FLAG_10000;
            }
            break;

        case MA4_TYPE_ALIENS_DEFEATED:
            if (gSaveContext.save.playerForm != PLAYER_FORM_HUMAN) {
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_21_80)) {
                    EnMa4_SetFaceExpression(this, 3, 3);
                    Message_StartTextbox(play, 0x3337, &this->actor);
                    this->textId = 0x3337;
                    func_80151BB4(play, 5);
                } else {
                    Message_StartTextbox(play, 0x3335, &this->actor);
                    this->textId = 0x3335;
                    SET_WEEKEVENTREG(WEEKEVENTREG_21_80);
                }
            } else if (this->state == MA4_STATE_DEFAULT) {
                Message_StartTextbox(play, 0x3354, &this->actor);
                this->textId = 0x3354;
            } else if (this->state == MA4_STATE_AFTERHORSEBACKGAME) {
                if (gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] >= SECONDS_TO_TIMER(120)) {
                    // "Try again?"
                    Message_StartTextbox(play, 0x3356, &this->actor);
                    this->textId = 0x3356;
                } else {
                    time = gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2];
                    if ((s32)time < (s32)gSaveContext.save.horseBackBalloonHighScore) {
                        gSaveContext.save.horseBackBalloonHighScore = time;
                        EnMa4_SetFaceExpression(this, 0, 3);
                        Message_StartTextbox(play, 0x3350, &this->actor);
                        this->textId = 0x3350;
                    } else {
                        Message_StartTextbox(play, 0x3351, &this->actor);
                        this->textId = 0x3351;
                    }
                }
                this->state = MA4_STATE_DEFAULT;
                this->actor.flags &= ~ACTOR_FLAG_10000;
            }
            break;

        case MA4_TYPE_ALIENS_WON:
            if (this->state == MA4_STATE_DEFAULT) {
                // "Huh... You?"
                Message_StartTextbox(play, 0x3358, &this->actor);
                this->textId = 0x3358;
            } else if (this->state == MA4_STATE_AFTERHORSEBACKGAME) {
                if (gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] >= SECONDS_TO_TIMER(120)) {
                    // "Try again?"
                    Message_StartTextbox(play, 0x3356, &this->actor);
                    this->textId = 0x3356;
                } else {
                    time = gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2];
                    if ((s32)time < (s32)gSaveContext.save.horseBackBalloonHighScore) {
                        // New record
                        gSaveContext.save.horseBackBalloonHighScore = time;
                        Message_StartTextbox(play, 0x335D, &this->actor);
                        this->textId = 0x335D;
                    } else {
                        // "Old record was: [record]"
                        Message_StartTextbox(play, 0x335E, &this->actor);
                        this->textId = 0x335E;
                    }
                }
                this->state = MA4_STATE_DEFAULT;
                this->actor.flags &= ~ACTOR_FLAG_10000;
            }
            break;

        default:
            // Dead code
            Message_StartTextbox(play, 0x3335, &this->actor);
            this->textId = 0x3335;
            break;
    }
}

void EnMa4_SetFaceExpression(EnMa4* this, s16 overrideEyeTexIndex, s16 mouthTexIndex) {
    this->overrideEyeTexIndex = overrideEyeTexIndex;
    this->mouthTexIndex = mouthTexIndex;
}

void EnMa4_InitFaceExpression(EnMa4* this) {
    if (this->type != MA4_TYPE_ALIENS_WON) {
        EnMa4_SetFaceExpression(this, 0, 0);
    } else {
        EnMa4_SetFaceExpression(this, 1, 2);
    }
}

void EnMa4_Update(Actor* thisx, PlayState* play) {
    EnMa4* this = THIS;
    s32 pad;

    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    SkelAnime_Update(&this->skelAnime);
    EnMa4_UpdateEyes(this);
    this->actionFunc(this, play);
    func_80ABDD9C(this, play);
}

s32 EnMa4_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    EnMa4* this = THIS;
    Vec3s sp4;

    if (limbIndex == ROMANI_LIMB_HEAD) {
        sp4 = this->unk_1D8.unk_08;
        rot->x = rot->x + sp4.y;
        rot->z = rot->z + sp4.x;
    }
    if (limbIndex == ROMANI_LIMB_TORSO) {
        sp4 = this->unk_1D8.unk_0E;
        rot->x = rot->x - sp4.y;
        rot->z = rot->z - sp4.x;
    }

    return false;
}

void EnMa4_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    EnMa4* this = THIS;
    Vec3f sp28 = { 800.0f, 0.0f, 0.0f };

    if (limbIndex == ROMANI_LIMB_HEAD) {
        Matrix_MultVec3f(&sp28, &this->actor.focus.pos);
    } else if (limbIndex == ROMANI_LIMB_LEFT_HAND) {
        if (this->hasBow == true) {
            OPEN_DISPS(play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, gRomaniBowDL);
            CLOSE_DISPS(play->state.gfxCtx);
        }
    }
}

void EnMa4_Draw(Actor* thisx, PlayState* play) {
    EnMa4* this = THIS;

    OPEN_DISPS(play->state.gfxCtx);
    if (this->type == MA4_TYPE_ALIENS_WON) {
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gRomaniWoodenBoxDL);
    }

    func_8012C28C(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[this->eyeTexIndex]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sMouthTextures[this->mouthTexIndex]));

    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          EnMa4_OverrideLimbDraw, EnMa4_PostLimbDraw, &this->actor);

    CLOSE_DISPS(play->state.gfxCtx);
}
