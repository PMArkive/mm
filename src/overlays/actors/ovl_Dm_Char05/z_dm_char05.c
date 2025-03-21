/*
 * File: z_dm_char05.c
 * Overlay: ovl_Dm_Char05
 * Description: Cutscene mask objects
 */

#include "z_dm_char05.h"
#include "objects/object_dmask/object_dmask.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((DmChar05*)thisx)

void DmChar05_Init(Actor* thisx, PlayState* play);
void DmChar05_Destroy(Actor* thisx, PlayState* play);
void DmChar05_Update(Actor* thisx, PlayState* play);
void DmChar05_Draw(Actor* thisx, PlayState* play);

void func_80AAC990(DmChar05* this, PlayState* play);
void func_80AAC9DC(DmChar05* this, PlayState* play);
void func_80AACA98(DmChar05* this, PlayState* play);
void func_80AACAE4(DmChar05* this, PlayState* play);
void func_80AACBE4(DmChar05* this, PlayState* play);
void func_80AACC48(DmChar05* this, PlayState* play);
void func_80AACC6C(DmChar05* this, PlayState* play);
void func_80AACCD4(DmChar05* this, PlayState* play);
void func_80AACD0C(DmChar05* this, PlayState* play);
void func_80AACD1C(DmChar05* this, PlayState* play);
void func_80AACD68(DmChar05* this, PlayState* play);
void func_80AACE10(DmChar05* this, PlayState* play);
void func_80AACE5C(DmChar05* this, PlayState* play);
void func_80AADD9C(PlayState* play, DmChar05* this);
void func_80AADE78(PlayState* play, DmChar05* this);
void func_80AADF54(PlayState* play, DmChar05* this);
void func_80AAE030(PlayState* play, DmChar05* this);
void func_80AAE114(PlayState* play, DmChar05* this);

ActorInit Dm_Char05_InitVars = {
    ACTOR_DM_CHAR05,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_DMASK,
    sizeof(DmChar05),
    (ActorFunc)DmChar05_Init,
    (ActorFunc)DmChar05_Destroy,
    (ActorFunc)DmChar05_Update,
    (ActorFunc)DmChar05_Draw,
};

static AnimationInfo sAnimationInfo[] = {
    { &object_dmask_Anim_001090, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    { &object_dmask_Anim_004288, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    { &object_dmask_Anim_0001A8, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    { &object_dmask_Anim_00017C, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    { &object_dmask_Anim_0011A0, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    { &object_dmask_Anim_0013A4, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
};

void func_80AAC5A0(SkelAnime* skelAnime, AnimationInfo* animation, u16 arg2) {
    f32 phi_f2;

    animation += arg2;

    if (animation->frameCount < 0.0f) {
        phi_f2 = Animation_GetLastFrame(animation->animation);
    } else {
        phi_f2 = animation->frameCount;
    }
    Animation_Change(skelAnime, animation->animation, animation->playSpeed, animation->startFrame, phi_f2,
                     animation->mode, animation->morphFrames);
}

void func_80AAC63C(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_Init(play, &this->skelAnime, &object_dmask_Skel_010B0, NULL, NULL, NULL, 0);
    if (DMCHAR05_GET(&this->actor) == DMCHAR05_0) {
        func_80AAC5A0(&this->skelAnime, &sAnimationInfo[0], 0);
    } else {
        func_80AAC5A0(&this->skelAnime, &sAnimationInfo[1], 0);
    }
    this->actionFunc = func_80AACC48;
}

void func_80AAC6E4(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_Init(play, &this->skelAnime, &object_dmask_Skel_042B0, NULL, NULL, NULL, 0);
    func_80AAC5A0(&this->skelAnime, &sAnimationInfo[1], 0);
    this->actionFunc = func_80AACC48;
}

void func_80AAC770(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &object_dmask_Skel_001D0, NULL, NULL, NULL, 0);
    func_80AAC5A0(&this->skelAnime, &sAnimationInfo[3], 0);
    this->actionFunc = func_80AACC48;
}

void func_80AAC7FC(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_Init(play, &this->skelAnime, &object_dmask_Skel_013D0, NULL, NULL, NULL, 0);
    func_80AAC5A0(&this->skelAnime, &sAnimationInfo[4], 0);
    this->actionFunc = func_80AACC48;
}

void func_80AAC888(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    this->actionFunc = func_80AACA98;
}

void DmChar05_Init(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    this->unk_18C = 0;
    this->unk_18E = 0;
    this->unk_19C = 0;
    this->unk_1A0 = 0;
    this->unk_1A4 = 0;
    this->unk_1A8 = 0;
    this->unk_1AC = 100;
    this->unk_1B0 = 1000;
    this->unk_1B4 = 0;

    switch (DMCHAR05_GET(&this->actor)) {
        case DMCHAR05_0:
            func_80AAC63C(thisx, play);
            break;

        case DMCHAR05_1:
            func_80AAC6E4(thisx, play);
            break;

        case DMCHAR05_2:
            func_80AAC770(thisx, play);
            break;

        case DMCHAR05_3:
            func_80AAC7FC(thisx, play);
            break;

        case DMCHAR05_4:
            func_80AAC888(thisx, play);
            break;

        case DMCHAR05_5:
        case DMCHAR05_6:
        case DMCHAR05_7:
        case DMCHAR05_8:
        case DMCHAR05_9:
        case DMCHAR05_10:
        case DMCHAR05_11:
        case DMCHAR05_12:
            this->actionFunc = func_80AACBE4;
            break;
    }
    Actor_SetScale(&this->actor, 0.01f);
}

void DmChar05_Destroy(Actor* thisx, PlayState* play) {
}

void func_80AAC990(DmChar05* this, PlayState* play) {
    s32 objectIdx = Object_GetIndex(&play->objectCtx, OBJECT_GI_GOLONMASK);

    if (objectIdx >= 0) {
        this->unk_18F = objectIdx;
        this->actionFunc = func_80AAC9DC;
    }
}

void func_80AAC9DC(DmChar05* this, PlayState* play) {
    if (Cutscene_CheckActorAction(play, 109) != 0) {
        s32 actionIndex = Cutscene_GetActorActionIndex(play, 109);

        if (play->csCtx.actorActions[actionIndex]->action == 4) {
            this->unk_18E = 1;
            this->unk_190.x = play->csCtx.actorActions[actionIndex]->startPos.x;
            this->unk_190.y = play->csCtx.actorActions[actionIndex]->startPos.y;
            this->unk_190.z = play->csCtx.actorActions[actionIndex]->startPos.z;
        }
    }
}

void func_80AACA98(DmChar05* this, PlayState* play) {
    s32 objectIdx = Object_GetIndex(&play->objectCtx, OBJECT_GI_MASK13);

    if (objectIdx >= 0) {
        this->unk_18F = objectIdx;
        this->actionFunc = func_80AACAE4;
    }
}

void func_80AACAE4(DmChar05* this, PlayState* play) {
    if (Cutscene_CheckActorAction(play, 564)) {
        s32 actionIndex = Cutscene_GetActorActionIndex(play, 564);

        if (play->csCtx.actorActions[actionIndex]->action == 2) {
            if (play->csCtx.frames == play->csCtx.actorActions[actionIndex]->startFrame) {
                Item_Give(play, ITEM_MASK_COUPLE);
            }
            this->unk_18E = 1;
            this->unk_190.x = play->csCtx.actorActions[actionIndex]->startPos.x;
            this->unk_190.y = play->csCtx.actorActions[actionIndex]->startPos.y;
            this->unk_190.z = play->csCtx.actorActions[actionIndex]->startPos.z;
        } else {
            this->unk_18E = 0;
        }
    }
}

void func_80AACBE4(DmChar05* this, PlayState* play) {
    static s16 D_80AAE2F0[] = {
        OBJECT_GI_MASK17, OBJECT_GI_MASK14, OBJECT_GI_MASK10,     OBJECT_GI_MASK21,
        OBJECT_GI_MASK11, OBJECT_GI_MASK20, OBJECT_GI_RABIT_MASK, OBJECT_GI_MASK12,
    };
    s32 params = DMCHAR05_GET(&this->actor) - DMCHAR05_5;
    s32 objectIdx = Object_GetIndex(&play->objectCtx, D_80AAE2F0[params]);

    if (objectIdx >= 0) {
        this->unk_18F = objectIdx;
        this->actionFunc = func_80AACD0C;
    }
}

void func_80AACC48(DmChar05* this, PlayState* play) {
    if (this->unk_1B4 != 0) {
        this->actionFunc = func_80AACC6C;
    }
}

void func_80AACC6C(DmChar05* this, PlayState* play) {
    if (this->unk_19C < 247) {
        this->unk_19C += 8;
    }

    if (this->unk_1A0 < 247) {
        this->unk_1A0 += 8;
    }

    if (this->unk_1A4 < 247) {
        this->unk_1A4 += 8;
    } else {
        this->unk_19C = 255;
        this->unk_1A0 = 255;
        this->unk_1A4 = 255;
        this->actionFunc = func_80AACCD4;
    }
}

void func_80AACCD4(DmChar05* this, PlayState* play) {
    if (this->unk_1AC < 975) {
        this->unk_1AC += 25;
    } else {
        this->unk_1AC = 996;
        this->actionFunc = func_80AACD0C;
    }
}

void func_80AACD0C(DmChar05* this, PlayState* play) {
}

void func_80AACD1C(DmChar05* this, PlayState* play) {
    s32 objectIdx = Object_GetIndex(&play->objectCtx, OBJECT_GI_ZORAMASK);

    if (objectIdx >= 0) {
        this->unk_18F = objectIdx;
        this->actionFunc = func_80AACD68;
    }
}

void func_80AACD68(DmChar05* this, PlayState* play) {
    if (Cutscene_CheckActorAction(play, 473)) {
        s32 actionIndex = Cutscene_GetActorActionIndex(play, 473);

        this->unk_18E = 1;
        this->unk_190.x = play->csCtx.actorActions[actionIndex]->startPos.x;
        this->unk_190.y = play->csCtx.actorActions[actionIndex]->startPos.y;
        this->unk_190.z = play->csCtx.actorActions[actionIndex]->startPos.z;
    }
}

void func_80AACE10(DmChar05* this, PlayState* play) {
    s32 objectIdx = Object_GetIndex(&play->objectCtx, OBJECT_GI_MASK15);

    if (objectIdx >= 0) {
        this->unk_18F = objectIdx;
        this->actionFunc = func_80AACE5C;
    }
}

void func_80AACE5C(DmChar05* this, PlayState* play) {
    if (Cutscene_CheckActorAction(play, 518)) {
        s32 actionIndex = Cutscene_GetActorActionIndex(play, 518);

        this->unk_18E = 1;
        this->unk_190.x = play->csCtx.actorActions[actionIndex]->startPos.x;
        this->unk_190.y = play->csCtx.actorActions[actionIndex]->startPos.y;
        this->unk_190.z = play->csCtx.actorActions[actionIndex]->startPos.z;
    }
}

void func_80AACF04(DmChar05* this, PlayState* play) {
    u8 sp2F = true;
    s32 actionIndex;

    switch (DMCHAR05_GET(&this->actor)) {
        case DMCHAR05_0:
            if (Cutscene_CheckActorAction(play, 109)) {
                actionIndex = Cutscene_GetActorActionIndex(play, 109);

                if (play->csCtx.frames == play->csCtx.actorActions[actionIndex]->startFrame) {
                    switch (play->csCtx.actorActions[actionIndex]->action) {
                        case 1:
                            sp2F = false;
                            break;

                        case 2:
                            this->unk_1B4 = 1;
                            break;

                        case 3:
                            this->unk_18C = 0;
                            break;

                        case 4:
                            Item_Give(play, ITEM_MASK_GORON);
                            sp2F = false;
                            this->actionFunc = func_80AAC990;
                            break;

                        case 5:
                            sp2F = false;
                            Actor_Kill(&this->actor);
                            break;

                        default:
                            sp2F = false;
                            break;
                    }

                    if (sp2F) {
                        func_80AAC5A0(&this->skelAnime, &sAnimationInfo[this->unk_18C], 0);
                    }
                }

                Cutscene_ActorTranslateAndYaw(&this->actor, play, actionIndex);
            }
            break;

        case DMCHAR05_1:
            if (Cutscene_CheckActorAction(play, 473)) {
                actionIndex = Cutscene_GetActorActionIndex(play, 473);

                if (play->csCtx.frames == play->csCtx.actorActions[actionIndex]->startFrame) {
                    switch (play->csCtx.actorActions[actionIndex]->action) {
                        case 1:
                            sp2F = false;
                            break;

                        case 2:
                            this->unk_18C = 1;
                            this->unk_1B4 = 1;
                            break;

                        case 3:
                            this->unk_18C = 1;
                            break;

                        case 4:
                            Item_Give(play, ITEM_MASK_ZORA);
                            sp2F = false;
                            this->actionFunc = func_80AACD1C;
                            break;

                        case 5:
                            sp2F = false;
                            Actor_Kill(&this->actor);
                            break;

                        default:
                            sp2F = false;
                            break;
                    }

                    if (sp2F) {
                        func_80AAC5A0(&this->skelAnime, &sAnimationInfo[this->unk_18C], 0);
                    }
                }

                Cutscene_ActorTranslateAndYaw(&this->actor, play, actionIndex);
            }
            break;

        case DMCHAR05_2:
            if (Cutscene_CheckActorAction(play, 518)) {
                actionIndex = Cutscene_GetActorActionIndex(play, 518);

                if (play->csCtx.frames == play->csCtx.actorActions[actionIndex]->startFrame) {
                    switch (play->csCtx.actorActions[actionIndex]->action) {
                        case 1:
                            sp2F = false;
                            break;

                        case 2:
                            this->unk_18C = 3;
                            break;

                        case 3:
                            this->unk_18C = 2;
                            break;

                        case 4:
                            Item_Give(play, ITEM_MASK_GIBDO);
                            sp2F = false;
                            this->actionFunc = func_80AACE10;
                            break;

                        case 5:
                            sp2F = false;
                            Actor_Kill(&this->actor);
                            break;

                        default:
                            sp2F = false;
                            break;
                    }

                    if (sp2F) {
                        func_80AAC5A0(&this->skelAnime, &sAnimationInfo[this->unk_18C], 0);
                    }
                }

                Cutscene_ActorTranslateAndYaw(&this->actor, play, actionIndex);
            }
            break;

        case DMCHAR05_3:
            if (Cutscene_CheckActorAction(play, 559)) {
                actionIndex = Cutscene_GetActorActionIndex(play, 559);

                if (play->csCtx.frames == play->csCtx.actorActions[actionIndex]->startFrame) {
                    switch (play->csCtx.actorActions[actionIndex]->action) {
                        default:
                            sp2F = false;
                            break;

                        case 1:
                            sp2F = false;
                            break;

                        case 2:
                            this->unk_18C = 4;
                            break;

                        case 3:
                            this->unk_18C = 5;
                            break;

                        case 4:
                            this->unk_18C = 5;
                            break;
                    }

                    if (sp2F) {
                        func_80AAC5A0(&this->skelAnime, &sAnimationInfo[this->unk_18C], 0);
                    }
                }

                if (play->csCtx.actorActions[actionIndex]->action != 4) {
                    Cutscene_ActorTranslateAndYaw(&this->actor, play, actionIndex);
                } else {
                    Cutscene_ActorTranslate(&this->actor, play, actionIndex);
                }
            }

            if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                if (this->unk_18C == 4) {
                    this->unk_18C++;
                    func_80AAC5A0(&this->skelAnime, &sAnimationInfo[this->unk_18C], 0);
                }
            }
            break;
    }
}

void func_80AAD3F8(DmChar05* this, PlayState* play) {
    if (play->csCtx.frames == 490) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_EVIL_POWER);
    }

    if (play->csCtx.frames > 550) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_MASK_FLOAT - SFX_FLAG);
    }
}

void func_80AAD450(DmChar05* this, PlayState* play) {
    if (play->csCtx.frames == 262) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_EVIL_POWER);
    }

    if (play->csCtx.frames > 318) {
        Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_MASK_FLOAT - SFX_FLAG);
    }
}

void func_80AAD4A8(DmChar05* this, PlayState* play) {
    if (DMCHAR05_GET(&this->actor) == DMCHAR05_0) {
        if (Cutscene_CheckActorAction(play, 109) &&
            (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 109)]->action == 3)) {
            if (Animation_OnFrame(&this->skelAnime, 14.0f) || Animation_OnFrame(&this->skelAnime, 15.0f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_IT_MASK_BOUND_0);
            } else if (Animation_OnFrame(&this->skelAnime, 19.0f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_IT_MASK_BOUND_1);
            }
        }
    } else if (DMCHAR05_GET(&this->actor) == DMCHAR05_1) {
        if (Cutscene_CheckActorAction(play, 473)) {
            if ((play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 473)]->action == 3) &&
                Animation_OnFrame(&this->skelAnime, 5.0f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_IT_MASK_BOUND_SAND);
            }
        }
    } else if (DMCHAR05_GET(&this->actor) == DMCHAR05_2) {
        if (Cutscene_CheckActorAction(play, 518) &&
            (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 518)]->action == 2)) {
            if (Animation_OnFrame(&this->skelAnime, 7.0f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_IT_MASK_BOUND_0);
            }

            if (Animation_OnFrame(&this->skelAnime, 14.0f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_IT_MASK_BOUND_1);
            }

            if (Animation_OnFrame(&this->skelAnime, 18.0f)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_IT_MASK_BOUND_1);
            }
        }
    } else if (DMCHAR05_GET(&this->actor) == DMCHAR05_3) {
        if (play->sceneId == SCENE_OKUJOU) {
            if (gSaveContext.sceneLayer == 2) {
                if (play->csCtx.currentCsIndex == 0) {
                    func_80AAD3F8(this, play);
                } else if (play->csCtx.currentCsIndex == 1) {
                    func_80AAD450(this, play);
                }
            }
        } else if (play->sceneId == SCENE_SPOT00) {
            if (gSaveContext.sceneLayer == 9) {
                if ((play->csCtx.currentCsIndex == 0) && (play->csCtx.frames == 255)) {
                    Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_EVIL_POWER);
                }
            } else if ((gSaveContext.sceneLayer == 0xB) && (play->csCtx.frames == 115)) {
                Actor_PlaySfxAtPos(&this->actor, NA_SE_EN_EVIL_POWER_PREDEMO);
            }
        }
    }
}

void DmChar05_Update(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    func_80AACF04(this, play);
    if (Cutscene_CheckActorAction(play, 109)) {
        if (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 109)]->action == 3) {
            SkelAnime_Update(&this->skelAnime);
        }
    } else if (Cutscene_CheckActorAction(play, 473)) {
        if (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 473)]->action == 3) {
            SkelAnime_Update(&this->skelAnime);
        }
    } else if (Cutscene_CheckActorAction(play, 518)) {
        if (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 518)]->action == 2) {
            SkelAnime_Update(&this->skelAnime);
        }
    } else if (Cutscene_CheckActorAction(play, 559)) {
        if ((play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 559)]->action == 2) ||
            (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 559)]->action == 3)) {
            SkelAnime_Update(&this->skelAnime);
        }

        if (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 559)]->action == 4) {
            this->actor.world.rot.y += 0x258;
            this->actor.shape.rot.y += 0x258;
        }
    }
    this->actionFunc(this, play);
    func_80AAD4A8(this, play);
}

s32 DmChar05_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    return false;
}

void DmChar05_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
}

void func_80AAD998(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;
    s32 pad[2];

    if (this->unk_18E == 0) {
        if (Cutscene_CheckActorAction(play, 109) &&
            (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 109)]->action != 1)) {
            OPEN_DISPS(play->state.gfxCtx);

            func_8012C28C(play->state.gfxCtx);
            POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, this->unk_19C, this->unk_1A0, this->unk_1A4, this->unk_1A8,
                                       this->unk_1AC, this->unk_1B0);
            SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, DmChar05_OverrideLimbDraw,
                              DmChar05_PostLimbDraw, &this->actor);

            CLOSE_DISPS(play->state.gfxCtx);
        }
    } else if (this->unk_18E == 1) {
        func_80AADD9C(play, this);
    }
}

void func_80AADA90(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    if (this->unk_18E == 0) {
        if (Cutscene_CheckActorAction(play, 473) &&
            (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 473)]->action != 1)) {
            func_8012C28C(play->state.gfxCtx);
            SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, DmChar05_OverrideLimbDraw,
                              DmChar05_PostLimbDraw, &this->actor);
        }
    } else if (this->unk_18E == 1) {
        func_80AADE78(play, this);
    }
}

void func_80AADB4C(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    if (this->unk_18E == 0) {
        if (Cutscene_CheckActorAction(play, 518) &&
            (play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, 518)]->action != 1)) {
            func_8012C28C(play->state.gfxCtx);
            SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                  this->skelAnime.dListCount, NULL, NULL, &this->actor);
        }
    } else if (this->unk_18E == 1) {
        func_80AADF54(play, this);
    }
}

void func_80AADC00(Actor* thisx, PlayState* play) {
    s32 pad;
    DmChar05* this = THIS;
    s32 actionIndex;

    if (Cutscene_CheckActorAction(play, 559)) {
        actionIndex = Cutscene_GetActorActionIndex(play, 559);

        if ((play->csCtx.actorActions[actionIndex]->action != 1) &&
            (play->csCtx.actorActions[actionIndex]->action != 4)) {
            func_8012C28C(play->state.gfxCtx);
            SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, DmChar05_OverrideLimbDraw,
                              DmChar05_PostLimbDraw, &this->actor);
        }

        if (play->csCtx.actorActions[actionIndex]->action == 4) {
            Matrix_Translate(-600.0f, 0.0f, 0.0f, MTXMODE_APPLY);
            Gfx_DrawDListOpa(play, object_dmask_DL_001E70);
        }
    }
}

void DmChar05_Draw(Actor* thisx, PlayState* play) {
    DmChar05* this = THIS;

    switch (DMCHAR05_GET(&this->actor)) {
        case DMCHAR05_0:
            func_80AAD998(thisx, play);
            break;

        case DMCHAR05_1:
            func_80AADA90(thisx, play);
            break;

        case DMCHAR05_2:
            func_80AADB4C(thisx, play);
            break;

        case DMCHAR05_3:
            func_80AADC00(thisx, play);
            break;

        case DMCHAR05_4:
            func_80AAE030(play, this);
            break;

        case DMCHAR05_5:
        case DMCHAR05_6:
        case DMCHAR05_7:
        case DMCHAR05_8:
        case DMCHAR05_9:
        case DMCHAR05_10:
        case DMCHAR05_11:
        case DMCHAR05_12:
            func_80AAE114(play, this);
            break;
    }
}

void func_80AADD9C(PlayState* play, DmChar05* this) {
    if (this->actor.objBankIndex == this->unk_18F) {
        Matrix_Translate(this->unk_190.x, this->unk_190.y, this->unk_190.z, MTXMODE_NEW);
        Matrix_RotateZYX(0, play->gameplayFrames * 1000, 0, MTXMODE_APPLY);
        Matrix_Scale(0.2f, 0.2f, 0.2f, MTXMODE_APPLY);
        GetItem_Draw(play, GID_MASK_GORON);
    }

    if (Object_IsLoaded(&play->objectCtx, this->unk_18F)) {
        this->actor.objBankIndex = this->unk_18F;
    }
}

void func_80AADE78(PlayState* play, DmChar05* this) {
    if (this->actor.objBankIndex == this->unk_18F) {
        Matrix_Translate(this->unk_190.x, this->unk_190.y, this->unk_190.z, MTXMODE_NEW);
        Matrix_RotateZYX(0, play->gameplayFrames * 1000, 0, MTXMODE_APPLY);
        Matrix_Scale(0.2f, 0.2f, 0.2f, MTXMODE_APPLY);
        GetItem_Draw(play, GID_MASK_ZORA);
    }

    if (Object_IsLoaded(&play->objectCtx, this->unk_18F)) {
        this->actor.objBankIndex = this->unk_18F;
    }
}

void func_80AADF54(PlayState* play, DmChar05* this) {
    if (this->actor.objBankIndex == this->unk_18F) {
        Matrix_Translate(this->unk_190.x, this->unk_190.y, this->unk_190.z, MTXMODE_NEW);
        Matrix_RotateZYX(0, play->gameplayFrames * 1000, 0, MTXMODE_APPLY);
        Matrix_Scale(0.2f, 0.2f, 0.2f, MTXMODE_APPLY);
        GetItem_Draw(play, GID_MASK_GIBDO);
    }

    if (Object_IsLoaded(&play->objectCtx, this->unk_18F)) {
        this->actor.objBankIndex = this->unk_18F;
    }
}

void func_80AAE030(PlayState* play, DmChar05* this) {
    if (this->unk_18E != 0) {
        if (this->actor.objBankIndex == this->unk_18F) {
            Matrix_Translate(this->unk_190.x, this->unk_190.y, this->unk_190.z, MTXMODE_NEW);
            Matrix_RotateZYX(0, play->gameplayFrames * 1000, 0, MTXMODE_APPLY);
            Matrix_Scale(0.2f, 0.2f, 0.2f, MTXMODE_APPLY);
            GetItem_Draw(play, GID_MASK_COUPLE);
        }

        if (Object_IsLoaded(&play->objectCtx, this->unk_18F)) {
            this->actor.objBankIndex = this->unk_18F;
        }
    }
}

void func_80AAE114(PlayState* play, DmChar05* this) {
    s32 getItemDrawIndex;
    s16 getItemDrawIds[] = {
        GID_MASK_KAMARO,        GID_MASK_GREAT_FAIRY, GID_MASK_ROMANI, GID_MASK_BLAST,
        GID_MASK_CIRCUS_LEADER, GID_MASK_BREMEN,      GID_MASK_BUNNY,  GID_MASK_POSTMAN,
    };

    getItemDrawIndex = DMCHAR05_GET(&this->actor) - DMCHAR05_5;
    if (this->actor.objBankIndex == this->unk_18F) {
        Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
        Matrix_RotateZYX(0, play->gameplayFrames * 1000, 0, MTXMODE_APPLY);
        Matrix_Scale(0.2f, 0.2f, 0.2f, MTXMODE_APPLY);
        GetItem_Draw(play, getItemDrawIds[getItemDrawIndex]);
    }

    if (Object_IsLoaded(&play->objectCtx, this->unk_18F)) {
        this->actor.objBankIndex = this->unk_18F;
    }
}
