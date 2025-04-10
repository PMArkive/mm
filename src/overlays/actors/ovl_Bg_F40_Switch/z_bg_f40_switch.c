/*
 * File: z_bg_f40_switch.c
 * Overlay: ovl_Bg_F40_Switch
 * Description: Stone Tower Switch
 */

#include "z_bg_f40_switch.h"
#include "z64rumble.h"
#include "objects/object_f40_switch/object_f40_switch.h"

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((BgF40Switch*)thisx)

void BgF40Switch_Init(Actor* thisx, PlayState* play);
void BgF40Switch_Destroy(Actor* thisx, PlayState* play);
void BgF40Switch_Update(Actor* thisx, PlayState* play);
void BgF40Switch_Draw(Actor* thisx, PlayState* play);

void BgF40Switch_CheckAll(BgF40Switch* this, PlayState* play);
void BgF40Switch_Unpress(BgF40Switch* this, PlayState* play);
void BgF40Switch_IdlePressed(BgF40Switch* this, PlayState* play);
void BgF40Switch_Press(BgF40Switch* this, PlayState* play);
void BgF40Switch_WaitToPress(BgF40Switch* this, PlayState* play);
void BgF40Switch_IdleUnpressed(BgF40Switch* this, PlayState* play);

ActorInit Bg_F40_Switch_InitVars = {
    ACTOR_BG_F40_SWITCH,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_F40_SWITCH,
    sizeof(BgF40Switch),
    (ActorFunc)BgF40Switch_Init,
    (ActorFunc)BgF40Switch_Destroy,
    (ActorFunc)BgF40Switch_Update,
    (ActorFunc)BgF40Switch_Draw,
};

s32 sBgF40SwitchGlobalsInitialized = false;
u32 sBgF40SwitchLastUpdateFrame;

/*
 * Updates all instances of this actor in the current room, unless it's already been called this frame.
 */
void BgF40Switch_CheckAll(BgF40Switch* this, PlayState* play) {
    if (play->gameplayFrames != sBgF40SwitchLastUpdateFrame) {
        u32 pressedSwitchFlags[4] = { 0 };
        u32 pad;
        s32 switchFlag;
        s32 isPressed;
        Actor* actor;
        BgF40Switch* actorAsSwitch;
        u32 inCsMode = Player_InCsMode(play);

        for (actor = play->actorCtx.actorLists[ACTORCAT_SWITCH].first; actor != NULL; actor = actor->next) {
            if (actor->id == ACTOR_BG_F40_SWITCH && actor->room == this->dyna.actor.room && actor->update != NULL) {
                actorAsSwitch = (BgF40Switch*)actor;
                actorAsSwitch->wasPressed = actorAsSwitch->isPressed;
                isPressed = DynaPolyActor_IsInSwitchPressedState(&actorAsSwitch->dyna);
                if (actorAsSwitch->isPressed && actorAsSwitch->actionFunc == BgF40Switch_IdlePressed) {
                    // Switch is fully pressed - if there's nothing keeping it pressed, wait a short time before
                    // reverting to unpressed state.
                    if (isPressed || inCsMode) {
                        actorAsSwitch->switchReleaseDelay = 6;
                    } else {
                        if (actorAsSwitch->switchReleaseDelay > 0) {
                            actorAsSwitch->switchReleaseDelay--;
                        } else {
                            actorAsSwitch->isPressed = isPressed;
                        }
                    }
                } else {
                    // No delay when pressing switch, or releasing from a not-fully-pressed state.
                    actorAsSwitch->isPressed = isPressed;
                }
                if (actorAsSwitch->isPressed) {
                    switchFlag = BGF40SWITCH_GET_SWITCHFLAG(&actorAsSwitch->dyna.actor);
                    if (switchFlag >= 0 && switchFlag < 0x80) {
                        pressedSwitchFlags[(switchFlag & ~0x1F) >> 5] |= 1 << (switchFlag & 0x1F);
                        if (!actorAsSwitch->wasPressed && actorAsSwitch->actionFunc == BgF40Switch_IdleUnpressed &&
                            !Flags_GetSwitch(play, switchFlag)) {
                            actorAsSwitch->isInitiator = true;
                        }
                    }
                }
            }
        }
        for (actor = play->actorCtx.actorLists[ACTORCAT_SWITCH].first; actor != NULL; actor = actor->next) {
            if (actor->id == ACTOR_BG_F40_SWITCH && actor->room == this->dyna.actor.room && actor->update != 0) {
                switchFlag = BGF40SWITCH_GET_SWITCHFLAG(actor);
                if (switchFlag >= 0 && switchFlag < 0x80 && Flags_GetSwitch(play, switchFlag) &&
                    !(pressedSwitchFlags[(switchFlag & ~0x1F) >> 5] & (1 << (switchFlag & 0x1F)))) {
                    Flags_UnsetSwitch(play, switchFlag);
                }
            }
        }
        sBgF40SwitchLastUpdateFrame = play->gameplayFrames;
    }
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 200, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 123, ICHAIN_STOP),
};

void BgF40Switch_Init(Actor* thisx, PlayState* play) {
    BgF40Switch* this = THIS;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    this->dyna.actor.scale.y = 0.165f;
    this->actionFunc = BgF40Switch_IdleUnpressed;
    this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y + 1.0f;
    DynaPolyActor_Init(&this->dyna, 1);
    DynaPolyActor_LoadMesh(play, &this->dyna, &object_f40_switch_Colheader_000118);
    if (!sBgF40SwitchGlobalsInitialized) {
        sBgF40SwitchLastUpdateFrame = play->gameplayFrames;
        sBgF40SwitchGlobalsInitialized = true;
    }
}

void BgF40Switch_Destroy(Actor* thisx, PlayState* play) {
    BgF40Switch* this = THIS;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void BgF40Switch_Unpress(BgF40Switch* this, PlayState* play) {
    this->dyna.actor.scale.y += 0.0495f;
    if (this->dyna.actor.scale.y >= 0.165f) {
        Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_IKANA_BLOCK_SWITCH);
        this->actionFunc = BgF40Switch_IdleUnpressed;
        this->dyna.actor.scale.y = 0.165f;
    }
}

void BgF40Switch_IdlePressed(BgF40Switch* this, PlayState* play) {
    if (!this->isPressed) {
        this->actionFunc = BgF40Switch_Unpress;
    }
}

void BgF40Switch_Press(BgF40Switch* this, PlayState* play) {
    this->dyna.actor.scale.y -= 0.0495f;
    if (this->dyna.actor.scale.y <= 0.0165f) {
        Actor_PlaySfxAtPos(&this->dyna.actor, NA_SE_EV_IKANA_BLOCK_SWITCH);
        Rumble_Request(this->dyna.actor.xyzDistToPlayerSq, 120, 20, 10);
        if (this->isInitiator) {
            ActorCutscene_Stop(this->dyna.actor.cutscene);
            this->isInitiator = false;
        }
        this->actionFunc = BgF40Switch_IdlePressed;
        this->dyna.actor.scale.y = 0.0165f;
        this->switchReleaseDelay = 6;
    }
}

void BgF40Switch_WaitToPress(BgF40Switch* this, PlayState* play) {
    if (!this->isInitiator || this->dyna.actor.cutscene == -1) {
        this->actionFunc = BgF40Switch_Press;
        if (this->isInitiator) {
            Flags_SetSwitch(play, BGF40SWITCH_GET_SWITCHFLAG(&this->dyna.actor));
        }
    } else if (ActorCutscene_GetCanPlayNext(this->dyna.actor.cutscene)) {
        ActorCutscene_StartAndSetUnkLinkFields(this->dyna.actor.cutscene, &this->dyna.actor);
        this->actionFunc = BgF40Switch_Press;
        if (this->isInitiator) {
            Flags_SetSwitch(play, BGF40SWITCH_GET_SWITCHFLAG(&this->dyna.actor));
        }
    } else {
        ActorCutscene_SetIntentToPlay(this->dyna.actor.cutscene);
    }
}

void BgF40Switch_IdleUnpressed(BgF40Switch* this, PlayState* play) {
    if (this->isPressed) {
        this->actionFunc = BgF40Switch_WaitToPress;
    }
}

void BgF40Switch_Update(Actor* thisx, PlayState* play) {
    BgF40Switch* this = THIS;

    BgF40Switch_CheckAll(this, play);
    this->actionFunc(this, play);
}

void BgF40Switch_Draw(Actor* thisx, PlayState* play) {
    BgF40Switch* this = THIS;

    Gfx_DrawDListOpa(play, object_f40_switch_DL_000438);
    Gfx_DrawDListOpa(play, object_f40_switch_DL_000390);
}
