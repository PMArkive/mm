/*
 * File: z_eff_lastday.c
 * Overlay: ovl_Eff_Lastday
 * Description: Moon Crash Cutscene Fire Wall
 */

#include "z_eff_lastday.h"
#include "objects/object_lastday/object_lastday.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((EffLastday*)thisx)

void EffLastday_Init(Actor* thisx, PlayState* play2);
void EffLastday_Destroy(Actor* thisx, PlayState* play);
void EffLastday_Update(Actor* thisx, PlayState* play);
void EffLastday_Draw(Actor* thisx, PlayState* play);

void func_80BEBD0C(EffLastday* this, PlayState* play);
void func_80BEBDF8(EffLastday* this, PlayState* play);
void func_80BEBEB8(EffLastday* this, PlayState* play);
void func_80BEBF78(EffLastday* this, PlayState* play);

typedef enum EffLastDayAction {
    /* 0x0 */ EFFLASTDAY_ACTION_NONE,
    /* 0x1 */ EFFLASTDAY_ACTION_1,
    /* 0x2 */ EFFLASTDAY_ACTION_2,
    /* 0x3 */ EFFLASTDAY_ACTION_3
} EffLastDayAction;

ActorInit Eff_Lastday_InitVars = {
    ACTOR_EFF_LASTDAY,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_LASTDAY,
    sizeof(EffLastday),
    (ActorFunc)EffLastday_Init,
    (ActorFunc)EffLastday_Destroy,
    (ActorFunc)EffLastday_Update,
    (ActorFunc)EffLastday_Draw,
};

void EffLastday_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EffLastday* this = THIS;

    Actor_SetScale(&this->actor, 0.1f);
    switch (EFFLASTDAY_GET_F(&this->actor)) {
        case EFFLASTDAY_PARAMS_1:
            this->dList = object_lastday_DL_000510;
            this->matAnim = Lib_SegmentedToVirtual(object_lastday_Matanimheader_000608);
            this->csActionCmd = 0x1FC;
            this->actionFunc = func_80BEBDF8;
            Actor_SetScale(&this->actor, 1.0f);
            break;

        case EFFLASTDAY_PARAMS_2:
            this->dList = object_lastday_DL_000210;
            this->matAnim = Lib_SegmentedToVirtual(object_lastday_Matanimheader_000308);
            this->csActionCmd = 0x1FD;
            this->actionFunc = func_80BEBEB8;
            Actor_SetScale(&this->actor, 0.5f);
            break;

        case EFFLASTDAY_PARAMS_3:
            this->dList = object_lastday_DL_000060;
            this->matAnim = Lib_SegmentedToVirtual(object_lastday_Matanimheader_000148);
            this->csActionCmd = 0x1FE;
            this->actionFunc = func_80BEBF78;
            Actor_SetScale(&this->actor, 0.2f);
            this->actor.home.rot.z = 0;
            break;

        default:
            this->dList = object_lastday_DL_000370;
            this->matAnim = Lib_SegmentedToVirtual(object_lastday_Matanimheader_000448);
            this->csActionCmd = 0x1FB;
            this->actionFunc = func_80BEBD0C;
            Actor_SetScale(&this->actor, 1.0f);
            Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EFF_LASTDAY, this->actor.world.pos.x,
                               this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.world.rot.y, 0,
                               EFFLASTDAY_PARAMS_1);
            Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EFF_LASTDAY, this->actor.world.pos.x,
                               this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.world.rot.y, 0,
                               EFFLASTDAY_PARAMS_2);
            Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EFF_LASTDAY, this->actor.world.pos.x,
                               this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.world.rot.y, 0,
                               EFFLASTDAY_PARAMS_3);
            break;
    }
}

void EffLastday_Destroy(Actor* thisx, PlayState* play) {
}

void func_80BEBD0C(EffLastday* this, PlayState* play) {
    u16 csAction;

    if ((Cutscene_CheckActorAction(play, this->csActionCmd))) {
        Cutscene_ActorTranslateAndYaw(&this->actor, play, Cutscene_GetActorActionIndex(play, this->csActionCmd));
        csAction = play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, this->csActionCmd)]->action;
        switch (csAction) {
            default:
                this->actor.draw = NULL;
                this->alpha = 0;
                this->step = 0;
                break;

            case EFFLASTDAY_ACTION_2:
                this->actor.draw = NULL;
                this->actor.draw = EffLastday_Draw;
                if (this->alpha < 255) {
                    this->alpha += 10;
                }
                if (this->alpha > 255) {
                    this->alpha = 255;
                }
                this->step++;
                break;

            case EFFLASTDAY_ACTION_3:
                this->actor.draw = EffLastday_Draw;
                break;
        }
    } else {
        this->actor.draw = NULL;
    }
}

void func_80BEBDF8(EffLastday* this, PlayState* play) {
    u16 csAction;

    if (Cutscene_CheckActorAction(play, this->csActionCmd)) {
        Cutscene_ActorTranslateAndYaw(&this->actor, play, Cutscene_GetActorActionIndex(play, this->csActionCmd));
        csAction = play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, this->csActionCmd)]->action;
        switch (csAction) {
            default:
                this->actor.draw = NULL;
                this->step = 0;
                break;

            case EFFLASTDAY_ACTION_2:
                this->actor.draw = EffLastday_Draw;
                this->step++;
                break;

            case EFFLASTDAY_ACTION_3:
                this->actor.draw = EffLastday_Draw;
                break;
        }
    } else {
        this->actor.draw = NULL;
    }
}

void func_80BEBEB8(EffLastday* this, PlayState* play) {
    u16 csAction;

    if (Cutscene_CheckActorAction(play, this->csActionCmd)) {
        Cutscene_ActorTranslateAndYaw(&this->actor, play, Cutscene_GetActorActionIndex(play, this->csActionCmd));
        csAction = play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, this->csActionCmd)]->action;
        switch (csAction) {
            default:
                this->actor.draw = NULL;
                this->step = 0;
                break;

            case EFFLASTDAY_ACTION_2:
                this->actor.draw = EffLastday_Draw;
                this->step++;
                break;

            case EFFLASTDAY_ACTION_3:
                this->actor.draw = EffLastday_Draw;
                break;
        }
    } else {
        this->actor.draw = NULL;
    }
}

void func_80BEBF78(EffLastday* this, PlayState* play) {
    u16 csAction;

    if (Cutscene_CheckActorAction(play, this->csActionCmd)) {
        Cutscene_ActorTranslateAndYaw(&this->actor, play, Cutscene_GetActorActionIndex(play, this->csActionCmd));
        csAction = play->csCtx.actorActions[Cutscene_GetActorActionIndex(play, this->csActionCmd)]->action;
        switch (csAction) {
            default:
                this->actor.draw = NULL;
                this->alpha = 0;
                this->step = 0;
                break;

            case EFFLASTDAY_ACTION_2:
                if (!this->actor.home.rot.z) {
                    this->actor.home.rot.z = true;
                    Actor_PlaySfxAtPos(&this->actor, NA_SE_EV_COMING_FIRE);
                }
                this->actor.draw = EffLastday_Draw;
                if (this->alpha < 255) {
                    this->alpha += 5;
                }
                if (this->alpha > 255) {
                    this->alpha = 255;
                }
                this->step++;
                break;

            case EFFLASTDAY_ACTION_3:

                this->actor.draw = EffLastday_Draw;
                break;
        }
    } else {
        this->actor.draw = NULL;
    }
}

void EffLastday_Update(Actor* thisx, PlayState* play) {
    EffLastday* this = THIS;

    this->actionFunc(this, play);
}

void EffLastday_SetVtxAlpha(s16 alpha) {
    Vtx* vtx = Lib_SegmentedToVirtual(object_lastday_Vtx_000000);

    vtx[0].v.cn[3] = alpha;
    vtx[3].v.cn[3] = alpha;
    vtx[4].v.cn[3] = alpha;
    vtx[5].v.cn[3] = alpha;
}

void EffLastday_Draw(Actor* thisx, PlayState* play) {
    EffLastday* this = THIS;

    OPEN_DISPS(play->state.gfxCtx);

    switch (EFFLASTDAY_GET_F(thisx)) {
        case EFFLASTDAY_PARAMS_1:
        case EFFLASTDAY_PARAMS_2:
            break;
        case EFFLASTDAY_PARAMS_3:
            EffLastday_SetVtxAlpha(this->alpha);
            break;
        default:
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 255, 255, 170, this->alpha);
            break;
    }
    AnimatedMat_DrawAlphaStep(play, this->matAnim, 1.0f, this->step);
    Gfx_DrawDListXlu(play, this->dList);

    CLOSE_DISPS(play->state.gfxCtx);
}
