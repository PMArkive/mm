#ifndef Z64PLAYER_H
#define Z64PLAYER_H

#include "alignment.h"
#include "os.h"
#include "z64actor.h"

struct Player;

typedef enum PlayerShield {
    /* 0 */ PLAYER_SHIELD_NONE,
    /* 1 */ PLAYER_SHIELD_HEROS_SHIELD,
    /* 2 */ PLAYER_SHIELD_MIRROR_SHIELD,
    /* 3 */ PLAYER_SHIELD_MAX
} PlayerShield;

typedef enum PlayerBoots {
    /* 0 */ PLAYER_BOOTS_FIERCE_DEITY,
    /* 1 */ PLAYER_BOOTS_HYLIAN,
    /* 2 */ PLAYER_BOOTS_GIANT,
    /* 3 */ PLAYER_BOOTS_DEKU,
    /* 4 */ PLAYER_BOOTS_ZORA_LAND,
    /* 5 */ PLAYER_BOOTS_ZORA_UNDERWATER,
    /* 6 */ PLAYER_BOOTS_GORON,
    /* 7 */ PLAYER_BOOTS_MAX
} PlayerBoots;

typedef enum PlayerStrength {
    /* 0 */ PLAYER_STRENGTH_DEKU,
    /* 1 */ PLAYER_STRENGTH_HUMAN,
    /* 2 */ PLAYER_STRENGTH_ZORA,
    /* 3 */ PLAYER_STRENGTH_GORON,
    /* 4 */ PLAYER_STRENGTH_MAX
} PlayerStrength;

typedef enum PlayerTransformation {
    /* 0 */ PLAYER_FORM_FIERCE_DEITY,
    /* 1 */ PLAYER_FORM_GORON,
    /* 2 */ PLAYER_FORM_ZORA,
    /* 3 */ PLAYER_FORM_DEKU,
    /* 4 */ PLAYER_FORM_HUMAN,
    /* 5 */ PLAYER_FORM_MAX
} PlayerTransformation;

typedef enum PlayerMask {
    /* 0x00 */ PLAYER_MASK_NONE,
    /* 0x01 */ PLAYER_MASK_TRUTH,
    /* 0x02 */ PLAYER_MASK_KAFEIS_MASK,
    /* 0x03 */ PLAYER_MASK_ALL_NIGHT,
    /* 0x04 */ PLAYER_MASK_BUNNY,
    /* 0x05 */ PLAYER_MASK_KEATON,
    /* 0x06 */ PLAYER_MASK_GARO,
    /* 0x07 */ PLAYER_MASK_ROMANI,
    /* 0x08 */ PLAYER_MASK_CIRCUS_LEADER,
    /* 0x09 */ PLAYER_MASK_POSTMAN,
    /* 0x0A */ PLAYER_MASK_COUPLE,
    /* 0x0B */ PLAYER_MASK_GREAT_FAIRY,
    /* 0x0C */ PLAYER_MASK_GIBDO,
    /* 0x0D */ PLAYER_MASK_DON_GERO,
    /* 0x0E */ PLAYER_MASK_KAMARO,
    /* 0x0F */ PLAYER_MASK_CAPTAIN,
    /* 0x10 */ PLAYER_MASK_STONE,
    /* 0x11 */ PLAYER_MASK_BREMEN,
    /* 0x12 */ PLAYER_MASK_BLAST,
    /* 0x13 */ PLAYER_MASK_SCENTS,
    /* 0x14 */ PLAYER_MASK_GIANT,
    /* 0x15 */ PLAYER_MASK_FIERCE_DEITY,
    /* 0x16 */ PLAYER_MASK_GORON,
    /* 0x17 */ PLAYER_MASK_ZORA,
    /* 0x18 */ PLAYER_MASK_DEKU,
    /* 0x19 */ PLAYER_MASK_MAX
} PlayerMask;

typedef enum {
    /* 0 */ PLAYER_ENV_HAZARD_NONE,
    /* 1 */ PLAYER_ENV_HAZARD_HOTROOM,
    /* 2 */ PLAYER_ENV_HAZARD_UNDERWATER_FLOOR,
    /* 3 */ PLAYER_ENV_HAZARD_SWIMMING,
    /* 4 */ PLAYER_ENV_HAZARD_UNDERWATER_FREE
} PlayerEnvHazard;

typedef enum PlayerItemAction {
    /*   -1 */ PLAYER_IA_MINUS1 = -1,
    /* 0x00 */ PLAYER_IA_NONE,
    /* 0x01 */ PLAYER_IA_LAST_USED,
    /* 0x02 */ PLAYER_IA_FISHING_ROD,
    /* 0x03 */ PLAYER_IA_SWORD_KOKIRI,
    /* 0x04 */ PLAYER_IA_SWORD_RAZOR,
    /* 0x05 */ PLAYER_IA_SWORD_GILDED,
    /* 0x06 */ PLAYER_IA_SWORD_GREAT_FAIRY,
    /* 0x07 */ PLAYER_IA_STICK,
    /* 0x08 */ PLAYER_IA_ZORA_FINS,
    /* 0x09 */ PLAYER_IA_BOW,
    /* 0x0A */ PLAYER_IA_BOW_FIRE,
    /* 0x0B */ PLAYER_IA_BOW_ICE,
    /* 0x0C */ PLAYER_IA_BOW_LIGHT,
    /* 0x0D */ PLAYER_IA_HOOKSHOT,
    /* 0x0E */ PLAYER_IA_BOMB,
    /* 0x0F */ PLAYER_IA_POWDER_KEG,
    /* 0x10 */ PLAYER_IA_BOMBCHU,
    /* 0x11 */ PLAYER_IA_11,
    /* 0x12 */ PLAYER_IA_NUT,
    /* 0x13 */ PLAYER_IA_PICTO_BOX,
    /* 0x14 */ PLAYER_IA_OCARINA,
    /* 0x15 */ PLAYER_IA_BOTTLE,
    /* 0x16 */ PLAYER_IA_BOTTLE_FISH,
    /* 0x17 */ PLAYER_IA_BOTTLE_SPRING_WATER,
    /* 0x18 */ PLAYER_IA_BOTTLE_HOT_SPRING_WATER,
    /* 0x19 */ PLAYER_IA_BOTTLE_ZORA_EGG,
    /* 0x1A */ PLAYER_IA_BOTTLE_DEKU_PRINCESS,
    /* 0x1B */ PLAYER_IA_BOTTLE_GOLD_DUST,
    /* 0x1C */ PLAYER_IA_BOTTLE_1C,
    /* 0x1D */ PLAYER_IA_BOTTLE_SEAHORSE,
    /* 0x1E */ PLAYER_IA_BOTTLE_MUSHROOM,
    /* 0x1F */ PLAYER_IA_BOTTLE_HYLIAN_LOACH,
    /* 0x20 */ PLAYER_IA_BOTTLE_BUG,
    /* 0x21 */ PLAYER_IA_BOTTLE_POE,
    /* 0x22 */ PLAYER_IA_BOTTLE_BIG_POE,
    /* 0x23 */ PLAYER_IA_BOTTLE_POTION_RED,
    /* 0x24 */ PLAYER_IA_BOTTLE_POTION_BLUE,
    /* 0x25 */ PLAYER_IA_BOTTLE_POTION_GREEN,
    /* 0x26 */ PLAYER_IA_BOTTLE_MILK,
    /* 0x27 */ PLAYER_IA_BOTTLE_MILK_HALF,
    /* 0x28 */ PLAYER_IA_BOTTLE_CHATEAU,
    /* 0x29 */ PLAYER_IA_BOTTLE_FAIRY,
    /* 0x2A */ PLAYER_IA_MOON_TEAR,
    /* 0x2B */ PLAYER_IA_DEED_LAND,
    /* 0x2C */ PLAYER_IA_ROOM_KEY,
    /* 0x2D */ PLAYER_IA_LETTER_TO_KAFEI,
    /* 0x2E */ PLAYER_IA_MAGIC_BEANS,
    /* 0x2F */ PLAYER_IA_DEED_SWAMP,
    /* 0x30 */ PLAYER_IA_DEED_MOUNTAIN,
    /* 0x31 */ PLAYER_IA_DEED_OCEAN,
    /* 0x32 */ PLAYER_IA_32,
    /* 0x33 */ PLAYER_IA_LETTER_MAMA,
    /* 0x34 */ PLAYER_IA_34,
    /* 0x35 */ PLAYER_IA_35,
    /* 0x36 */ PLAYER_IA_PENDANT_OF_MEMORIES,
    /* 0x37 */ PLAYER_IA_37,
    /* 0x38 */ PLAYER_IA_38,
    /* 0x39 */ PLAYER_IA_39,
    /* 0x3A */ PLAYER_IA_MASK_TRUTH,
    /* 0x3B */ PLAYER_IA_MASK_KAFEIS_MASK,
    /* 0x3C */ PLAYER_IA_MASK_ALL_NIGHT,
    /* 0x3D */ PLAYER_IA_MASK_BUNNY,
    /* 0x3E */ PLAYER_IA_MASK_KEATON,
    /* 0x3F */ PLAYER_IA_MASK_GARO,
    /* 0x40 */ PLAYER_IA_MASK_ROMANI,
    /* 0x41 */ PLAYER_IA_MASK_CIRCUS_LEADER,
    /* 0x42 */ PLAYER_IA_MASK_POSTMAN,
    /* 0x43 */ PLAYER_IA_MASK_COUPLE,
    /* 0x44 */ PLAYER_IA_MASK_GREAT_FAIRY,
    /* 0x45 */ PLAYER_IA_MASK_GIBDO,
    /* 0x46 */ PLAYER_IA_MASK_DON_GERO,
    /* 0x47 */ PLAYER_IA_MASK_KAMARO,
    /* 0x48 */ PLAYER_IA_MASK_CAPTAIN,
    /* 0x49 */ PLAYER_IA_MASK_STONE,
    /* 0x4A */ PLAYER_IA_MASK_BREMEN,
    /* 0x4B */ PLAYER_IA_MASK_BLAST,
    /* 0x4C */ PLAYER_IA_MASK_SCENTS,
    /* 0x4D */ PLAYER_IA_MASK_GIANT,
    /* 0x4E */ PLAYER_IA_MASK_FIERCE_DEITY,
    /* 0x4F */ PLAYER_IA_MASK_GORON,
    /* 0x50 */ PLAYER_IA_MASK_ZORA,
    /* 0x51 */ PLAYER_IA_MASK_DEKU,
    /* 0x52 */ PLAYER_IA_LENS,
    /* 0x53 */ PLAYER_IA_MAX
} PlayerItemAction;

typedef enum PlayerMeleeWeaponAnimation {
    /*  0 */ PLAYER_MWA_FORWARD_SLASH_1H,      // Vertical one-handed slash
    /*  1 */ PLAYER_MWA_FORWARD_SLASH_2H,      // Vertical two-handed slash
    /*  2 */ PLAYER_MWA_FORWARD_COMBO_1H,      // Third vertical one-handed slash
    /*  3 */ PLAYER_MWA_FORWARD_COMBO_2H,      // Third vertical two-handed slash
    /*  4 */ PLAYER_MWA_RIGHT_SLASH_1H,        // Horizontal one-handed slash
    /*  5 */ PLAYER_MWA_RIGHT_SLASH_2H,        // Horizontal two-handed slash
    /*  6 */ PLAYER_MWA_RIGHT_COMBO_1H,        // Third horizontal one-handed slash
    /*  7 */ PLAYER_MWA_RIGHT_COMBO_2H,        // Third horizontal two-handed slash
    /*  8 */ PLAYER_MWA_LEFT_SLASH_1H,         // Targeted one-handed rightwalk slash
    /*  9 */ PLAYER_MWA_LEFT_SLASH_2H,         // Targeted two-handed rightwalk slash
    /* 10 */ PLAYER_MWA_LEFT_COMBO_1H,         // Third targeted one-handed rightwalk slash
    /* 11 */ PLAYER_MWA_LEFT_COMBO_2H,         // Third targeted two-handed rightwalk slash
    /* 12 */ PLAYER_MWA_STAB_1H,               // Crouch stab/targeted one-handed stab
    /* 13 */ PLAYER_MWA_STAB_2H,               // Targeted two-handed stab
    /* 14 */ PLAYER_MWA_STAB_COMBO_1H,         // Third targeted one-handed stab
    /* 15 */ PLAYER_MWA_STAB_COMBO_2H,         // Third targeted two-handed stab
    // These animations result in double damage
    /* 16 */ PLAYER_MWA_FLIPSLASH_START,       // unused
    /* 17 */ PLAYER_MWA_JUMPSLASH_START,       // Start of jumpslash
    /* 18 */ PLAYER_MWA_ZORA_JUMPKICK_START,   // Start of Zora jump attack
    /* 19 */ PLAYER_MWA_FLIPSLASH_FINISH,      // unused
    /* 20 */ PLAYER_MWA_JUMPSLASH_FINISH,      // End of jumpslash
    /* 21 */ PLAYER_MWA_ZORA_JUMPKICK_FINISH,  // End of Zora jump attack
    /* 22 */ PLAYER_MWA_BACKSLASH_RIGHT,       // unused
    /* 23 */ PLAYER_MWA_BACKSLASH_LEFT,        // unused
    /* 24 */ PLAYER_MWA_GORON_PUNCH_LEFT,      // Goron punch
    /* 25 */ PLAYER_MWA_GORON_PUNCH_RIGHT,     // Second Goron punch
    /* 26 */ PLAYER_MWA_GORON_PUNCH_BUTT,      // Goron butt punch
    /* 27 */ PLAYER_MWA_ZORA_PUNCH_LEFT,       // Zora punch
    /* 28 */ PLAYER_MWA_ZORA_PUNCH_COMBO,      // Second Zora punch
    /* 29 */ PLAYER_MWA_ZORA_PUNCH_KICK,       // Zora kick
    /* 30 */ PLAYER_MWA_SPIN_ATTACK_1H,        // Half-charged one-handed spin
    /* 31 */ PLAYER_MWA_SPIN_ATTACK_2H,        // Half-charged two-handed spin
    /* 32 */ PLAYER_MWA_BIG_SPIN_1H,           // Fully-charged one-handed spin
    /* 33 */ PLAYER_MWA_BIG_SPIN_2H,           // Fully-charged two-handed spin
    /* 34 */ PLAYER_MWA_MAX
} PlayerMeleeWeaponAnimation;

typedef enum PlayerDoorType {
    /* -1 */ PLAYER_DOORTYPE_TALKING = -1, // Displays a message instead of opening
    /*  0 */ PLAYER_DOORTYPE_NONE,
    /*  1 */ PLAYER_DOORTYPE_HANDLE,
    /*  2 */ PLAYER_DOORTYPE_SLIDING,
    /*  3 */ PLAYER_DOORTYPE_FAKE, // Unused? OoT leftover (?)
    /*  4 */ PLAYER_DOORTYPE_STAIRCASE,
    /*  5 */ PLAYER_DOORTYPE_PROXIMITY
} PlayerDoorType;

typedef enum PlayerAnimType {
    /* 0 */ PLAYER_ANIMTYPE_0,
    /* 1 */ PLAYER_ANIMTYPE_1,
    /* 2 */ PLAYER_ANIMTYPE_2,
    /* 3 */ PLAYER_ANIMTYPE_3,
    /* 4 */ PLAYER_ANIMTYPE_4,
    /* 5 */ PLAYER_ANIMTYPE_5,
    /* 6 */ PLAYER_ANIMTYPE_MAX
} PlayerAnimType;

typedef enum PlayerModelType {
    // left hand
    /*  0 */ PLAYER_MODELTYPE_LH_OPEN,
    /*  1 */ PLAYER_MODELTYPE_LH_CLOSED,
    /*  2 */ PLAYER_MODELTYPE_LH_ONE_HAND_SWORD,
    /*  3 */ PLAYER_MODELTYPE_LH_TWO_HAND_SWORD,
    /*  4 */ PLAYER_MODELTYPE_LH_4,
    /*  5 */ PLAYER_MODELTYPE_LH_BOTTLE,
    // right hand
    /*  6 */ PLAYER_MODELTYPE_RH_OPEN,
    /*  7 */ PLAYER_MODELTYPE_RH_CLOSED,
    /*  8 */ PLAYER_MODELTYPE_RH_SHIELD,
    /*  9 */ PLAYER_MODELTYPE_RH_BOW,
    /* 10 */ PLAYER_MODELTYPE_RH_INSTRUMENT,
    /* 11 */ PLAYER_MODELTYPE_RH_HOOKSHOT,
    // sheath
    /* 12 */ PLAYER_MODELTYPE_SHEATH_12,
    /* 13 */ PLAYER_MODELTYPE_SHEATH_13,
    /* 14 */ PLAYER_MODELTYPE_SHEATH_14,
    /* 15 */ PLAYER_MODELTYPE_SHEATH_15,
    // waist
    /* 16 */ PLAYER_MODELTYPE_WAIST,
    /* 17 */ PLAYER_MODELTYPE_17, // NULL?
    /* 18 */ PLAYER_MODELTYPE_MAX
} PlayerModelType;

typedef struct PlayerModelIndices {
    /* 0x0 */ u8 modelAnimType; // PlayerAnimType enum
    /* 0x1 */ u8 leftHandType; // PlayerModelType enum
    /* 0x2 */ u8 rightHandType; // PlayerModelType enum
    /* 0x3 */ u8 sheathType; // PlayerModelType enum
    /* 0x4 */ u8 waistType; // PlayerModelType enum
} PlayerModelIndices; // size = 0x5

typedef enum PlayerModelGroup {
    /*  0 */ PLAYER_MODELGROUP_0,
    /*  1 */ PLAYER_MODELGROUP_1,
    /*  2 */ PLAYER_MODELGROUP_ONE_HAND_SWORD,
    /*  3 */ PLAYER_MODELGROUP_DEFAULT, // non-specific models, for items that don't have particular link models
    /*  4 */ PLAYER_MODELGROUP_4,
    /*  5 */ PLAYER_MODELGROUP_TWO_HAND_SWORD,
    /*  6 */ PLAYER_MODELGROUP_BOW,
    /*  7 */ PLAYER_MODELGROUP_EXPLOSIVES,
    /*  8 */ PLAYER_MODELGROUP_8,
    /*  9 */ PLAYER_MODELGROUP_HOOKSHOT,
    /* 10 */ PLAYER_MODELGROUP_STICK,
    /* 11 */ PLAYER_MODELGROUP_INSTRUMENT,
    /* 12 */ PLAYER_MODELGROUP_BOTTLE,
    /* 13 */ PLAYER_MODELGROUP_13,
    /* 14 */ PLAYER_MODELGROUP_ZORA_FINS,
    /* 15 */ PLAYER_MODELGROUP_MAX
} PlayerModelGroup;

typedef enum PlayerEyeIndex {
    /* 0 */ PLAYER_EYES_OPEN,
    /* 1 */ PLAYER_EYES_HALF,
    /* 2 */ PLAYER_EYES_CLOSED,
    /* 3 */ PLAYER_EYES_ROLL_RIGHT,
    /* 4 */ PLAYER_EYES_ROLL_LEFT,
    /* 5 */ PLAYER_EYES_ROLL_UP,
    /* 6 */ PLAYER_EYES_ROLL_DOWN,
    /* 7 */ PLAYER_EYES_7,
    /* 8 */ PLAYER_EYES_MAX
} PlayerEyeIndex;

typedef enum PlayerMouthIndex {
    /* 0 */ PLAYER_MOUTH_CLOSED,
    /* 1 */ PLAYER_MOUTH_TEETH,
    /* 2 */ PLAYER_MOUTH_ANGRY,
    /* 3 */ PLAYER_MOUTH_HAPPY,
    /* 4 */ PLAYER_MOUTH_MAX
} PlayerMouthIndex;

typedef enum PlayerFacialExpression {
    /*  0 */ PLAYER_FACE_0,
    /*  1 */ PLAYER_FACE_1,
    /*  2 */ PLAYER_FACE_2,
    /*  3 */ PLAYER_FACE_3,
    /*  4 */ PLAYER_FACE_4,
    /*  5 */ PLAYER_FACE_5,
    /*  6 */ PLAYER_FACE_6,
    /*  7 */ PLAYER_FACE_7,
    /*  8 */ PLAYER_FACE_8,
    /*  9 */ PLAYER_FACE_9,
    /* 10 */ PLAYER_FACE_10,
    /* 11 */ PLAYER_FACE_11,
    /* 12 */ PLAYER_FACE_12,
    /* 13 */ PLAYER_FACE_13,
    /* 14 */ PLAYER_FACE_14,
    /* 15 */ PLAYER_FACE_15
} PlayerFacialExpression;

typedef enum PlayerLimb {
    /* 0x00 */ PLAYER_LIMB_NONE,
    /* 0x01 */ PLAYER_LIMB_ROOT,
    /* 0x02 */ PLAYER_LIMB_WAIST,
    /* 0x03 */ PLAYER_LIMB_LOWER_ROOT,
    /* 0x04 */ PLAYER_LIMB_RIGHT_THIGH,
    /* 0x05 */ PLAYER_LIMB_RIGHT_SHIN,
    /* 0x06 */ PLAYER_LIMB_RIGHT_FOOT,
    /* 0x07 */ PLAYER_LIMB_LEFT_THIGH,
    /* 0x08 */ PLAYER_LIMB_LEFT_SHIN,
    /* 0x09 */ PLAYER_LIMB_LEFT_FOOT,
    /* 0x0A */ PLAYER_LIMB_UPPER_ROOT,
    /* 0x0B */ PLAYER_LIMB_HEAD,
    /* 0x0C */ PLAYER_LIMB_HAT,
    /* 0x0D */ PLAYER_LIMB_COLLAR,
    /* 0x0E */ PLAYER_LIMB_LEFT_SHOULDER,
    /* 0x0F */ PLAYER_LIMB_LEFT_FOREARM,
    /* 0x10 */ PLAYER_LIMB_LEFT_HAND,
    /* 0x11 */ PLAYER_LIMB_RIGHT_SHOULDER,
    /* 0x12 */ PLAYER_LIMB_RIGHT_FOREARM,
    /* 0x13 */ PLAYER_LIMB_RIGHT_HAND,
    /* 0x14 */ PLAYER_LIMB_SHEATH,
    /* 0x15 */ PLAYER_LIMB_TORSO,
    /* 0x16 */ PLAYER_LIMB_MAX
} PlayerLimb;

typedef enum PlayerBodyPart {
    /* 0x00 */ PLAYER_BODYPART_WAIST,          // PLAYER_LIMB_WAIST
    /* 0x01 */ PLAYER_BODYPART_RIGHT_THIGH,    // PLAYER_LIMB_RIGHT_THIGH
    /* 0x02 */ PLAYER_BODYPART_RIGHT_SHIN,     // PLAYER_LIMB_RIGHT_SHIN
    /* 0x03 */ PLAYER_BODYPART_RIGHT_FOOT,     // PLAYER_LIMB_RIGHT_FOOT
    /* 0x04 */ PLAYER_BODYPART_LEFT_THIGH,     // PLAYER_LIMB_LEFT_THIGH
    /* 0x05 */ PLAYER_BODYPART_LEFT_SHIN,      // PLAYER_LIMB_LEFT_SHIN
    /* 0x06 */ PLAYER_BODYPART_LEFT_FOOT,      // PLAYER_LIMB_LEFT_FOOT
    /* 0x07 */ PLAYER_BODYPART_HEAD,           // PLAYER_LIMB_HEAD
    /* 0x08 */ PLAYER_BODYPART_HAT,            // PLAYER_LIMB_HAT
    /* 0x09 */ PLAYER_BODYPART_COLLAR,         // PLAYER_LIMB_COLLAR
    /* 0x0A */ PLAYER_BODYPART_LEFT_SHOULDER,  // PLAYER_LIMB_LEFT_SHOULDER
    /* 0x0B */ PLAYER_BODYPART_LEFT_FOREARM,   // PLAYER_LIMB_LEFT_FOREARM
    /* 0x0C */ PLAYER_BODYPART_LEFT_HAND,      // PLAYER_LIMB_LEFT_HAND
    /* 0x0D */ PLAYER_BODYPART_RIGHT_SHOULDER, // PLAYER_LIMB_R_SHOULDER
    /* 0x0E */ PLAYER_BODYPART_RIGHT_FOREARM,  // PLAYER_LIMB_R_FOREARM
    /* 0x0F */ PLAYER_BODYPART_RIGHT_HAND,     // PLAYER_LIMB_R_HAND
    /* 0x10 */ PLAYER_BODYPART_SHEATH,         // PLAYER_LIMB_SHEATH
    /* 0x11 */ PLAYER_BODYPART_TORSO,          // PLAYER_LIMB_TORSO
    /* 0x12 */ PLAYER_BODYPART_MAX
} PlayerBodyPart;

typedef struct PlayerAnimationFrame {
    /* 0x000 */ Vec3s frameTable[PLAYER_LIMB_MAX];
    /* 0x108 */ s16 faceInfo;
} PlayerAnimationFrame; // size = 0x10A

#define PLAYER_LIMB_BUF_SIZE (ALIGN16(sizeof(PlayerAnimationFrame)) + 0xF)

typedef struct PlayerAgeProperties {
    /* 0x00 */ f32 unk_00; // ceilingCheckHeight?
    /* 0x04 */ f32 shadowScale;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ f32 unk_14;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ f32 unk_1C;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ f32 unk_24;
    /* 0x28 */ f32 unk_28;
    /* 0x2C */ f32 unk_2C;
    /* 0x30 */ f32 unk_30;
    /* 0x34 */ f32 unk_34;
    /* 0x38 */ f32 unk_38; // wallCheckHeight?
    /* 0x3C */ f32 unk_3C;
    /* 0x40 */ f32 unk_40;
    /* 0x44 */ Vec3s unk_44;
    /* 0x4A */ Vec3s unk_4A[4];
    /* 0x62 */ Vec3s unk_62[4];
    /* 0x7A */ Vec3s unk_7A[4];
    /* 0x92 */ u16 voiceSfxIdOffset;
    /* 0x94 */ u16 surfaceSfxIdOffset;
    /* 0x98 */ f32 unk_98;
    /* 0x9C */ f32 unk_9C;
    /* 0xA0 */ LinkAnimationHeader* unk_A0;
    /* 0xA4 */ LinkAnimationHeader* unk_A4;
    /* 0xA8 */ LinkAnimationHeader* unk_A8;
    /* 0xAC */ LinkAnimationHeader* unk_AC;
    /* 0xB0 */ LinkAnimationHeader* unk_B0;
    /* 0xB4 */ LinkAnimationHeader* unk_B4[4];
    /* 0xC4 */ LinkAnimationHeader* unk_C4[2];
    /* 0xCC */ LinkAnimationHeader* unk_CC[2];
    /* 0xD4 */ LinkAnimationHeader* unk_D4[2];
} PlayerAgeProperties; // size = 0xDC

typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ Vec3f tip;
    /* 0x10 */ Vec3f base;
} WeaponInfo; // size = 0x1C


typedef struct {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 alpha;
    /* 0x04 */ MtxF mf;
} struct_80122D44_arg1_unk_04; // size = 0x44

typedef struct {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ char unk_02[2]; // probably alignment padding
    /* 0x04 */ struct_80122D44_arg1_unk_04 unk_04[4];
} struct_80122D44_arg1; // size >= 0x114

typedef struct struct_80122744_arg1 {
    /* 0x0 */ s8 unk_00;
    /* 0x1 */ s8 unk_01;
    /* 0x2 */ s8 unk_02;
    /* 0x3 */ s8 unk_03;
    /* 0x4 */ Vec3s* unk_04;
} struct_80122744_arg1; // size = 0x8

typedef enum PlayerCsMode {
    /* 0x00 */ PLAYER_CSMODE_0,
    /* 0x01 */ PLAYER_CSMODE_1,
    /* 0x02 */ PLAYER_CSMODE_2,
    /* 0x03 */ PLAYER_CSMODE_3,
    /* 0x04 */ PLAYER_CSMODE_4,
    /* 0x05 */ PLAYER_CSMODE_5,
    /* 0x06 */ PLAYER_CSMODE_6,
    /* 0x07 */ PLAYER_CSMODE_7,
    /* 0x08 */ PLAYER_CSMODE_8,
    /* 0x09 */ PLAYER_CSMODE_9,
    /* 0x0A */ PLAYER_CSMODE_10,
    /* 0x0B */ PLAYER_CSMODE_11,
    /* 0x0C */ PLAYER_CSMODE_12,
    /* 0x0D */ PLAYER_CSMODE_13,
    /* 0x0E */ PLAYER_CSMODE_14,
    /* 0x0F */ PLAYER_CSMODE_15,
    /* 0x10 */ PLAYER_CSMODE_16,
    /* 0x11 */ PLAYER_CSMODE_17,
    /* 0x12 */ PLAYER_CSMODE_18,
    /* 0x13 */ PLAYER_CSMODE_19,
    /* 0x14 */ PLAYER_CSMODE_20,
    /* 0x15 */ PLAYER_CSMODE_21,
    /* 0x16 */ PLAYER_CSMODE_22,
    /* 0x17 */ PLAYER_CSMODE_23,
    /* 0x18 */ PLAYER_CSMODE_24,
    /* 0x19 */ PLAYER_CSMODE_25,
    /* 0x1A */ PLAYER_CSMODE_26,
    /* 0x1B */ PLAYER_CSMODE_27,
    /* 0x1C */ PLAYER_CSMODE_28,
    /* 0x1D */ PLAYER_CSMODE_29,
    /* 0x1E */ PLAYER_CSMODE_30,
    /* 0x1F */ PLAYER_CSMODE_31,
    /* 0x20 */ PLAYER_CSMODE_32,
    /* 0x21 */ PLAYER_CSMODE_33,
    /* 0x22 */ PLAYER_CSMODE_34,
    /* 0x23 */ PLAYER_CSMODE_35,
    /* 0x24 */ PLAYER_CSMODE_36,
    /* 0x25 */ PLAYER_CSMODE_37,
    /* 0x26 */ PLAYER_CSMODE_38,
    /* 0x27 */ PLAYER_CSMODE_39,
    /* 0x28 */ PLAYER_CSMODE_40,
    /* 0x29 */ PLAYER_CSMODE_41,
    /* 0x2A */ PLAYER_CSMODE_42,
    /* 0x2B */ PLAYER_CSMODE_43,
    /* 0x2C */ PLAYER_CSMODE_44,
    /* 0x2D */ PLAYER_CSMODE_45,
    /* 0x2E */ PLAYER_CSMODE_46,
    /* 0x2F */ PLAYER_CSMODE_47,
    /* 0x30 */ PLAYER_CSMODE_48,
    /* 0x31 */ PLAYER_CSMODE_49,
    /* 0x32 */ PLAYER_CSMODE_50,
    /* 0x33 */ PLAYER_CSMODE_51,
    /* 0x34 */ PLAYER_CSMODE_52,
    /* 0x35 */ PLAYER_CSMODE_53,
    /* 0x36 */ PLAYER_CSMODE_54,
    /* 0x37 */ PLAYER_CSMODE_55,
    /* 0x38 */ PLAYER_CSMODE_56,
    /* 0x39 */ PLAYER_CSMODE_57,
    /* 0x3A */ PLAYER_CSMODE_58,
    /* 0x3B */ PLAYER_CSMODE_59,
    /* 0x3C */ PLAYER_CSMODE_60,
    /* 0x3D */ PLAYER_CSMODE_61,
    /* 0x3E */ PLAYER_CSMODE_62,
    /* 0x3F */ PLAYER_CSMODE_63,
    /* 0x40 */ PLAYER_CSMODE_64,
    /* 0x41 */ PLAYER_CSMODE_65,
    /* 0x42 */ PLAYER_CSMODE_66,
    /* 0x43 */ PLAYER_CSMODE_67,
    /* 0x44 */ PLAYER_CSMODE_68,
    /* 0x45 */ PLAYER_CSMODE_69,
    /* 0x46 */ PLAYER_CSMODE_70,
    /* 0x47 */ PLAYER_CSMODE_71,
    /* 0x48 */ PLAYER_CSMODE_72,
    /* 0x49 */ PLAYER_CSMODE_73,
    /* 0x4A */ PLAYER_CSMODE_74,
    /* 0x4B */ PLAYER_CSMODE_75,
    /* 0x4C */ PLAYER_CSMODE_76,
    /* 0x4D */ PLAYER_CSMODE_77,
    /* 0x4E */ PLAYER_CSMODE_78,
    /* 0x4F */ PLAYER_CSMODE_79,
    /* 0x50 */ PLAYER_CSMODE_80,
    /* 0x51 */ PLAYER_CSMODE_81,
    /* 0x52 */ PLAYER_CSMODE_82,
    /* 0x53 */ PLAYER_CSMODE_83,
    /* 0x54 */ PLAYER_CSMODE_84,
    /* 0x55 */ PLAYER_CSMODE_85,
    /* 0x56 */ PLAYER_CSMODE_86,
    /* 0x57 */ PLAYER_CSMODE_87,
    /* 0x58 */ PLAYER_CSMODE_88,
    /* 0x59 */ PLAYER_CSMODE_89,
    /* 0x5A */ PLAYER_CSMODE_90,
    /* 0x5B */ PLAYER_CSMODE_91,
    /* 0x5C */ PLAYER_CSMODE_92,
    /* 0x5D */ PLAYER_CSMODE_93,
    /* 0x5E */ PLAYER_CSMODE_94,
    /* 0x5F */ PLAYER_CSMODE_95,
    /* 0x60 */ PLAYER_CSMODE_96,
    /* 0x61 */ PLAYER_CSMODE_97,
    /* 0x62 */ PLAYER_CSMODE_98,
    /* 0x63 */ PLAYER_CSMODE_99,
    /* 0x64 */ PLAYER_CSMODE_100,
    /* 0x65 */ PLAYER_CSMODE_101,
    /* 0x66 */ PLAYER_CSMODE_102,
    /* 0x67 */ PLAYER_CSMODE_103,
    /* 0x68 */ PLAYER_CSMODE_104,
    /* 0x69 */ PLAYER_CSMODE_105,
    /* 0x6A */ PLAYER_CSMODE_106,
    /* 0x6B */ PLAYER_CSMODE_107,
    /* 0x6C */ PLAYER_CSMODE_108,
    /* 0x6D */ PLAYER_CSMODE_109,
    /* 0x6E */ PLAYER_CSMODE_110,
    /* 0x6F */ PLAYER_CSMODE_111,
    /* 0x70 */ PLAYER_CSMODE_112,
    /* 0x71 */ PLAYER_CSMODE_113,
    /* 0x72 */ PLAYER_CSMODE_114,
    /* 0x73 */ PLAYER_CSMODE_115,
    /* 0x74 */ PLAYER_CSMODE_116,
    /* 0x75 */ PLAYER_CSMODE_117,
    /* 0x76 */ PLAYER_CSMODE_118,
    /* 0x77 */ PLAYER_CSMODE_119,
    /* 0x78 */ PLAYER_CSMODE_120,
    /* 0x79 */ PLAYER_CSMODE_121,
    /* 0x7A */ PLAYER_CSMODE_122,
    /* 0x7B */ PLAYER_CSMODE_123,
    /* 0x7C */ PLAYER_CSMODE_124,
    /* 0x7D */ PLAYER_CSMODE_125,
    /* 0x7E */ PLAYER_CSMODE_126,
    /* 0x7F */ PLAYER_CSMODE_127,
    /* 0x80 */ PLAYER_CSMODE_128,
    /* 0x81 */ PLAYER_CSMODE_129,
    /* 0x82 */ PLAYER_CSMODE_130,
    /* 0x83 */ PLAYER_CSMODE_131,
    /* 0x84 */ PLAYER_CSMODE_132,
    /* 0x85 */ PLAYER_CSMODE_133,
    /* 0x86 */ PLAYER_CSMODE_134,
    /* 0x87 */ PLAYER_CSMODE_135,
    /* 0x88 */ PLAYER_CSMODE_136,
    /* 0x89 */ PLAYER_CSMODE_137,
    /* 0x8A */ PLAYER_CSMODE_138,
    /* 0x8B */ PLAYER_CSMODE_139,
    /* 0x8C */ PLAYER_CSMODE_MAX
} PlayerCsMode;


// 
#define PLAYER_STATE1_1          (1 << 0)
// 
#define PLAYER_STATE1_2          (1 << 1)
// 
#define PLAYER_STATE1_4          (1 << 2)
// 
#define PLAYER_STATE1_8          (1 << 3)
// Zora electric shield
#define PLAYER_STATE1_10         (1 << 4)
// 
#define PLAYER_STATE1_20         (1 << 5)
// 
#define PLAYER_STATE1_40         (1 << 6)
// 
#define PLAYER_STATE1_80         (1 << 7)
// 
#define PLAYER_STATE1_100        (1 << 8)
// 
#define PLAYER_STATE1_200        (1 << 9)
// 
#define PLAYER_STATE1_400        (1 << 10)
// 
#define PLAYER_STATE1_800        (1 << 11)
// 
#define PLAYER_STATE1_1000       (1 << 12)
// 
#define PLAYER_STATE1_2000       (1 << 13)
// 
#define PLAYER_STATE1_4000       (1 << 14)
// 
#define PLAYER_STATE1_8000       (1 << 15)
// 
#define PLAYER_STATE1_10000      (1 << 16)
// 
#define PLAYER_STATE1_20000      (1 << 17)
// 
#define PLAYER_STATE1_40000      (1 << 18)
// 
#define PLAYER_STATE1_80000      (1 << 19)
// 
#define PLAYER_STATE1_100000     (1 << 20)
// 
#define PLAYER_STATE1_200000     (1 << 21)
// 
#define PLAYER_STATE1_400000     (1 << 22)
// 
#define PLAYER_STATE1_800000     (1 << 23)
// 
#define PLAYER_STATE1_1000000    (1 << 24)
// 
#define PLAYER_STATE1_2000000    (1 << 25)
// 
#define PLAYER_STATE1_4000000    (1 << 26)
// Swimming?
#define PLAYER_STATE1_8000000    (1 << 27)
// 
#define PLAYER_STATE1_10000000   (1 << 28)
// 
#define PLAYER_STATE1_20000000   (1 << 29)
// 
#define PLAYER_STATE1_40000000   (1 << 30)
// Related to exit a grotto
#define PLAYER_STATE1_80000000   (1 << 31)


// 
#define PLAYER_STATE2_1          (1 << 0)
// 
#define PLAYER_STATE2_2          (1 << 1)
// 
#define PLAYER_STATE2_4          (1 << 2)
// 
#define PLAYER_STATE2_8          (1 << 3)
// 
#define PLAYER_STATE2_10         (1 << 4)
// 
#define PLAYER_STATE2_20         (1 << 5)
// 
#define PLAYER_STATE2_40         (1 << 6)
// 
#define PLAYER_STATE2_80         (1 << 7)
// 
#define PLAYER_STATE2_100        (1 << 8)
// 
#define PLAYER_STATE2_200        (1 << 9)
// 
#define PLAYER_STATE2_400        (1 << 10)
// 
#define PLAYER_STATE2_800        (1 << 11)
// 
#define PLAYER_STATE2_1000       (1 << 12)
// 
#define PLAYER_STATE2_2000       (1 << 13)
// 
#define PLAYER_STATE2_4000       (1 << 14)
// 
#define PLAYER_STATE2_8000       (1 << 15)
// 
#define PLAYER_STATE2_10000      (1 << 16)
// 
#define PLAYER_STATE2_20000      (1 << 17)
// 
#define PLAYER_STATE2_40000      (1 << 18)
// 
#define PLAYER_STATE2_80000      (1 << 19)
// 
#define PLAYER_STATE2_100000     (1 << 20)
// 
#define PLAYER_STATE2_200000     (1 << 21)
// 
#define PLAYER_STATE2_400000     (1 << 22)
// 
#define PLAYER_STATE2_800000     (1 << 23)
// 
#define PLAYER_STATE2_1000000    (1 << 24)
// 
#define PLAYER_STATE2_2000000    (1 << 25)
// 
#define PLAYER_STATE2_4000000    (1 << 26)
// 
#define PLAYER_STATE2_8000000    (1 << 27)
// 
#define PLAYER_STATE2_10000000   (1 << 28)
// Disable drawing player
#define PLAYER_STATE2_20000000   (1 << 29)
// Lunge: small forward boost at the end of certain attack animations
#define PLAYER_STATE2_40000000   (1 << 30)
// Void-out
#define PLAYER_STATE2_80000000   (1 << 31)


// Ignores collision with floor?
#define PLAYER_STATE3_1          (1 << 0)
// 
#define PLAYER_STATE3_2          (1 << 1)
// 
#define PLAYER_STATE3_4          (1 << 2)
// 
#define PLAYER_STATE3_8          (1 << 3)
// 
#define PLAYER_STATE3_10         (1 << 4)
// 
#define PLAYER_STATE3_20         (1 << 5)
// 
#define PLAYER_STATE3_40         (1 << 6)
// 
#define PLAYER_STATE3_80         (1 << 7)
// Related to form Deku
#define PLAYER_STATE3_100        (1 << 8)
// 
#define PLAYER_STATE3_200        (1 << 9)
// 
#define PLAYER_STATE3_400        (1 << 10)
// 
#define PLAYER_STATE3_800        (1 << 11)
// goron curled
#define PLAYER_STATE3_1000       (1 << 12)
// 
#define PLAYER_STATE3_2000       (1 << 13)
// 
#define PLAYER_STATE3_4000       (1 << 14)
// 
#define PLAYER_STATE3_8000       (1 << 15)
// 
#define PLAYER_STATE3_10000      (1 << 16)
// 
#define PLAYER_STATE3_20000      (1 << 17)
// Related to form Deku
#define PLAYER_STATE3_40000      (1 << 18)
// 
#define PLAYER_STATE3_80000      (1 << 19)
// 
#define PLAYER_STATE3_100000     (1 << 20)
// Deku hopping?
#define PLAYER_STATE3_200000     (1 << 21)
// 
#define PLAYER_STATE3_400000     (1 << 22)
// 
#define PLAYER_STATE3_800000     (1 << 23)
// 
#define PLAYER_STATE3_1000000    (1 << 24)
// 
#define PLAYER_STATE3_2000000    (1 << 25)
// 
#define PLAYER_STATE3_4000000    (1 << 26)
// 
#define PLAYER_STATE3_8000000    (1 << 27)
// 
#define PLAYER_STATE3_10000000   (1 << 28)
// breman mask march?
#define PLAYER_STATE3_20000000   (1 << 29)
// 
#define PLAYER_STATE3_40000000   (1 << 30)
// 
#define PLAYER_STATE3_80000000   (1 << 31)


#define PLAYER_GET_INITMODE(thisx) (((thisx)->params & 0xF00) >> 8)

typedef enum PlayerInitMode {
    /* 0x0 */ PLAYER_INITMODE_0,
    /* 0x1 */ PLAYER_INITMODE_1, // Spawning after pulling/putting-back Master sword // OoT leftover
    /* 0x2 */ PLAYER_INITMODE_2,
    /* 0x3 */ PLAYER_INITMODE_3,
    /* 0x4 */ PLAYER_INITMODE_4,
    /* 0x5 */ PLAYER_INITMODE_5,
    /* 0x6 */ PLAYER_INITMODE_6,
    /* 0x7 */ PLAYER_INITMODE_7,
    /* 0x8 */ PLAYER_INITMODE_8,
    /* 0x9 */ PLAYER_INITMODE_9,
    /* 0xA */ PLAYER_INITMODE_A,
    /* 0xB */ PLAYER_INITMODE_B,
    /* 0xC */ PLAYER_INITMODE_TELESCOPE,
    /* 0xD */ PLAYER_INITMODE_D,
    /* 0xE */ PLAYER_INITMODE_E,
    /* 0xF */ PLAYER_INITMODE_F
} PlayerInitMode;

#define PLAYER_PARAMS(startBgCamIndex, initMode) ((startBgCamIndex & 0xFF) | ((initMode & 0xF) << 8))

typedef void (*PlayerActionFunc)(struct Player* this, struct PlayState* play);
typedef s32 (*PlayerFuncAC4)(struct Player* this, struct PlayState* play);
typedef void (*PlayerFuncD58)(struct PlayState* play, struct Player* this);


typedef struct Player {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s8 currentShield;
    /* 0x145 */ s8 currentBoots;
    /* 0x146 */ s8 heldItemButton;
    /* 0x147 */ s8 heldItemAction; // PlayerItemAction enum
    /* 0x148 */ u8 heldItemId; // ItemId enum
    /* 0x149 */ s8 prevBoots;
    /* 0x14A */ s8 itemAction; // PlayerItemAction enum
    /* 0x14B */ u8 transformation; // PlayerTransformation enum
    /* 0x14C */ u8 modelGroup; // PlayerModelGroup enum
    /* 0x14D */ u8 nextModelGroup;
    /* 0x14E */ s8 unk_14E;
    /* 0x14F */ u8 modelAnimType; // PlayerAnimType enum
    /* 0x150 */ u8 leftHandType;
    /* 0x151 */ u8 rightHandType;
    /* 0x152 */ u8 sheathType;
    /* 0x153 */ u8 currentMask; // PlayerMask enum
    /* 0x154 */ s8 unk_154;
    /* 0x155 */ u8 prevMask;
    /* 0x158 */ Gfx** rightHandDLists;
    /* 0x15C */ Gfx** leftHandDLists;
    /* 0x160 */ Gfx** sheathDLists;
    /* 0x164 */ Gfx** waistDLists;
    /* 0x168 */ f32 unk_168;
    /* 0x16C */ f32 unk_16C;
    /* 0x170 */ f32 unk_170;
    /* 0x174 */ f32 unk_174;
    /* 0x178 */ f32 unk_178;
    /* 0x17C */ f32 unk_17C;
    /* 0x180 */ f32 unk_180;
    /* 0x184 */ f32 unk_184;
    /* 0x188 */ f32 unk_188;
    /* 0x18C */ f32 unk_18C;
    /* 0x190 */ f32 unk_190;
    /* 0x194 */ f32 unk_194;
    /* 0x198 */ f32 unk_198;
    /* 0x19C */ f32 unk_19C;
    /* 0x1A0 */ f32 unk_1A0;
    /* 0x1A4 */ f32 unk_1A4;
    /* 0x1A8 */ f32 unk_1A8;
    /* 0x1AC */ f32 unk_1AC;
    /* 0x1B0 */ f32 unk_1B0;
    /* 0x1B4 */ s16 unk_1B4;
    /* 0x1B6 */ char unk_1B6[2];
    /* 0x1B8 */ u8 giObjectLoading;
    /* 0x1BC */ DmaRequest giObjectDmaRequest;
    /* 0x1DC */ OSMesgQueue giObjectLoadQueue;
    /* 0x1F4 */ OSMesg giObjectLoadMsg;
    /* 0x1F8 */ void* giObjectSegment;
    /* 0x1FC */ u8 maskObjectLoadState;
    /* 0x1FD */ s8 maskId;
    /* 0x200 */ DmaRequest maskDmaRequest;
    /* 0x220 */ OSMesgQueue maskObjectLoadQueue;
    /* 0x238 */ OSMesg maskObjectLoadMsg;
    /* 0x23C */ void* maskObjectSegment;
    /* 0x240 */ SkelAnime skelAnime;
    /* 0x284 */ SkelAnime unk_284;
    /* 0x2C8 */ SkelAnime unk_2C8;
    /* 0x30C */ Vec3s jointTable[5];
    /* 0x32A */ Vec3s morphTable[5];
    /* 0x348 */ s16 eyeTexId;
    /* 0x34A */ s16 eyeAnimTimer;
    /* 0x34C */ Actor* heldActor;
    /* 0x350 */ PosRot leftHandWorld;
    /* 0x364 */ Actor* rightHandActor;
    /* 0x368 */ PosRot rightHandWorld;
    /* 0x37C */ s8 doorType; // PlayerDoorType enum
    /* 0x37D */ s8 doorDirection;
    /* 0x37E */ s8 doorTimer;
    /* 0x37F */ s8 doorNext; // used with spiral staircase
    /* 0x380 */ Actor* doorActor;
    /* 0x384 */ s16 getItemId; // GetItemId enum
    /* 0x386 */ u16 getItemDirection;
    /* 0x388 */ Actor* interactRangeActor;
    /* 0x38C */ s8 mountSide;
    /* 0x390 */ Actor* rideActor;
    /* 0x394 */ u8 csMode; // PlayerCsMode enum
    /* 0x395 */ u8 prevCsMode; // PlayerCsMode enum
    /* 0x396 */ u8 unk_396; // currentActorActionId?
    /* 0x397 */ u8 unk_397; // PlayerDoorType enum
    /* 0x398 */ Actor* unk_398;
    /* 0x39C */ UNK_TYPE1 unk_39C[0x4];
    /* 0x3A0 */ Vec3f unk_3A0;
    /* 0x3AC */ Vec3f unk_3AC;
    /* 0x3B8 */ u16 unk_3B8;
    /* 0x3BA */ s16 unk_3BA;
    /* 0x3BC */ s16 subCamId;
    /* 0x3BE */ char unk_3BE[2];
    /* 0x3C0 */ Vec3f unk_3C0;
    /* 0x3CC */ s16 unk_3CC;
    /* 0x3CE */ s8 unk_3CE;
    /* 0x3CF */ u8 unk_3CF;
    /* 0x3D0 */ struct_80122D44_arg1 unk_3D0;
    /* 0x4E4 */ UNK_TYPE1 unk_4E4[0x20];
    /* 0x504 */ LightNode* lightNode;
    /* 0x508 */ LightInfo lightInfo;
    /* 0x518 */ ColliderCylinder cylinder;
    /* 0x564 */ ColliderQuad meleeWeaponQuads[2];
    /* 0x664 */ ColliderQuad shieldQuad;
    /* 0x6E4 */ ColliderCylinder shieldCylinder;
    /* 0x730 */ Actor* targetedActor; // Z/L-Targeted actor
    /* 0x734 */ char unk_734[4];
    /* 0x738 */ s32 unk_738;
    /* 0x73C */ s32 meleeWeaponEffectIndex[3];
    /* 0x748 */ PlayerActionFunc actionFunc;
    /* 0x74C */ u8 jointTableBuffer[PLAYER_LIMB_BUF_SIZE];
    /* 0x7EB */ u8 morphTableBuffer[PLAYER_LIMB_BUF_SIZE];
    /* 0x88A */ u8 blendTableBuffer[PLAYER_LIMB_BUF_SIZE];
    /* 0x929 */ u8 unk_929[PLAYER_LIMB_BUF_SIZE];
    /* 0x9C8 */ u8 unk_9C8[PLAYER_LIMB_BUF_SIZE];
    /* 0xA68 */ PlayerAgeProperties* ageProperties; // repurposed as "transformation properties"?
    /* 0xA6C */ u32 stateFlags1;
    /* 0xA70 */ u32 stateFlags2;
    /* 0xA74 */ u32 stateFlags3;
    /* 0xA78 */ Actor* unk_A78;
    /* 0xA7C */ Actor* boomerangActor;
    /* 0xA80 */ Actor* tatlActor;
    /* 0xA84 */ s16 tatlTextId;
    /* 0xA86 */ s8 unk_A86; // actorCutsceneIndex?
    /* 0xA87 */ s8 exchangeItemId; // PlayerItemAction enum
    /* 0xA88 */ Actor* talkActor;
    /* 0xA8C */ f32 talkActorDistance;
    /* 0xA90 */ Actor* unk_A90;
    /* 0xA94 */ f32 unk_A94;
    /* 0xA98 */ Actor* unk_A98;
    /* 0xA9C */ f32 secretRumbleCharge; // builds per frame until discharges with a rumble request
    /* 0xAA0 */ f32 closestSecretDistSq; // Used to augment `secretRumbleCharge`. Cleared every frame
    /* 0xAA4 */ s8 unk_AA4;
    /* 0xAA5 */ u8 unk_AA5;
    /* 0xAA6 */ u16 unk_AA6; // flags of some kind
    /* 0xAA8 */ s16 unk_AA8;
    /* 0xAAA */ s16 unk_AAA;
    /* 0xAAC */ Vec3s unk_AAC;
    /* 0xAB2 */ Vec3s unk_AB2;
    /* 0xAB8 */ f32 unk_AB8;
    /* 0xABC */ f32 unk_ABC;
    /* 0xAC0 */ f32 unk_AC0;
    /* 0xAC4 */ PlayerFuncAC4 unk_AC4;
    /* 0xAC8 */ f32 unk_AC8;
    /* 0xACC */ s16 unk_ACC;
    /* 0xACE */ s8 unk_ACE;
    /* 0xACF */ u8 putAwayCountdown; // Frames to wait before showing "Put Away" on A
    /* 0xAD0 */ f32 linearVelocity;
    /* 0xAD4 */ s16 currentYaw;
    /* 0xAD6 */ s16 targetYaw;
    /* 0xAD8 */ u16 underwaterTimer;
    /* 0xADA */ s8 meleeWeaponAnimation;
    /* 0xADB */ s8 meleeWeaponState;
    /* 0xADC */ s8 unk_ADC;
    /* 0xADD */ s8 unk_ADD;
    /* 0xADE */ u8 unk_ADE;
    /* 0xADF */ s8 unk_ADF[4]; // Circular buffer used for testing for triggering a quickspin
    /* 0xAE3 */ s8 unk_AE3[4]; // Circular buffer used for ?
    /* 0xAE7 */ s8 unk_AE7; // a timer
    /* 0xAE8 */ s16 unk_AE8; // multipurpose timer
    /* 0xAEC */ f32 unk_AEC;
    /* 0xAF0 */ Vec3f unk_AF0[2];
    /* 0xB08 */ f32 unk_B08[2]; // TODO: Investigate if this member actually is an array
    /* 0xB10 */ f32 unk_B10[6];
    /* 0xB28 */ s16 unk_B28; //Burning stick timer?
    /* 0xB2A */ s8 getItemDrawId;
    /* 0xB2B */ s8 unk_B2B;
    /* 0xB2C */ f32 windSpeed;
    /* 0xB30 */ s16 windAngleX;
    /* 0xB32 */ s16 windAngleY;
    /* 0xB34 */ f32 unk_B34;
    /* 0xB38 */ f32 unk_B38;
    /* 0xB3C */ f32 unk_B3C;
    /* 0xB40 */ f32 unk_B40;
    /* 0xB44 */ f32 unk_B44;
    /* 0xB48 */ f32 unk_B48;
    /* 0xB4C */ s16 unk_B4C;
    /* 0xB4E */ s16 unk_B4E;
    /* 0xB50 */ f32 unk_B50;
    /* 0xB54 */ f32 wallHeight; // height used to determine whether link can climb or grab a ledge at the top
    /* 0xB58 */ f32 wallDistance; // distance to the colliding wall plane
    /* 0xB5C */ u8 unk_B5C;
    /* 0xB5D */ u8 unk_B5D;
    /* 0xB5E */ u8 unk_B5E;
    /* 0xB5F */ u8 unk_B5F;
    /* 0xB60 */ u16 unk_B60; // blast mask timer?
    /* 0xB62 */ s16 unk_B62;
    /* 0xB64 */ u8 unk_B64;
    /* 0xB65 */ u8 shockTimer;
    /* 0xB66 */ u8 unk_B66;
    /* 0xB67 */ u8 unk_B67; // deku remaining hops counter
    /* 0xB68 */ s16 fallStartHeight; // last truncated Y position before falling
    /* 0xB6A */ s16 fallDistance; // truncated Y distance the player has fallen so far (positive is down)
    /* 0xB6C */ s16 unk_B6C;
    /* 0xB6E */ s16 unk_B6E;
    /* 0xB70 */ s16 unk_B70;
    /* 0xB72 */ u16 floorSfxOffset;
    /* 0xB74 */ u8 unk_B74;
    /* 0xB75 */ u8 unk_B75;
    /* 0xB76 */ s16 unk_B76;
    /* 0xB78 */ f32 unk_B78;
    /* 0xB7C */ f32 unk_B7C;
    /* 0xB80 */ f32 unk_B80;
    /* 0xB84 */ s16 unk_B84;
    /* 0xB86 */ s16 unk_B86[2]; // unknown length
    /* 0xB8A */ s16 unk_B8A;
    /* 0xB8C */ s16 unk_B8C;
    /* 0xB8E */ s16 unk_B8E;
    /* 0xB90 */ s16 unk_B90;
    /* 0xB92 */ s16 unk_B92;
    /* 0xB94 */ s16 unk_B94;
    /* 0xB96 */ s16 unk_B96;
    /* 0xB98 */ WeaponInfo meleeWeaponInfo[3];
    /* 0xBEC */ Vec3f bodyPartsPos[PLAYER_BODYPART_MAX];
    /* 0xCC4 */ MtxF mf_CC4;
    /* 0xD04 */ MtxF shieldMf;
    /* 0xD44 */ u8 isBurning;
    /* 0xD45 */ u8 flameTimers[PLAYER_BODYPART_MAX];
    /* 0xD57 */ u8 unk_D57;
    /* 0xD58 */ PlayerFuncD58 unk_D58;
    /* 0xD5C */ s8 invincibilityTimer; // prevents damage when nonzero (positive = visible, counts towards zero each frame)
    /* 0xD5D */ u8 unk_D5D;
    /* 0xD5E */ u8 unk_D5E;
    /* 0xD5F */ u8 unk_D5F;
    /* 0xD60 */ f32 unk_D60;
    /* 0xD64 */ s16 unk_D64;
    /* 0xD66 */ u16 unk_D66; // sfx
    /* 0xD68 */ s16 unk_D68;
    /* 0xD6A */ s8 unk_D6A;
    /* 0xD6B */ u8 unk_D6B;
    /* 0xD6C */ Vec3f unk_D6C; // previous body part 0 position
} Player; // size = 0xD78

#endif
