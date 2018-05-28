/*
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "PassiveAI.h"
#include "black_temple.h"
#include "Player.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiersImpl.h"

enum IllidanSay
{
    // Illidan
    SAY_ILLIDAN_MINION                      = 0,
    SAY_ILLIDAN_KILL                        = 1,
    SAY_ILLIDAN_TAKEOFF                     = 2,
    SAY_ILLIDAN_SUMMONFLAMES                = 3,
    SAY_ILLIDAN_EYE_BLAST                   = 4,
    SAY_ILLIDAN_MORPH                       = 5,
    SAY_ILLIDAN_ENRAGE                      = 6,
    SAY_ILLIDAN_TAUNT                       = 7,
    SAY_ILLIDAN_DUPLICITY                   = 8,
    SAY_ILLIDAN_UNCONVINCED                 = 9,
    SAY_ILLIDAN_PREPARED                    = 10,
    SAY_ILLIDAN_SHADOW_PRISON               = 11,
    SAY_ILLIDAN_CONFRONT_MAIEV              = 12,
    SAY_ILLIDAN_FRENZY                      = 13,
    SAY_ILLIDAN_DEFEATED                    = 14,

    // Maiev Shadowsong
    SAY_MAIEV_SHADOWSONG_TAUNT              = 0,
    SAY_MAIEV_SHADOWSONG_APPEAR             = 1,
    SAY_MAIEV_SHADOWSONG_JUSTICE            = 2,
    SAY_MAIEV_SHADOWSONG_TRAP               = 3,
    SAY_MAIEV_SHADOWSONG_DOWN               = 4,
    SAY_MAIEV_SHADOWSONG_FINISHED           = 5,
    SAY_MAIEV_SHADOWSONG_OUTRO              = 6,
    SAY_MAIEV_SHADOWSONG_FAREWELL           = 7,

    // Flame of Azzinoth
    EMOTE_AZZINOTH_GAZE                     = 0,

    // Akama
    SAY_AKAMA_DOOR                          = 0,
    SAY_AKAMA_ALONE                         = 1,
    SAY_AKAMA_SALUTE                        = 2,
    SAY_AKAMA_BETRAYER                      = 3,
    SAY_AKAMA_FREE                          = 4,
    SAY_AKAMA_TIME_HAS_COME                 = 5,
    SAY_AKAMA_MINIONS                       = 6,
    SAY_AKAMA_LIGHT                         = 7,
    SAY_AKAMA_FINISH                        = 8,

    // Spirits
    SAY_SPIRIT_ALONE                        = 0,

    // Direct Sounds
    ILLIDAN_TAKEOFF_SOUND_ID                = 11479,
    ILLIDAN_WARGLAIVE_SOUND_ID              = 11480,
    WARGLAIVE_SPAWN_SOUND_ID                = 11689,
    EVENT_BT_SUMMIT_WALK_SOUND_ID           = 11717,
    EVENT_BT_SUMMIT_WALK_3_SOUND_ID         = 11725,
    EVENT_BT_STORM_WALK_HERO_2_SOUND_ID     = 11727,
    EVENT_BT_STORM_WALK_UNI_3_SOUND_ID      = 11729,
    EVENT_BT_ARRIVAL_WALK_HERO_1_SOUND_ID   = 11728
};

enum IllidanSpells
{
    // Akama
    SPELL_AKAMA_DOOR_CHANNEL            = 41268,
    SPELL_AKAMA_DOOR_FAIL               = 41271,
    SPELL_HEALING_POTION                = 40535,
    SPELL_CHAIN_LIGHTNING               = 40536,
    SPELL_AKAMA_TELEPORT                = 41077,
    SPELL_AKAMA_DESPAWN                 = 41242,

    // Spirits
    SPELL_DEATHSWORN_DOOR_CHANNEL       = 41269,

    // Door Trigger
    SPELL_ARCANE_EXPLOSION              = 35426,

    // Blade of Azzinoth
    SPELL_BIRTH                         = 40031,
    SPELL_SUMMON_TEAR_OF_AZZINOTH       = 39855,
    SPELL_AZZINOTH_CHANNEL              = 39857,
    SPELL_GLAIVE_RETURNS                = 39873,

    // Flame of Azzinoth
    SPELL_FLAME_TEAR_OF_AZZINOTH        = 39856,
    SPELL_CHARGE                        = 42003,
    SPELL_FLAME_BLAST                   = 40631,
    SPELL_UNCAGED_WRATH                 = 39869,

    // Maiev
    SPELL_TELEPORT_VISUAL               = 41236,
    SPELL_CAGE_TRAP_SUMMON              = 40694,
    SPELL_SHADOW_STRIKE                 = 40685,
    SPELL_THROW_DAGGER                  = 41152,
    SPELL_MAIEV_DOWN                    = 40409,

    // Cage Trap Disturb Trigger
    SPELL_CAGE_TRAP_PERIODIC            = 40761,

    // Shadow Demon
    SPELL_SHADOW_DEMON_PASSIVE          = 41079,
    SPELL_FIND_TARGET                   = 41081,
    SPELL_PARALYZE                      = 41083,
    SPELL_CONSUME_SOUL                  = 41080,

    // Player
    SPELL_SUMMON_PARASITIC_SHADOWFIENDS = 41915,
    SPELL_BLAZE_SUMMON                  = 40637,

    // Illidan DB Target
    SPELL_EYE_BLAST_TRIGGER             = 40017,

    // Cage Trap Summon Spells
    SPELL_SUMMON_CAGE_TRAP_1            = 40696,
    SPELL_SUMMON_CAGE_TRAP_2            = 40697,
    SPELL_SUMMON_CAGE_TRAP_3            = 40698,
    SPELL_SUMMON_CAGE_TRAP_4            = 40699,
    SPELL_SUMMON_CAGE_TRAP_5            = 40700,
    SPELL_SUMMON_CAGE_TRAP_6            = 40701,
    SPELL_SUMMON_CAGE_TRAP_7            = 40702,
    SPELL_SUMMON_CAGE_TRAP_8            = 40703,

    // Glaive Target
    SPELL_RANGE_MARKER                  = 41997,
    SPELL_SUMMON_GLAIVE                 = 41466,

    // Illidan
    SPELL_FLAME_CRASH                   = 40832,
    SPELL_SHEAR                         = 41032,
    SPELL_DRAW_SOUL                     = 40904,
    SPELL_DRAW_SOUL_HEAL                = 40903,
    SPELL_PARASITIC_SHADOWFIEND         = 41917,
    SPELL_AGONIZING_FLAMES              = 40932,
    SPELL_AGONIZING_FLAMES_SELECTOR     = 40834,
    SPELL_FRENZY                        = 40683,
    SPELL_THROW_GLAIVE                  = 39849,
    SPELL_THROW_GLAIVE2                 = 39635,
    SPELL_FIREBALL                      = 40598,
    SPELL_DARK_BARRAGE                  = 40585,
    SPELL_DEMON_TRANSFORM_1             = 40511,
    SPELL_DEMON_TRANSFORM_2             = 40398,
    SPELL_DEMON_TRANSFORM_3             = 40510,
    SPELL_DEMON_FORM                    = 40506,
    SPELL_AURA_OF_DREAD                 = 41142,
    SPELL_SHADOW_BLAST                  = 41078,
    SPELL_FLAME_BURST                   = 41126,
    SPELL_FLAME_BURST_EFFECT            = 41131,
    SPELL_KNEEL                         = 39656,
    SPELL_SHADOW_PRISON                 = 40647,
    SPELL_EMOTE_TALK_QUESTION           = 41616,
    SPELL_BERSERK                       = 45078,
    SPELL_SUMMON_MAIEV                  = 40403,
    SPELL_TELEPORT_MAIEV                = 41221,
    SPELL_CLEAR_ALL_DEBUFFS             = 34098,
    SPELL_DEATH                         = 41218,
    SPELL_QUIET_SUICIDE                 = 3617,
    SPELL_SUMMON_SHADOWDEMON            = 41117,
    SPELL_CAGED_TRAP_TELEPORT           = 40693,
    SPELL_CAGE_TRAP                     = 40760,
    SPELL_CAGED_DEBUFF                  = 40695,
    SPELL_EYE_BLAST                     = 39908
};

enum IllidanMisc
{
    GOSSIP_START_INTRO           = 0,
    GOSSIP_START_FIGHT           = 1,
    SUMMON_GROUP                 = 1,
    DATA_AKAMA_TELEPORT_POSITION = 0,
    MAX_MINIONS_NUMBER           = 10,
    SPELL_GLAIVE_VISUAL_KIT      = 7668
};

enum IllidanActions
{
    ACTION_START_ENCOUNTER = 5,
    ACTION_FREE,
    ACTION_INTRO_DONE,
    ACTION_START_MINIONS,
    ACTION_START_MINIONS_WEAVE,
    ACTION_START_PHASE_2,
    ACTION_FLAME_DEAD,
    ACTION_FINALIZE_AIR_PHASE,
    ACTION_START_PHASE_4,
    ACTION_ILLIDAN_CAGED,
    ACTION_START_OUTRO,
    ACTION_MAIEV_DOWN_FADE
};

enum IllidanPhases
{
    PHASE_INTRO = 1,
    PHASE_1,
    PHASE_MINIONS,
    PHASE_2,
    PHASE_3,
    PHASE_4
};

enum IllidanSplineMovement
{
    SPLINE_ILLIDARI_COUNCIL = 1,
    SPLINE_STAIRS           = 2,
    SPLINE_ILLIDAN_ROOM     = 3,
    SPLINE_FACE_ILLIDAN     = 4,
    SPLINE_TELEPORT         = 5,
    SPLINE_MINIONS          = 6,
    SPLINE_MOVE_BACK        = 7
};

enum IllidanPoints
{
    POINT_ILLIDARI_COUNCIL = 1,
    POINT_STAIRS,
    POINT_ILLIDAN_ROOM,
    POINT_FACE_ILLIDAN,
    POINT_TELEPORT,
    POINT_MINIONS,
    POINT_THROW_GLAIVE,
    POINT_RANDOM_PILLAR,
    POINT_DB_TARGET,
    POINT_ILLIDAN_MIDDLE,
    POINT_MOVE_BACK,
    POINT_ILLIDAN
};

enum IllidanEventGroup
{
    GROUP_PHASE_ALL = 0,
    GROUP_PHASE_1,
    GROUP_PHASE_2,
    GROUP_PHASE_3,
    GROUP_PHASE_DEMON,
    GROUP_PHASE_4
};

enum IllidanEvents
{
    // Akama
    EVENT_TELEPORT = 1,
    EVENT_MOVE_TO_ILLIDARI_ROOM,
    EVENT_AKAMA_SAY_DOOR,
    EVENT_AKAMA_DOOR_FAIL,
    EVENT_AKAMA_SAY_ALONE,
    EVENT_SUMMON_SPIRITS,
    EVENT_SPIRIT_SAY_1,
    EVENT_SPIRIT_SAY_2,
    EVENT_AKAMA_DOOR_SUCCESS,
    EVENT_AKAMA_THANKS,
    EVENT_SPIRIT_SALUTE,
    EVENT_RUN_FROM_ILLIDAN_ROOM,
    EVENT_START_ILLIDAN,
    EVENT_FREE,
    EVENT_TIME_HAS_COME,
    EVENT_ROAR,
    EVENT_CHANGE_ORIENTATION,
    EVENT_HEALING_POTION,
    EVENT_AKAMA_MINIONS,
    EVENT_AKAMA_MINIONS_EMOTE,
    EVENT_AKAMA_MINIONS_MOVE,
    EVENT_AKAMA_MINIONS_MOVE_2,
    EVENT_CHAIN_LIGHTNING,
    EVENT_AKAMA_MOVE_BACK,
    EVENT_AKAMA_MOVE_TO_ILLIDAN,
    EVENT_AKAMA_LIGHT_TEXT,
    EVENT_FINAL_SALUTE,
    EVENT_AKAMA_DESPAWN,
    EVENT_AKAMA_START_SOUND,

    // Illidan Stormrage
    EVENT_START_INTRO,
    EVENT_UNCONVINCED,
    EVENT_PREPARED,
    EVENT_ENCOUNTER_START,
    EVENT_EVADE_CHECK,
    EVENT_FLAME_CRASH,
    EVENT_DRAW_SOUL,
    EVENT_SHEAR,
    EVENT_BERSERK,
    EVENT_PARASITIC_SHADOWFIEND,
    EVENT_MINIONS_WEAVE,
    EVENT_MOVE_TO_WARGLAIVE_POINT,
    EVENT_FACE_MIDDLE,
    EVENT_FLY,
    EVENT_THROW_WARGLAIVE,
    EVENT_THROW_WARGLAIVE_2,
    EVENT_FLY_TO_RANDOM_PILLAR,
    EVENT_FIREBALL,
    EVENT_EYE_BLAST,
    EVENT_DARK_BARRAGE,
    EVENT_GLAIVE_EMOTE,
    EVENT_RESUME_COMBAT,
    EVENT_AGONIZING_FLAMES,
    EVENT_DEMON,
    EVENT_DEMON_TEXT,
    EVENT_CANCEL_DEMON_FORM,
    EVENT_RESUME_COMBAT_DEMON,
    EVENT_FLAME_BURST,
    EVENT_SHADOW_DEMON,
    EVENT_SCHEDULE_DEMON_SPELLS,
    EVENT_SHADOW_BLAST,
    EVENT_PHASE_4_DELAYED,
    EVENT_SHADOW_PRISON_TEXT,
    EVENT_SUMMON_MAIEV,
    EVENT_CONFRONT_MAIEV_TEXT,
    EVENT_RESUME_COMBAT_PHASE_4,
    EVENT_FRENZY,
    EVENT_TAUNT,
    EVENT_DEFEATED_TEXT,
    EVENT_QUIET_SUICIDE,

    // Flame of Azzinoth
    EVENT_ENGAGE,
    EVENT_FLAME_CHARGE,
    EVENT_FLAME_BLAST,

    // Maiev
    EVENT_MAIEV_APPEAR,
    EVENT_MAIEV_JUSTICE_TEXT,
    EVENT_MAIEV_EXCLAMATION,
    EVENT_MAIEV_YES,
    EVENT_MAIEV_ROAR,
    EVENT_MAIEV_COMBAT,
    EVENT_CAGE_TRAP,
    EVENT_SHADOW_STRIKE,
    EVENT_THROW_DAGGER,
    EVENT_MAIEV_OUTRO_TEXT,
    EVENT_MAIEV_FAREWELL_TEXT,
    EVENT_MAIEV_TELEPORT_DESPAWN
};

Position const AkamaTeleportPositions[2] =
{
    { 609.7720f, 308.4560f,  271.826f }, // Illidari Council Position
    { 752.2771f, 369.9401f, 353.1584f }  // Minions Position
};

Position const MinionsSpawnPositions[10] =
{
    { 745.2552f, 322.1574f, 310.4596f, 6.038839f  },
    { 747.0576f, 326.4268f, 309.0688f, 0.0f       },
    { 743.9686f, 289.6447f, 311.1807f, 6.056293f  },
    { 748.8422f, 288.0620f, 310.9782f, 1.884956f  },
    { 751.0878f, 327.6505f, 309.4576f, 6.178465f  },
    { 750.0472f, 282.3274f, 309.4353f, 3.071779f  },
    { 754.0332f, 325.8136f, 310.3195f, 2.9147f    },
    { 753.8425f, 286.562f,  310.9353f, 1.029744f  },
    { 745.3237f, 283.986f,  309.2765f, 0.6283185f },
    { 750.0322f, 323.6064f, 310.2757f, 5.497787f  }
};

Position const IllidanPhase2Positions[4] =
{
    { 705.921997f, 337.145996f, 370.083008f, 3.961900f },
    { 706.226990f, 273.264008f, 370.083008f, 2.251072f },
    { 658.830017f, 265.098999f, 370.083008f, 0.850345f },
    { 656.859009f, 344.071991f, 370.083008f, 5.235990f }
};

Position const IllidanMiddlePoint = { 676.6479f, 304.7606f, 354.1909f , 6.230825f };

Position const IllidanDBTargetSpawnPositions[4] =
{
    { 710.8815f, 306.4028f, 353.5962f, 2.391101f },
    { 652.105f,  259.5127f, 353.0794f, 0.122173f },
    { 642.7164f, 305.2436f, 353.5596f, 3.438299f },
    { 710.8815f, 306.4028f, 353.5962f, 2.391101f }
};

Position const IllidanDBTargetPoints[4] =
{
    { 660.3492f, 345.5749f, 353.2961f },
    { 701.6755f, 297.3358f, 354.041f  },
    { 706.7507f, 269.4593f, 353.2778f },
    { 660.3492f, 345.5749f, 353.2961f }
};

uint32 const SummonCageTrapSpells[8] =
{
    SPELL_SUMMON_CAGE_TRAP_1,
    SPELL_SUMMON_CAGE_TRAP_2,
    SPELL_SUMMON_CAGE_TRAP_3,
    SPELL_SUMMON_CAGE_TRAP_4,
    SPELL_SUMMON_CAGE_TRAP_5,
    SPELL_SUMMON_CAGE_TRAP_6,
    SPELL_SUMMON_CAGE_TRAP_7,
    SPELL_SUMMON_CAGE_TRAP_8
};

class SummonWarglaiveEvent : public BasicEvent
{
public:
    SummonWarglaiveEvent(Unit* owner) : BasicEvent(), _owner(owner) { }

    bool Execute(uint64 /*eventTime*/, uint32 /*diff*/) override
    {
        _owner->CastSpell(_owner, SPELL_RANGE_MARKER, true);
        _owner->CastSpell(_owner, SPELL_SUMMON_GLAIVE, true);
        return true;
    }

private:
    Unit* _owner;
};

class ChargeTargetSelector : public std::unary_function<Unit*, bool>
{
public:
    ChargeTargetSelector(Unit const* unit) : _me(unit) { }

    bool operator()(Unit* unit) const
    {
        return _me->GetDistance2d(unit) > 25.0f;
    }

private:
    Unit const* _me;
};

class boss_illidan_stormrage : public CreatureScript
{
public:
    boss_illidan_stormrage() : CreatureScript("boss_illidan_stormrage") { }

    struct boss_illidan_stormrageAI : public BossAI
    {
        boss_illidan_stormrageAI(Creature* creature) : BossAI(creature, DATA_ILLIDAN_STORMRAGE),
            _intro(true), _minionsCount(0), _flameCount(0), _orientation(0.0f), _pillarIndex(0), _phase(0), _dead(false), _isDemon(false) { }

        void Reset() override
        {
            _Reset();
            specialEvents.Reset();
            me->SummonCreatureGroup(SUMMON_GROUP);
            me->LoadEquipment(1, true);
            me->SetSheath(SHEATH_STATE_UNARMED);
            me->SetControlled(false, UNIT_STATE_ROOT);
            me->SetDisableGravity(false);
            _dead = false;
            _minionsCount = 0;
            _flameCount = 0;
            _phase = PHASE_1;
            _isDemon = false;
            if (_intro && instance->GetBossState(DATA_ILLIDARI_COUNCIL) == DONE)
                if (Creature* akama = instance->GetCreature(DATA_AKAMA))
                    akama->AI()->DoAction(ACTION_ACTIVE_AKAMA_INTRO);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
            me->SetCanDualWield(true);
            if (GameObject* musicController = instance->GetGameObject(DATA_ILLIDAN_MUSIC_CONTROLLER))
                musicController->PlayDirectMusic(EVENT_BT_SUMMIT_WALK_3_SOUND_ID);
            specialEvents.ScheduleEvent(EVENT_EVADE_CHECK, Seconds(10));
            specialEvents.ScheduleEvent(EVENT_BERSERK, Minutes(25));
            ScheduleEvents(GROUP_PHASE_1, GROUP_PHASE_1);
            events.ScheduleEvent(EVENT_TAUNT, Seconds(30), Seconds(60), GROUP_PHASE_ALL);
        }

        void ChangeOrientation(float orientation)
        {
            _orientation = orientation;
            events.ScheduleEvent(EVENT_CHANGE_ORIENTATION, Milliseconds(1), GROUP_PHASE_ALL);
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_ILLIDAN_KILL);
        }

        void ScheduleEvents(uint8 phase, uint8 group)
        {
            switch (phase)
            {
                case GROUP_PHASE_1:
                    events.ScheduleEvent(EVENT_FLAME_CRASH, Seconds(30), group);
                    events.ScheduleEvent(EVENT_DRAW_SOUL, Seconds(34), group);
                    events.ScheduleEvent(EVENT_SHEAR, Seconds(10), group);
                    events.ScheduleEvent(EVENT_PARASITIC_SHADOWFIEND, Seconds(26), group);
                    break;
                case GROUP_PHASE_2:
                    events.ScheduleEvent(EVENT_FIREBALL, Seconds(1), Seconds(8), group);
                    events.ScheduleEvent(EVENT_EYE_BLAST, Seconds(1), Seconds(30), group);
                    if (roll_chance_i(50))
                        events.ScheduleEvent(EVENT_DARK_BARRAGE, Seconds(1), Seconds(20), group);
                    break;
                case GROUP_PHASE_3:
                    ScheduleEvents(GROUP_PHASE_1, group);
                    events.ScheduleEvent(EVENT_AGONIZING_FLAMES, Seconds(21), group);
                    events.ScheduleEvent(EVENT_DEMON, Seconds(60), group);
                    break;
                case GROUP_PHASE_DEMON:
                    events.ScheduleEvent(EVENT_SHADOW_BLAST, Seconds(1), group);
                    events.ScheduleEvent(EVENT_FLAME_BURST, Seconds(6), group);
                    events.ScheduleEvent(EVENT_SHADOW_DEMON, Seconds(18), Seconds(30), group);
                    break;
                case GROUP_PHASE_4:
                    ScheduleEvents(GROUP_PHASE_3, group);
                    events.ScheduleEvent(EVENT_FRENZY, Seconds(40), group);
                    break;
                default:
                    break;
            }
        }

        void JustSummoned(Creature* summon) override
        {
            BossAI::JustSummoned(summon);
            if (summon->GetEntry() == NPC_ILLIDARI_ELITE)
                if (Creature* akama = instance->GetCreature(DATA_AKAMA))
                {
                    summon->CombatStart(akama);
                    summon->AddThreat(akama, 1000.0f);
                }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
        {
            if (summon->GetEntry() == NPC_ILLIDARI_ELITE)
                _minionsCount--;
        }

        void EnterEvadeMode(EvadeReason /*why*/) override
        {
            summons.DespawnAll();
            specialEvents.Reset();
            _DespawnAtEvade();
        }

        void DoAction(int32 actionId) override
        {
            switch (actionId)
            {
                case ACTION_START_ENCOUNTER:
                    events.SetPhase(PHASE_INTRO);
                    me->RemoveAurasDueToSpell(SPELL_KNEEL);
                    events.ScheduleEvent(EVENT_START_INTRO, Seconds(2), GROUP_PHASE_ALL);
                    events.ScheduleEvent(EVENT_UNCONVINCED, Seconds(24), GROUP_PHASE_ALL);
                    if (Creature* akama = instance->GetCreature(DATA_AKAMA))
                        akama->AI()->DoAction(ACTION_FREE);
                    break;
                case ACTION_INTRO_DONE:
                    _intro = false;
                    break;
                case ACTION_START_MINIONS:
                    Talk(SAY_ILLIDAN_MINION);
                    if (Creature* akama = instance->GetCreature(DATA_AKAMA))
                        akama->AI()->DoAction(ACTION_START_MINIONS);
                    break;
                case ACTION_START_MINIONS_WEAVE:
                    events.ScheduleEvent(EVENT_MINIONS_WEAVE, Milliseconds(1), GROUP_PHASE_ALL);
                    break;
                case ACTION_START_PHASE_2:
                {
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                    me->SetDisableGravity(true);
                    DoPlaySoundToSet(me, ILLIDAN_TAKEOFF_SOUND_ID);
                    events.ScheduleEvent(EVENT_FLY, Seconds(1), GROUP_PHASE_ALL);
                    events.CancelEventGroup(GROUP_PHASE_1);
                    break;
                }
                case ACTION_FLAME_DEAD:
                    _flameCount++;
                    if (_flameCount == 2)
                    {
                        _flameCount = 0;
                        DoAction(ACTION_FINALIZE_AIR_PHASE);
                    }
                    break;
                case ACTION_FINALIZE_AIR_PHASE:
                    me->InterruptNonMeleeSpells(false);
                    me->GetMotionMaster()->Clear();
                    events.CancelEventGroup(GROUP_PHASE_2);
                    _phase = PHASE_3;
                    events.CancelEvent(EVENT_FLY_TO_RANDOM_PILLAR);
                    me->GetMotionMaster()->MovePoint(POINT_ILLIDAN_MIDDLE, IllidanMiddlePoint);
                    break;
                case ACTION_START_PHASE_4:
                    events.CancelEventGroup(GROUP_PHASE_3);
                    DoCastSelf(SPELL_SHADOW_PRISON, true);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    events.ScheduleEvent(EVENT_SHADOW_PRISON_TEXT, Milliseconds(500), GROUP_PHASE_ALL);
                    break;
                case ACTION_ILLIDAN_CAGED:
                    for (uint32 summonSpell : SummonCageTrapSpells)
                        DoCastSelf(summonSpell, true);
                    DoCastSelf(SPELL_CAGE_TRAP, true);
                    break;
                case ACTION_START_OUTRO:
                    me->AttackStop();
                    events.Reset();
                    specialEvents.Reset();
                    DoCastSelf(SPELL_DEATH, true);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    events.ScheduleEvent(EVENT_DEFEATED_TEXT, Seconds(4));
                    break;
                default:
                    break;
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            instance->SetBossState(DATA_ILLIDAN_STORMRAGE, DONE);
            events.Reset();
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE)
                return;

            switch (pointId)
            {
                case POINT_THROW_GLAIVE:
                    DoPlaySoundToSet(me, ILLIDAN_WARGLAIVE_SOUND_ID);
                    events.ScheduleEvent(EVENT_THROW_WARGLAIVE, Seconds(2), GROUP_PHASE_ALL);
                    events.ScheduleEvent(EVENT_FACE_MIDDLE, Milliseconds(1), GROUP_PHASE_ALL);
                    break;
                case POINT_RANDOM_PILLAR:
                {
                    float orientation = IllidanPhase2Positions[_pillarIndex].GetOrientation();
                    ChangeOrientation(orientation);
                    ScheduleEvents(GROUP_PHASE_2, GROUP_PHASE_2);
                    break;
                }
                case POINT_ILLIDAN_MIDDLE:
                {
                    float orientation = IllidanMiddlePoint.GetOrientation();
                    ChangeOrientation(orientation);

                    std::list<Creature*> triggers;
                    GetCreatureListWithEntryInGrid(triggers, me, NPC_BLADE_OF_AZZINOTH, 150.0f);
                    for (Creature* trigger : triggers)
                        trigger->CastSpell(trigger, SPELL_GLAIVE_RETURNS, true);

                    events.ScheduleEvent(EVENT_GLAIVE_EMOTE, Seconds(3), GROUP_PHASE_ALL);
                    break;
                }
                default:
                    break;
            }
        }

        void EnterEvadeModeIfNeeded()
        {
            Map::PlayerList const &players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                if (Player* player = i->GetSource())
                    if (player->IsAlive() && !player->IsGameMaster() && CheckBoundary(player))
                        return;

            EnterEvadeMode(EVADE_REASON_NO_HOSTILES);
        }

        void SummonMinions()
        {
            uint8 needSummon = MAX_MINIONS_NUMBER - _minionsCount;
            for (uint8 i = 0; i < needSummon; ++i)
            {
                _minionsCount++;
                me->SummonCreature(NPC_ILLIDARI_ELITE, MinionsSpawnPositions[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 6000);
            }
        }

        void DamageTaken(Unit* who, uint32 &damage) override
        {

            if (damage >= me->GetHealth() && who->GetGUID() != me->GetGUID())
            {
                damage = me->GetHealth() - 1;
                if (!_dead)
                {
                    if (_isDemon)
                    {
                        events.Reset();
                        specialEvents.Reset();
                        DoCastSelf(SPELL_DEMON_TRANSFORM_1);
                        return;
                    }
                    _dead = true;
                    DoAction(ACTION_START_OUTRO);
                    if (Creature* maiev = instance->GetCreature(DATA_MAIEV))
                        maiev->AI()->DoAction(ACTION_START_OUTRO);
                }
            }
            else if (me->HealthBelowPct(90) && _phase < PHASE_MINIONS)
            {
                _phase = PHASE_MINIONS;
                DoAction(ACTION_START_MINIONS);
            }
            else if (me->HealthBelowPct(65) && _phase < PHASE_2)
            {
                _phase = PHASE_2;
                DoAction(ACTION_START_PHASE_2);
            }
            else if (me->HealthBelowPct(30) && _phase < PHASE_4)
            {
                _phase = PHASE_4;

                if (_isDemon)
                {
                    _isDemon = false;
                    me->SetControlled(false, UNIT_STATE_ROOT);
                    events.CancelEventGroup(GROUP_PHASE_DEMON);
                    me->InterruptNonMeleeSpells(false);
                    DoCastSelf(SPELL_DEMON_TRANSFORM_1, true);
                    events.ScheduleEvent(EVENT_PHASE_4_DELAYED, Seconds(12), GROUP_PHASE_ALL);
                }
                else
                    DoAction(ACTION_START_PHASE_4);
            }
        }

        void ExecuteSpecialEvents()
        {
            while (uint32 eventId = specialEvents.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BERSERK:
                        Talk(SAY_ILLIDAN_ENRAGE);
                        DoCastSelf(SPELL_BERSERK, true);
                        break;
                    case EVENT_CANCEL_DEMON_FORM:
                        me->InterruptNonMeleeSpells(false);
                        me->SetControlled(false, UNIT_STATE_ROOT);
                        events.CancelEventGroup(GROUP_PHASE_DEMON);
                        DoCastSelf(SPELL_DEMON_TRANSFORM_1, true);
                        events.ScheduleEvent(EVENT_RESUME_COMBAT_DEMON, Seconds(12), GROUP_PHASE_ALL);
                        _isDemon = false;
                        break;
                    case EVENT_EVADE_CHECK:
                        EnterEvadeModeIfNeeded();
                        specialEvents.Repeat(Seconds(10));
                        break;
                    default:
                        break;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() && !events.IsInPhase(PHASE_INTRO))
                return;

            specialEvents.Update(diff);

            ExecuteSpecialEvents();

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_INTRO:
                        Talk(SAY_ILLIDAN_DUPLICITY);
                        break;
                    case EVENT_UNCONVINCED:
                        Talk(SAY_ILLIDAN_UNCONVINCED);
                        events.ScheduleEvent(EVENT_PREPARED, Seconds(14), GROUP_PHASE_ALL);
                        break;
                    case EVENT_PREPARED:
                        Talk(SAY_ILLIDAN_PREPARED);
                        me->SetSheath(SHEATH_STATE_MELEE);
                        events.ScheduleEvent(EVENT_ENCOUNTER_START, Seconds(3), GROUP_PHASE_ALL);
                        break;
                    case EVENT_ENCOUNTER_START:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                        DoZoneInCombat();
                        if (Creature* akama = instance->GetCreature(DATA_AKAMA))
                            akama->AI()->DoAction(ACTION_START_ENCOUNTER);
                        break;
                    case EVENT_FLAME_CRASH:
                        DoCastVictim(SPELL_FLAME_CRASH);
                        events.Repeat(Seconds(30));
                        break;
                    case EVENT_DRAW_SOUL:
                        DoCastAOE(SPELL_DRAW_SOUL);
                        events.Repeat(Seconds(34));
                        break;
                    case EVENT_SHEAR:
                        DoCastVictim(SPELL_SHEAR);
                        events.Repeat(Seconds(12));
                        break;
                    case EVENT_PARASITIC_SHADOWFIEND:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                            DoCast(target, SPELL_PARASITIC_SHADOWFIEND);
                        events.Repeat(Seconds(30));
                        break;
                    case EVENT_MINIONS_WEAVE:
                        SummonMinions();
                        events.Repeat(Seconds(30));
                        break;
                    case EVENT_MOVE_TO_WARGLAIVE_POINT:
                    {
                        Position pos;
                        std::list<Creature*> triggers;
                        GetCreatureListWithEntryInGrid(triggers, me, NPC_GLAIVE_WORLD_TRIGGER, 150.0f);
                        triggers.remove_if([](WorldObject* unit)
                        {
                            return unit->GetPositionZ() < 355.0f || unit->GetPositionZ() > 365.0f;
                        });

                        if (triggers.empty())
                            break;

                        triggers.sort(Trinity::ObjectDistanceOrderPred(me));
                        pos.Relocate(triggers.front());
                        pos.SetOrientation(0.0f);
                        me->GetMotionMaster()->MovePoint(POINT_THROW_GLAIVE, pos);
                        if (GameObject* musicController = instance->GetGameObject(DATA_ILLIDAN_MUSIC_CONTROLLER))
                            musicController->PlayDirectMusic(EVENT_BT_STORM_WALK_HERO_2_SOUND_ID);
                        break;
                    }
                    case EVENT_THROW_WARGLAIVE:
                        DoCastAOE(SPELL_THROW_GLAIVE);
                        events.ScheduleEvent(EVENT_THROW_WARGLAIVE_2, Seconds(1), GROUP_PHASE_ALL);
                        break;
                    case EVENT_THROW_WARGLAIVE_2:
                        DoCastAOE(SPELL_THROW_GLAIVE2);
                        me->SetSheath(SHEATH_STATE_UNARMED);
                        events.ScheduleEvent(EVENT_FLY_TO_RANDOM_PILLAR, Seconds(2), GROUP_PHASE_ALL);
                        break;
                    case EVENT_CHANGE_ORIENTATION:
                        me->SetFacingTo(_orientation);
                        break;
                    case EVENT_FLY:
                        ChangeOrientation(3.137039f);
                        events.ScheduleEvent(EVENT_MOVE_TO_WARGLAIVE_POINT, Seconds(6), GROUP_PHASE_ALL);
                        break;
                    case EVENT_FLY_TO_RANDOM_PILLAR:
                    {
                        events.CancelEventGroup(GROUP_PHASE_2);
                        _pillarIndex = urand(0, 3);
                        me->GetMotionMaster()->MovePoint(POINT_RANDOM_PILLAR, IllidanPhase2Positions[_pillarIndex]);
                        events.Repeat(Seconds(30));
                        break;
                    }
                    case EVENT_FACE_MIDDLE:
                    {
                        float angle = me->GetAngle(IllidanMiddlePoint);
                        me->SetFacingTo(angle);
                        break;
                    }
                    case EVENT_EYE_BLAST:
                    {
                        events.CancelEvent(EVENT_DARK_BARRAGE);
                        Position pos = IllidanDBTargetSpawnPositions[_pillarIndex];
                        if (TempSummon* dbTarget = me->SummonCreature(NPC_ILLIDAN_DB_TARGET, pos, TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            Talk(SAY_ILLIDAN_EYE_BLAST);
                            DoCast(dbTarget, SPELL_EYE_BLAST);
                            dbTarget->GetMotionMaster()->MovePoint(POINT_DB_TARGET, IllidanDBTargetPoints[_pillarIndex]);
                        }
                        break;
                    }
                    case EVENT_DARK_BARRAGE:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                            DoCast(target, SPELL_DARK_BARRAGE);
                        events.RescheduleEvent(EVENT_EYE_BLAST, Seconds(5), GROUP_PHASE_2);
                        uint32 currentTime = events.GetNextEventTime(EVENT_FLY_TO_RANDOM_PILLAR);
                        events.RescheduleEvent(EVENT_FLY_TO_RANDOM_PILLAR, Seconds(currentTime) + Seconds(30), GROUP_PHASE_2);
                        break;
                    }
                    case EVENT_FIREBALL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                            DoCast(target, SPELL_FIREBALL);
                        events.Repeat(Seconds(2), Seconds(4));
                        break;
                    case EVENT_GLAIVE_EMOTE:
                        me->SetDisableGravity(false);
                        me->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                        me->SetSheath(SHEATH_STATE_MELEE);
                        events.ScheduleEvent(EVENT_RESUME_COMBAT, Seconds(3), GROUP_PHASE_ALL);
                        break;
                    case EVENT_RESUME_COMBAT:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->SetReactState(REACT_AGGRESSIVE);
                        ScheduleEvents(GROUP_PHASE_3, GROUP_PHASE_3);
                        if (GameObject* musicController = instance->GetGameObject(DATA_ILLIDAN_MUSIC_CONTROLLER))
                            musicController->PlayDirectMusic(EVENT_BT_STORM_WALK_UNI_3_SOUND_ID);
                        break;
                    case EVENT_AGONIZING_FLAMES:
                        DoCastSelf(SPELL_AGONIZING_FLAMES_SELECTOR);
                        events.Repeat(Seconds(53));
                        break;
                    case EVENT_DEMON:
                        me->SetControlled(true, UNIT_STATE_ROOT);
                        _isDemon = true;
                        events.CancelEventGroup(_phase == PHASE_3 ? GROUP_PHASE_3 : GROUP_PHASE_4);
                        me->LoadEquipment(0, true);
                        DoCastSelf(SPELL_DEMON_TRANSFORM_1, true);
                        events.ScheduleEvent(EVENT_DEMON_TEXT, Seconds(2), GROUP_PHASE_ALL);
                        specialEvents.ScheduleEvent(EVENT_CANCEL_DEMON_FORM, Minutes(1) + Seconds(12));
                        events.ScheduleEvent(EVENT_SCHEDULE_DEMON_SPELLS, Seconds(15));
                        break;
                    case EVENT_SCHEDULE_DEMON_SPELLS:
                        DoResetThreat();
                        ScheduleEvents(GROUP_PHASE_DEMON, GROUP_PHASE_DEMON);
                        break;
                    case EVENT_DEMON_TEXT:
                        Talk(SAY_ILLIDAN_MORPH);
                        break;
                    case EVENT_RESUME_COMBAT_DEMON:
                    {
                        uint8 group = _phase == PHASE_3 ? GROUP_PHASE_3 : GROUP_PHASE_4;
                        DoResetThreat();
                        ScheduleEvents(group, group);
                        me->LoadEquipment(1, true);
                        break;
                    }
                    case EVENT_FLAME_BURST:
                        DoCastSelf(SPELL_FLAME_BURST);
                        events.Repeat(Seconds(22));
                        break;
                    case EVENT_SHADOW_DEMON:
                        DoCastSelf(SPELL_SUMMON_SHADOWDEMON);
                        break;
                    case EVENT_SHADOW_BLAST:
                        DoCastVictim(SPELL_SHADOW_BLAST);
                        events.Repeat(Seconds(2));
                        break;
                    case EVENT_PHASE_4_DELAYED:
                        DoAction(ACTION_START_PHASE_4);
                        break;
                    case EVENT_SHADOW_PRISON_TEXT:
                        Talk(SAY_ILLIDAN_SHADOW_PRISON);
                        events.ScheduleEvent(EVENT_SUMMON_MAIEV, Seconds(9), GROUP_PHASE_ALL);
                        break;
                    case EVENT_SUMMON_MAIEV:
                        DoCastSelf(SPELL_SUMMON_MAIEV);
                        if (Creature* maiev = instance->GetCreature(DATA_MAIEV))
                            me->SetFacingToObject(maiev);
                        events.ScheduleEvent(EVENT_CONFRONT_MAIEV_TEXT, Seconds(9), GROUP_PHASE_ALL);
                        break;
                    case EVENT_CONFRONT_MAIEV_TEXT:
                        Talk(SAY_ILLIDAN_CONFRONT_MAIEV);
                        events.ScheduleEvent(EVENT_RESUME_COMBAT_PHASE_4, Seconds(13), GROUP_PHASE_ALL);
                        break;
                    case EVENT_RESUME_COMBAT_PHASE_4:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->SetReactState(REACT_AGGRESSIVE);
                        ScheduleEvents(GROUP_PHASE_4, GROUP_PHASE_4);
                        break;
                    case EVENT_FRENZY:
                        DoCastSelf(SPELL_FRENZY);
                        Talk(SAY_ILLIDAN_FRENZY);
                        events.Repeat(Seconds(40));
                        break;
                    case EVENT_TAUNT:
                        Talk(SAY_ILLIDAN_TAUNT);
                        events.Repeat(Seconds(30), Seconds(60));
                        break;
                    case EVENT_DEFEATED_TEXT:
                        Talk(SAY_ILLIDAN_DEFEATED);
                        if (GameObject* musicController = instance->GetGameObject(DATA_ILLIDAN_MUSIC_CONTROLLER))
                            musicController->PlayDirectMusic(EVENT_BT_ARRIVAL_WALK_HERO_1_SOUND_ID);
                        events.ScheduleEvent(EVENT_QUIET_SUICIDE, Seconds(18));
                        break;
                    case EVENT_QUIET_SUICIDE:
                    {
                        DoCastSelf(SPELL_QUIET_SUICIDE, true);
                        if (Creature* akama = instance->GetCreature(DATA_AKAMA))
                            akama->AI()->DoAction(ACTION_START_OUTRO);
                        ObjectGuid _akamaGUID = instance->GetGuidData(DATA_AKAMA);
                        ObjectGuid _maievGUID = instance->GetGuidData(DATA_MAIEV);
                        summons.DespawnIf([_akamaGUID, _maievGUID](ObjectGuid unitGuid)
                        {
                            return unitGuid != _akamaGUID && unitGuid != _maievGUID;
                        });
                        break;
                    }
                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }

    private:
        bool _intro;
        uint8 _minionsCount;
        uint8 _flameCount;
        float _orientation;
        uint8 _pillarIndex;
        uint8 _phase;
        bool _dead;
        bool _isDemon;
        EventMap specialEvents;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<boss_illidan_stormrageAI>(creature);
    }
};

class npc_akama : public CreatureScript
{
public:
    npc_akama() : CreatureScript("npc_akama_illidan") { }

    struct npc_akamaAI : public ScriptedAI
    {
        npc_akamaAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()), _orientation(0.0f), _isTeleportToMinions(false) { }

        void Reset() override
        {
            _events.Reset();
            _spiritOfUdaloGUID.Clear();
            _spiritOfOlumGUID.Clear();
            _isTeleportToMinions = false;
        }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            if (gossipListId == GOSSIP_START_INTRO)
            {
                _instance->SetData(DATA_AKAMA, AKAMA_FIGHT);
                me->GetMotionMaster()->MoveAlongSplineChain(POINT_STAIRS, SPLINE_STAIRS, false);
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                    illidan->AI()->DoAction(ACTION_INTRO_DONE);
                CloseGossipMenuFor(player);

            }
            else if (gossipListId == GOSSIP_START_FIGHT)
            {
                _events.SetPhase(PHASE_INTRO);
                me->GetMotionMaster()->MoveAlongSplineChain(POINT_FACE_ILLIDAN, SPLINE_FACE_ILLIDAN, false);
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                CloseGossipMenuFor(player);
            }
            return false;
        }

        bool CanAIAttack(Unit const* who) const override
        {
            if (_events.IsInPhase(PHASE_MINIONS) && who->GetEntry() == NPC_ILLIDAN_STORMRAGE)
                return false;
            return ScriptedAI::CanAIAttack(who);
        }

        uint32 GetData(uint32 /*data*/) const override
        {
            return _isTeleportToMinions ? 1 : 0;
        }

        void EnterEvadeMode(EvadeReason /*why*/) override { }

        void JustSummoned(Creature* summon) override
        {
            if (summon->GetEntry() == NPC_SPIRIT_OF_UDALO)
            {
                _spiritOfUdaloGUID = summon->GetGUID();
                summon->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
            else if (summon->GetEntry() == NPC_SPIRIT_OF_OLUM)
            {
                _spiritOfOlumGUID = summon->GetGUID();
                summon->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        }

        void DoAction(int32 actionId) override
        {
            switch (actionId)
            {
                case ACTION_ACTIVE_AKAMA_INTRO:
                    _events.SetPhase(PHASE_INTRO);
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    _events.SetPhase(PHASE_INTRO);
                    _events.ScheduleEvent(EVENT_TELEPORT, Seconds(1));
                    _events.ScheduleEvent(EVENT_MOVE_TO_ILLIDARI_ROOM, Seconds(1) + Milliseconds(500));
                    break;
                case ACTION_OPEN_DOOR:
                    _instance->SetData(ACTION_OPEN_DOOR, 0);
                    _events.ScheduleEvent(EVENT_AKAMA_THANKS, Seconds(2));
                    break;
                case ACTION_FREE:
                    _events.ScheduleEvent(EVENT_FREE, Seconds(14));
                    break;
                case ACTION_START_ENCOUNTER:
                    DoZoneInCombat();
                    _events.ScheduleEvent(EVENT_HEALING_POTION, Seconds(1));
                    break;
                case ACTION_START_MINIONS:
                    _events.ScheduleEvent(EVENT_AKAMA_MINIONS, Seconds(8));
                    break;
                case ACTION_START_OUTRO:
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    _events.Reset();
                    _events.ScheduleEvent(EVENT_AKAMA_MOVE_BACK, Seconds(2));
                    break;
                default:
                    break;
            }
        }

        void ChangeOrientation(float orientation)
        {
            _orientation = orientation;
            _events.ScheduleEvent(EVENT_CHANGE_ORIENTATION, Milliseconds(1));
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type != POINT_MOTION_TYPE && type != SPLINE_CHAIN_MOTION_TYPE)
                return;

            switch (pointId)
            {
                case POINT_ILLIDARI_COUNCIL:
                    Talk(SAY_AKAMA_FINISH);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    break;
                case POINT_STAIRS:
                    ChangeOrientation(6.265732f);
                    _events.ScheduleEvent(EVENT_AKAMA_SAY_DOOR, Seconds(5));
                    break;
                case POINT_ILLIDAN_ROOM:
                    ChangeOrientation(2.129302f);
                    Talk(SAY_AKAMA_BETRAYER);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    break;
                case POINT_FACE_ILLIDAN:
                    ChangeOrientation(3.140537f);
                    _events.ScheduleEvent(EVENT_START_ILLIDAN, Seconds(2));
                    break;
                case POINT_TELEPORT:
                    DoCastSelf(SPELL_AKAMA_TELEPORT);
                    _events.ScheduleEvent(EVENT_AKAMA_MINIONS_MOVE_2, Milliseconds(500));
                    break;
                case POINT_MINIONS:
                    _events.SetPhase(PHASE_MINIONS);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    me->SetReactState(REACT_AGGRESSIVE);
                    if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                        illidan->AI()->DoAction(ACTION_START_MINIONS_WEAVE);
                    _events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, Seconds(2));
                    break;
                case POINT_MOVE_BACK:
                    _events.ScheduleEvent(EVENT_AKAMA_MOVE_TO_ILLIDAN, Milliseconds(1));
                    break;
                case POINT_ILLIDAN:
                    _events.ScheduleEvent(EVENT_AKAMA_LIGHT_TEXT, Seconds(1));
                    break;
                default:
                    break;
            }
        }

        void DamageTaken(Unit* /*who*/, uint32 &damage) override
        {
            if (damage >= me->GetHealth())
                damage = me->GetHealth() - 1;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() && !_events.IsInPhase(PHASE_INTRO))
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TELEPORT:
                        DoCastSelf(SPELL_AKAMA_TELEPORT, true);
                        break;
                    case EVENT_MOVE_TO_ILLIDARI_ROOM:
                        me->GetMotionMaster()->MoveAlongSplineChain(POINT_ILLIDARI_COUNCIL, SPLINE_ILLIDARI_COUNCIL, false);
                        break;
                    case EVENT_AKAMA_SAY_DOOR:
                        Talk(SAY_AKAMA_DOOR);
                        _events.ScheduleEvent(EVENT_AKAMA_DOOR_FAIL, Seconds(4));
                        break;
                    case EVENT_AKAMA_DOOR_FAIL:
                        DoCastSelf(SPELL_AKAMA_DOOR_FAIL);
                        _events.ScheduleEvent(EVENT_AKAMA_SAY_ALONE, Seconds(10));
                        break;
                    case EVENT_AKAMA_SAY_ALONE:
                        Talk(SAY_AKAMA_ALONE);
                        _events.ScheduleEvent(EVENT_SUMMON_SPIRITS, Seconds(7));
                        break;
                    case EVENT_SUMMON_SPIRITS:
                        me->SummonCreatureGroup(SUMMON_GROUP);
                        _events.ScheduleEvent(EVENT_SPIRIT_SAY_1, Seconds(1));
                        break;
                    case EVENT_SPIRIT_SAY_1:
                        if (Creature* undalo = ObjectAccessor::GetCreature(*me, _spiritOfUdaloGUID))
                            undalo->AI()->Talk(SAY_SPIRIT_ALONE);
                        _events.ScheduleEvent(EVENT_SPIRIT_SAY_2, Seconds(6));
                        break;
                    case EVENT_SPIRIT_SAY_2:
                        if (Creature* olum = ObjectAccessor::GetCreature(*me, _spiritOfOlumGUID))
                            olum->AI()->Talk(SAY_SPIRIT_ALONE);
                        _events.ScheduleEvent(EVENT_AKAMA_DOOR_SUCCESS, Seconds(6));
                        break;
                    case EVENT_AKAMA_DOOR_SUCCESS:
                        DoCastSelf(SPELL_AKAMA_DOOR_CHANNEL);
                        if (Creature* undalo = ObjectAccessor::GetCreature(*me, _spiritOfUdaloGUID))
                            undalo->CastSpell((Unit*) nullptr, SPELL_DEATHSWORN_DOOR_CHANNEL);
                        if (Creature* olum = ObjectAccessor::GetCreature(*me, _spiritOfOlumGUID))
                            olum->CastSpell((Unit*) nullptr, SPELL_DEATHSWORN_DOOR_CHANNEL);
                        _events.ScheduleEvent(EVENT_AKAMA_START_SOUND, Seconds(5));
                        break;
                    case EVENT_AKAMA_START_SOUND:
                        if (GameObject* musicController = _instance->GetGameObject(DATA_ILLIDAN_MUSIC_CONTROLLER))
                            musicController->PlayDirectMusic(EVENT_BT_SUMMIT_WALK_SOUND_ID);
                        break;
                    case EVENT_AKAMA_THANKS:
                        Talk(SAY_AKAMA_SALUTE);
                        _events.ScheduleEvent(EVENT_SPIRIT_SALUTE, Seconds(3));
                        _events.ScheduleEvent(EVENT_RUN_FROM_ILLIDAN_ROOM, Seconds(7));
                        break;
                    case EVENT_SPIRIT_SALUTE:
                        if (Creature* undalo = ObjectAccessor::GetCreature(*me, _spiritOfUdaloGUID))
                        {
                            undalo->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                            undalo->DespawnOrUnsummon(Seconds(7));
                        }
                        if (Creature* olum = ObjectAccessor::GetCreature(*me, _spiritOfOlumGUID))
                        {
                            olum->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                            olum->DespawnOrUnsummon(Seconds(7));
                        }
                        break;
                    case EVENT_RUN_FROM_ILLIDAN_ROOM:
                        me->GetMotionMaster()->MoveAlongSplineChain(POINT_ILLIDAN_ROOM, SPLINE_ILLIDAN_ROOM, false);
                        break;
                    case EVENT_START_ILLIDAN:
                        if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                            illidan->AI()->DoAction(ACTION_START_ENCOUNTER);
                        break;
                    case EVENT_FREE:
                        Talk(SAY_AKAMA_FREE);
                        _events.ScheduleEvent(EVENT_TIME_HAS_COME, Seconds(18));
                        break;
                    case EVENT_TIME_HAS_COME:
                        Talk(SAY_AKAMA_TIME_HAS_COME);
                        _events.ScheduleEvent(EVENT_ROAR, Seconds(2));
                        break;
                    case EVENT_ROAR:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                        break;
                    case EVENT_CHANGE_ORIENTATION:
                        me->SetFacingTo(_orientation);
                        break;
                    case EVENT_HEALING_POTION:
                        if (me->HealthBelowPct(20))
                            DoCastSelf(SPELL_HEALING_POTION);
                        _events.Repeat(Seconds(1));
                        break;
                    case EVENT_AKAMA_MINIONS:
                        Talk(SAY_AKAMA_MINIONS);
                        _events.ScheduleEvent(EVENT_AKAMA_MINIONS_EMOTE, Seconds(2));
                        break;
                    case EVENT_AKAMA_MINIONS_EMOTE:
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                        _events.ScheduleEvent(EVENT_AKAMA_MINIONS_MOVE, Seconds(4));
                        break;
                    case EVENT_AKAMA_MINIONS_MOVE:
                        _isTeleportToMinions = true;
                        me->GetMotionMaster()->MoveAlongSplineChain(POINT_TELEPORT, SPLINE_TELEPORT, false);
                        break;
                    case EVENT_AKAMA_MINIONS_MOVE_2:
                        me->GetMotionMaster()->MoveAlongSplineChain(POINT_MINIONS, SPLINE_MINIONS, false);
                        break;
                    case EVENT_CHAIN_LIGHTNING:
                        DoCastVictim(SPELL_CHAIN_LIGHTNING);
                        _events.Repeat(Seconds(8) + Milliseconds(500));
                        break;
                    case EVENT_AKAMA_MOVE_BACK:
                        me->GetMotionMaster()->MoveAlongSplineChain(POINT_MOVE_BACK, SPLINE_MOVE_BACK, false);
                        break;
                    case EVENT_AKAMA_MOVE_TO_ILLIDAN:
                        if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                            me->GetMotionMaster()->MoveCloserAndStop(POINT_ILLIDAN, illidan, 5.0f);
                        break;
                    case EVENT_AKAMA_LIGHT_TEXT:
                        Talk(SAY_AKAMA_LIGHT);
                        _events.ScheduleEvent(EVENT_FINAL_SALUTE, Seconds(4));
                        break;
                    case EVENT_FINAL_SALUTE:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                        _events.ScheduleEvent(EVENT_AKAMA_DESPAWN, Seconds(5));
                        break;
                    case EVENT_AKAMA_DESPAWN:
                        DoCastSelf(SPELL_AKAMA_DESPAWN, true);
                        break;
                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* _instance;
        EventMap _events;
        ObjectGuid _spiritOfUdaloGUID;
        ObjectGuid _spiritOfOlumGUID;
        float _orientation;
        bool _isTeleportToMinions;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<npc_akamaAI>(creature);
    }
};

class npc_parasitic_shadowfiend : public CreatureScript
{
public:
    npc_parasitic_shadowfiend() : CreatureScript("npc_parasitic_shadowfiend") { }

    struct npc_parasitic_shadowfiendAI : public ScriptedAI
    {
        npc_parasitic_shadowfiendAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()) { }

        void Reset() override
        {
            if (_instance->GetBossState(DATA_ILLIDAN_STORMRAGE) != IN_PROGRESS)
            {
                me->DespawnOrUnsummon();
                return;
            }

            if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                illidan->AI()->JustSummoned(me);
            me->SetReactState(REACT_DEFENSIVE);
            _scheduler.Schedule(Seconds(2), [this](TaskContext /*context*/)
            {
                me->SetReactState(REACT_AGGRESSIVE);
                me->SetInCombatWithZone();
            });
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            _scheduler.Update(diff, [this]
            {
                DoMeleeAttackIfReady();
            });
        }

    private:
        InstanceScript* _instance;
        TaskScheduler _scheduler;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<npc_parasitic_shadowfiendAI>(creature);
    }
};

class npc_blade_of_azzinoth : public CreatureScript
{
public:
    npc_blade_of_azzinoth() : CreatureScript("npc_blade_of_azzinoth") { }

    struct npc_blade_of_azzinothAI : public NullCreatureAI
    {
        npc_blade_of_azzinothAI(Creature* creature) : NullCreatureAI(creature), _instance(creature->GetInstanceScript()) { }

        void Reset() override
        {
            if (_instance->GetBossState(DATA_ILLIDAN_STORMRAGE) != IN_PROGRESS)
            {
                me->DespawnOrUnsummon();
                return;
            }

            if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                illidan->AI()->JustSummoned(me);
            _flameGuid.Clear();
            me->PlayDirectSound(WARGLAIVE_SPAWN_SOUND_ID);
            DoCastSelf(SPELL_BIRTH, true);
            _scheduler.Schedule(Seconds(3), [this](TaskContext /*context*/)
            {
                DoCastSelf(SPELL_SUMMON_TEAR_OF_AZZINOTH);
                _scheduler.Schedule(Milliseconds(500), [this](TaskContext /*context*/)
                {
                    if (Creature* flame = ObjectAccessor::GetCreature(*me, _flameGuid))
                        DoCast(flame, SPELL_AZZINOTH_CHANNEL);
                });
            });
        }

        void JustSummoned(Creature* summon) override
        {
            if (summon->GetEntry() == NPC_FLAME_OF_AZZINOTH)
                _flameGuid = summon->GetGUID();
        }

        void UpdateAI(uint32 diff) override
        {
            _scheduler.Update(diff);
        }

    private:
        InstanceScript* _instance;
        TaskScheduler _scheduler;
        ObjectGuid _flameGuid;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<npc_blade_of_azzinothAI>(creature);
    }
};

class npc_flame_of_azzinoth : public CreatureScript
{
public:
    npc_flame_of_azzinoth() : CreatureScript("npc_flame_of_azzinoth") { }

    struct npc_flame_of_azzinothAI : public ScriptedAI
    {
        npc_flame_of_azzinothAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
        {
            SetBoundary(_instance->GetBossBoundary(DATA_ILLIDAN_STORMRAGE));
        }

        void Reset() override
        {
            if (_instance->GetBossState(DATA_ILLIDAN_STORMRAGE) != IN_PROGRESS)
            {
                me->DespawnOrUnsummon();
                return;
            }

            if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                illidan->AI()->JustSummoned(me);
            DoCastSelf(SPELL_FLAME_TEAR_OF_AZZINOTH, true); // Idk what this spell should do
            me->SetReactState(REACT_PASSIVE);
            _events.ScheduleEvent(EVENT_ENGAGE, Seconds(3));
            _events.ScheduleEvent(EVENT_FLAME_BLAST, Seconds(11));
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ENGAGE:
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->SetInCombatWithZone();
                        _events.ScheduleEvent(EVENT_FLAME_CHARGE, Seconds(5));
                        break;
                    case EVENT_FLAME_CHARGE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, ChargeTargetSelector(me)))
                        {
                            DoCast(target, SPELL_CHARGE);
                            _events.Repeat(Seconds(5));
                        }
                        else
                            _events.Repeat(Seconds(1));
                        break;
                    case EVENT_FLAME_BLAST:
                        DoCastAOE(SPELL_FLAME_BLAST);
                        _events.Repeat(Seconds(12));
                        break;
                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                illidan->AI()->DoAction(ACTION_FLAME_DEAD);
        }

    private:
        InstanceScript* _instance;
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<npc_flame_of_azzinothAI>(creature);
    }
};

class npc_illidan_db_target : public CreatureScript
{
public:
    npc_illidan_db_target() : CreatureScript("npc_illidan_db_target") { }

    struct npc_illidan_db_targetAI : public NullCreatureAI
    {
        npc_illidan_db_targetAI(Creature* creature) : NullCreatureAI(creature) { }

        void Reset() override
        {
            DoCastSelf(SPELL_EYE_BLAST_TRIGGER);
        }

        void JustSummoned(Creature* summon) override
        {
            if (summon->GetEntry() == NPC_DEMON_FIRE)
                summon->SetReactState(REACT_PASSIVE);
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == POINT_DB_TARGET)
            {
                me->RemoveAurasDueToSpell(SPELL_EYE_BLAST_TRIGGER);
                me->RemoveAurasDueToSpell(SPELL_EYE_BLAST);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<npc_illidan_db_targetAI>(creature);
    }
};

class npc_illidan_shadow_demon : public CreatureScript
{
public:
    npc_illidan_shadow_demon() : CreatureScript("npc_shadow_demon") { }

    struct npc_illidan_shadow_demonAI : public PassiveAI
    {
        npc_illidan_shadow_demonAI(Creature* creature) : PassiveAI(creature), _instance(creature->GetInstanceScript()) { }

        void Reset() override
        {
            if (_instance->GetBossState(DATA_ILLIDAN_STORMRAGE) != IN_PROGRESS)
            {
                me->DespawnOrUnsummon();
                return;
            }

            DoCastSelf(SPELL_SHADOW_DEMON_PASSIVE);
            DoCastSelf(SPELL_FIND_TARGET);
            _scheduler.Schedule(Seconds(1), [this](TaskContext checkTarget)
            {
                if (Unit* target = ObjectAccessor::GetUnit(*me, _targetGUID))
                {
                    if (!target->IsAlive())
                        DoCastSelf(SPELL_FIND_TARGET);
                    else if (me->IsWithinMeleeRange(target))
                    {
                        me->InterruptNonMeleeSpells(false);
                        DoCast(target, SPELL_CONSUME_SOUL, true);
                    }
                }
                checkTarget.Repeat();
            });
        }

        void SetGUID(ObjectGuid guid, int32 /*id*/) override
        {
            _targetGUID = guid;
            if (Unit* target = ObjectAccessor::GetUnit(*me, _targetGUID))
                me->GetMotionMaster()->MoveChase(target);
        }

        void UpdateAI(uint32 diff) override
        {
            _scheduler.Update(diff);
        }

    private:
        InstanceScript* _instance;
        TaskScheduler _scheduler;
        ObjectGuid _targetGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<npc_illidan_shadow_demonAI>(creature);
    }
};

class npc_maiev : public CreatureScript
{
public:
    npc_maiev() : CreatureScript("npc_maiev") { }

    struct npc_maievAI : public ScriptedAI
    {
        npc_maievAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()), _canDown(true) { }

        void Reset() override
        {
            if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                me->SetFacingToObject(illidan);
            me->SetReactState(REACT_PASSIVE);
            _events.SetPhase(PHASE_INTRO);
            _events.ScheduleEvent(EVENT_MAIEV_APPEAR, Seconds(1));
            _events.ScheduleEvent(EVENT_MAIEV_EXCLAMATION, Seconds(2));
            _events.ScheduleEvent(EVENT_MAIEV_JUSTICE_TEXT, Seconds(14));
            _events.ScheduleEvent(EVENT_TAUNT, Seconds(20), Seconds(60));
            _canDown = true;
        }

        void EnterCombat(Unit* /*who*/) override
        {
            _events.SetPhase(PHASE_1);
            _events.ScheduleEvent(EVENT_CAGE_TRAP, Seconds(30));
            _events.ScheduleEvent(EVENT_SHADOW_STRIKE, Seconds(50));
            _events.ScheduleEvent(EVENT_THROW_DAGGER, Seconds(1));
        }

        void DoAction(int32 actionId) override
        {
            if (actionId == ACTION_START_OUTRO)
            {
                _events.Reset();
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                    me->SetFacingToObject(illidan);
                Talk(SAY_MAIEV_SHADOWSONG_FINISHED);
                _events.ScheduleEvent(EVENT_MAIEV_OUTRO_TEXT, Seconds(28));
            }
            else if (actionId == ACTION_MAIEV_DOWN_FADE)
                _canDown = true;
        }

        void DamageTaken(Unit* /*who*/, uint32 &damage) override
        {
            if (damage >= me->GetHealth() && _canDown)
            {
                damage = me->GetHealth() - 1;
                _canDown = false;
                DoCastSelf(SPELL_MAIEV_DOWN, true);
                Talk(SAY_MAIEV_SHADOWSONG_DOWN, me);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() && !_events.IsInPhase(PHASE_INTRO))
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MAIEV_APPEAR:
                        Talk(SAY_MAIEV_SHADOWSONG_APPEAR);
                        break;
                    case EVENT_MAIEV_EXCLAMATION:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                        break;
                    case EVENT_MAIEV_JUSTICE_TEXT:
                        Talk(SAY_MAIEV_SHADOWSONG_JUSTICE);
                        _events.ScheduleEvent(EVENT_MAIEV_YES, Seconds(2));
                        break;
                    case EVENT_MAIEV_YES:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_YES);
                        _events.ScheduleEvent(EVENT_MAIEV_ROAR, Seconds(3));
                        break;
                    case EVENT_MAIEV_ROAR:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
                        _events.ScheduleEvent(EVENT_MAIEV_COMBAT, Seconds(3));
                        break;
                    case EVENT_MAIEV_COMBAT:
                        me->SetReactState(REACT_AGGRESSIVE);
                        if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                            AttackStart(illidan);
                        break;
                    case EVENT_CAGE_TRAP:
                        if (Creature* illidan = _instance->GetCreature(DATA_ILLIDAN_STORMRAGE))
                            illidan->CastSpell(illidan, SPELL_CAGED_TRAP_TELEPORT, true);
                        DoCastSelf(SPELL_CAGE_TRAP_SUMMON);
                        Talk(SAY_MAIEV_SHADOWSONG_TRAP);
                        _events.Repeat(Seconds(30));
                        break;
                    case EVENT_SHADOW_STRIKE:
                        DoCastVictim(SPELL_SHADOW_STRIKE);
                        _events.Repeat(Seconds(50));
                        break;
                    case EVENT_THROW_DAGGER:
                        if (Unit* target = me->GetVictim())
                            if (!me->IsWithinMeleeRange(target))
                            {
                                DoCastVictim(SPELL_THROW_DAGGER);
                                _events.Repeat(Seconds(5));
                                break;
                            }
                        _events.Repeat(Seconds(1));
                        break;
                    case EVENT_TAUNT:
                        Talk(SAY_MAIEV_SHADOWSONG_TAUNT);
                        _events.Repeat(Seconds(30), Seconds(60));
                        break;
                    case EVENT_MAIEV_OUTRO_TEXT:
                        Talk(SAY_MAIEV_SHADOWSONG_OUTRO);
                        _events.ScheduleEvent(EVENT_MAIEV_FAREWELL_TEXT, Seconds(11));
                        break;
                    case EVENT_MAIEV_FAREWELL_TEXT:
                        Talk(SAY_MAIEV_SHADOWSONG_FAREWELL);
                        _events.ScheduleEvent(EVENT_MAIEV_TELEPORT_DESPAWN, Seconds(3));
                        break;
                    case EVENT_MAIEV_TELEPORT_DESPAWN:
                        DoCastSelf(SPELL_TELEPORT_VISUAL);
                        me->DespawnOrUnsummon(Seconds(1));
                        break;
                    default:
                        break;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        InstanceScript* _instance;
        bool _canDown;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<npc_maievAI>(creature);
    }
};

class npc_cage_trap_trigger : public CreatureScript
{
public:
    npc_cage_trap_trigger() : CreatureScript("npc_cage_trap_trigger") { }

    struct npc_cage_trap_triggerAI : public PassiveAI
    {
        npc_cage_trap_triggerAI(Creature* creature) : PassiveAI(creature) { }

        void Reset() override
        {
            _scheduler.Schedule(Seconds(1), [this](TaskContext checkTarget)
            {
                DoCastSelf(SPELL_CAGE_TRAP_PERIODIC);
                checkTarget.Repeat();
            });
        }

        void UpdateAI(uint32 diff) override
        {
            _scheduler.Update(diff);
        }

    private:
        TaskScheduler _scheduler;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetBlackTempleAI<npc_cage_trap_triggerAI>(creature);
    }
};

// 41077 - Akama Teleport
class spell_illidan_akama_teleport : public SpellScriptLoader
{
    public:
        spell_illidan_akama_teleport() : SpellScriptLoader("spell_illidan_akama_teleport") { }

        class spell_illidan_akama_teleport_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_akama_teleport_SpellScript);

            void SetDest(SpellDestination& dest)
            {
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    uint32 destination = caster->AI()->GetData(DATA_AKAMA_TELEPORT_POSITION);
                    dest.Relocate(AkamaTeleportPositions[destination]);
                }
            }

            void Register() override
            {
                OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_illidan_akama_teleport_SpellScript::SetDest, EFFECT_0, TARGET_DEST_NEARBY_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_akama_teleport_SpellScript();
        }
};

// 41268 - Quest - Black Temple - Akama - Door Open
class spell_illidan_akama_door_channel : public SpellScriptLoader
{
    public:
        spell_illidan_akama_door_channel() : SpellScriptLoader("spell_illidan_akama_door_channel") { }

        class spell_illidan_akama_door_channel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_illidan_akama_door_channel_AuraScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ARCANE_EXPLOSION))
                    return false;
                return true;
            }

            void OnRemoveDummy(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->CastSpell(target, SPELL_ARCANE_EXPLOSION, true);

                if (InstanceScript* instance = target->GetInstanceScript())
                    if (Creature* akama = instance->GetCreature(DATA_AKAMA))
                        akama->AI()->DoAction(ACTION_OPEN_DOOR);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_illidan_akama_door_channel_AuraScript::OnRemoveDummy, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_illidan_akama_door_channel_AuraScript();
        }
};

// 40904 - Draw Soul
class spell_illidan_draw_soul : public SpellScriptLoader
{
    public:
        spell_illidan_draw_soul() : SpellScriptLoader("spell_illidan_draw_soul") { }

        class spell_illidan_draw_soul_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_draw_soul_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DRAW_SOUL_HEAL))
                    return false;
                return true;
            }

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->CastSpell(GetCaster(), SPELL_DRAW_SOUL_HEAL, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_illidan_draw_soul_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_draw_soul_SpellScript();
        }
};

/* 41917 - Parasitic Shadowfiend
   41914 - Parasitic Shadowfiend */
class spell_illidan_parasitic_shadowfiend : public SpellScriptLoader
{
    public:
        spell_illidan_parasitic_shadowfiend() : SpellScriptLoader("spell_illidan_parasitic_shadowfiend") { }

        class spell_illidan_parasitic_shadowfiend_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_illidan_parasitic_shadowfiend_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SUMMON_PARASITIC_SHADOWFIENDS))
                    return false;
                return true;
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->CastSpell(target, SPELL_SUMMON_PARASITIC_SHADOWFIENDS, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_illidan_parasitic_shadowfiend_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_illidan_parasitic_shadowfiend_AuraScript();
        }
};

/* 39635 - Throw Glaive
   39849 - Throw Glaive */
class spell_illidan_throw_warglaive : public SpellScriptLoader
{
    public:
        spell_illidan_throw_warglaive() : SpellScriptLoader("spell_illidan_throw_warglaive") { }

        class spell_illidan_throw_warglaive_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_throw_warglaive_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                target->m_Events.AddEvent(new SummonWarglaiveEvent(target), target->m_Events.CalculateTime(1000));
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_illidan_throw_warglaive_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_throw_warglaive_SpellScript();
        }
};

// 39857 - Tear of Azzinoth Summon Channel
class spell_illidan_tear_of_azzinoth_channel : public SpellScriptLoader
{
    public:
        spell_illidan_tear_of_azzinoth_channel() : SpellScriptLoader("spell_illidan_tear_of_azzinoth_channel") { }

        class spell_illidan_tear_of_azzinoth_channel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_illidan_tear_of_azzinoth_channel_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_UNCAGED_WRATH))
                    return false;
                return true;
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                if (Unit* caster = GetCaster())
                {
                    Unit* target = GetTarget();
                    if (caster->GetDistance2d(target) > 25.0f)
                    {
                        target->CastSpell(target, SPELL_UNCAGED_WRATH, true);
                        Remove();
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_illidan_tear_of_azzinoth_channel_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_illidan_tear_of_azzinoth_channel_AuraScript();
        }
};

// 40631 - Flame Blast
class spell_illidan_flame_blast : public SpellScriptLoader
{
    public:
        spell_illidan_flame_blast() : SpellScriptLoader("spell_illidan_flame_blast") { }

        class spell_illidan_flame_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_flame_blast_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_BLAZE_SUMMON))
                    return false;
                return true;
            }

            void HandleBlaze(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                target->CastSpell(target, SPELL_BLAZE_SUMMON, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_illidan_flame_blast_SpellScript::HandleBlaze, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_flame_blast_SpellScript();
        }
};

// 39873 - Glaive Returns
class spell_illidan_return_glaives : public SpellScriptLoader
{
    public: spell_illidan_return_glaives() : SpellScriptLoader("spell_illidan_return_glaives") { }

        class spell_illidan_return_glaives_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_return_glaives_SpellScript);

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                GetHitUnit()->SendPlaySpellVisual(SPELL_GLAIVE_VISUAL_KIT);
                if (Creature* caster = GetCaster()->ToCreature())
                    caster->DespawnOrUnsummon();
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_illidan_return_glaives_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_return_glaives_SpellScript();
        }
};

// 40834 - Agonizing Flames
class spell_illidan_agonizing_flames : public SpellScriptLoader
{
    public:
        spell_illidan_agonizing_flames() : SpellScriptLoader("spell_illidan_agonizing_flames") { }

        class spell_illidan_agonizing_flames_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_agonizing_flames_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AGONIZING_FLAMES))
                    return false;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
                targets.clear();
                targets.push_back(target);
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->CastSpell(GetHitUnit(), SPELL_AGONIZING_FLAMES, true);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_illidan_agonizing_flames_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_illidan_agonizing_flames_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_agonizing_flames_SpellScript();
        }
};

// 40511 - Demon Transform 1
class spell_illidan_demon_transform1 : public SpellScriptLoader
{
    public:
        spell_illidan_demon_transform1() : SpellScriptLoader("spell_illidan_demon_transform1") { }

        class spell_illidan_demon_transform1_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_illidan_demon_transform1_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DEMON_TRANSFORM_2))
                    return false;
                return true;
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(GetTarget(), SPELL_DEMON_TRANSFORM_2, true);
                Remove();
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_illidan_demon_transform1_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_illidan_demon_transform1_AuraScript();
        }
};

// 40398 - Demon Transform 2
class spell_illidan_demon_transform2 : public SpellScriptLoader
{
    public:
        spell_illidan_demon_transform2() : SpellScriptLoader("spell_illidan_demon_transform2") { }

        class spell_illidan_demon_transform2_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_illidan_demon_transform2_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DEMON_FORM)
                    || !sSpellMgr->GetSpellInfo(SPELL_DEMON_TRANSFORM_3))
                    return false;
                return true;
            }

            void OnPeriodic(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                Unit* target = GetTarget();

                if (aurEff->GetTickNumber() == 1)
                {
                    if (target->GetDisplayId() == target->GetNativeDisplayId())
                        target->CastSpell(target, SPELL_DEMON_FORM, true);
                    else
                        target->RemoveAurasDueToSpell(SPELL_DEMON_FORM);
                }
                else if (aurEff->GetTickNumber() == 2)
                {
                    target->CastSpell(target, SPELL_DEMON_TRANSFORM_3, true);
                    if (Aura* aura = GetUnitOwner()->GetAura(SPELL_DEMON_TRANSFORM_3))
                        aura->SetDuration(4300);
                    Remove();
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_illidan_demon_transform2_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_illidan_demon_transform2_AuraScript();
        }
};

// 41126 - Flame Burst
class spell_illidan_flame_burst : public SpellScriptLoader
{
    public:
        spell_illidan_flame_burst() : SpellScriptLoader("spell_illidan_flame_burst") { }

        class spell_illidan_flame_burst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_flame_burst_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FLAME_BURST_EFFECT))
                    return false;
                return true;
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                GetHitUnit()->CastSpell(GetHitUnit(), SPELL_FLAME_BURST_EFFECT, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_illidan_flame_burst_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_flame_burst_SpellScript();
        }
};

// 41081 - Find Target
class spell_illidan_find_target : public SpellScriptLoader
{
    public:
        spell_illidan_find_target() : SpellScriptLoader("spell_illidan_find_target") { }

        class spell_illidan_find_target_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_find_target_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PARALYZE))
                    return false;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(Trinity::UnitAuraCheck(true, SPELL_PARALYZE));

                if (targets.empty())
                    return;

                WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
                targets.clear();
                targets.push_back(target);
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* target = GetHitUnit();
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    caster->CastSpell(target, SPELL_PARALYZE, true);
                    caster->AI()->SetGUID(target->GetGUID(), 0);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_illidan_find_target_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_illidan_find_target_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_find_target_SpellScript();
        }
};

// 39908 - Eye Blast
class spell_illidan_eye_blast : public SpellScriptLoader
{
    public:
        spell_illidan_eye_blast() : SpellScriptLoader("spell_illidan_eye_blast") { }

        class spell_illidan_eye_blast_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_illidan_eye_blast_AuraScript);

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* target = GetTarget()->ToCreature())
                    target->DespawnOrUnsummon();
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_illidan_eye_blast_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_illidan_eye_blast_AuraScript();
        }
};

// 40761 - Cage Trap
class spell_illidan_cage_trap : public SpellScriptLoader
{
    public:
        spell_illidan_cage_trap() : SpellScriptLoader("spell_illidan_cage_trap") { }

        class spell_illidan_cage_trap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_cage_trap_SpellScript);

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                Creature* target = GetHitCreature();
                Creature* caster = GetCaster()->ToCreature();

                if (!target || !caster)
                    return;

                if (caster->GetDistance2d(target) < 4.0f)
                {
                    target->AI()->DoAction(ACTION_ILLIDAN_CAGED);
                    caster->DespawnOrUnsummon();
                    if (GameObject* trap = target->FindNearestGameObject(GO_ILLIDAN_CAGE_TRAP, 10.0f))
                        trap->UseDoorOrButton();
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_illidan_cage_trap_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_cage_trap_SpellScript();
        }
};

// 40760 - Cage Trap
class spell_illidan_caged : public SpellScriptLoader
{
    public:
        spell_illidan_caged() : SpellScriptLoader("spell_illidan_caged") { }

        class spell_illidan_caged_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_illidan_caged_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CAGED_DEBUFF))
                    return false;
                return true;
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                Unit* target = GetTarget();
                target->CastSpell(target, SPELL_CAGED_DEBUFF, true);
                Remove();
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_illidan_caged_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_illidan_caged_AuraScript();
        }
};

// 40409 - Maiev Down
class spell_maiev_down : public SpellScriptLoader
{
public:
    spell_maiev_down() : SpellScriptLoader("spell_maiev_down") { }

    class spell_maiev_down_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_maiev_down_AuraScript);

        bool Load() override
        {
            return GetUnitOwner()->GetTypeId() == TYPEID_UNIT;
        }

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            GetTarget()->GetAI()->DoAction(ACTION_MAIEV_DOWN_FADE);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_maiev_down_AuraScript::HandleEffectApply, EFFECT_1, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_maiev_down_AuraScript::HandleEffectRemove, EFFECT_1, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_maiev_down_AuraScript();
    }
};

//  40693 - Cage Trap
class spell_illidan_cage_teleport : public SpellScriptLoader
{
    public:
        spell_illidan_cage_teleport() : SpellScriptLoader("spell_illidan_cage_teleport") { }

        class spell_illidan_cage_teleport_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_cage_teleport_SpellScript);

            void SetDest(SpellDestination& dest)
            {
                Position offset = { 0.0f, 0.0f, GetCaster()->GetPositionZ(), 0.0f };
                dest.RelocateOffset(offset);
            }

            void Register() override
            {
                OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_illidan_cage_teleport_SpellScript::SetDest, EFFECT_0, TARGET_DEST_CASTER_RADIUS);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_cage_teleport_SpellScript();
        }
};

// 41242 - Akama Despawn
class spell_illidan_despawn_akama : public SpellScriptLoader
{
    public:
        spell_illidan_despawn_akama() : SpellScriptLoader("spell_illidan_despawn_akama") { }

        class spell_illidan_despawn_akama_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_illidan_despawn_akama_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Creature* target = GetHitCreature())
                    target->DespawnOrUnsummon(Seconds(1));
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_illidan_despawn_akama_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_illidan_despawn_akama_SpellScript();
        }
};

void AddSC_boss_illidan()
{
    new boss_illidan_stormrage();
    new npc_akama();
    new npc_parasitic_shadowfiend();
    new npc_blade_of_azzinoth();
    new npc_flame_of_azzinoth();
    new npc_illidan_db_target();
    new npc_maiev();
    new npc_illidan_shadow_demon();
    new npc_cage_trap_trigger();
    new spell_illidan_akama_teleport();
    new spell_illidan_akama_door_channel();
    new spell_illidan_draw_soul();
    new spell_illidan_parasitic_shadowfiend();
    new spell_illidan_throw_warglaive();
    new spell_illidan_tear_of_azzinoth_channel();
    new spell_illidan_flame_blast();
    new spell_illidan_return_glaives();
    new spell_illidan_agonizing_flames();
    new spell_illidan_demon_transform1();
    new spell_illidan_demon_transform2();
    new spell_illidan_flame_burst();
    new spell_illidan_find_target();
    new spell_illidan_eye_blast();
    new spell_illidan_cage_trap();
    new spell_illidan_caged();
    new spell_maiev_down();
    new spell_illidan_cage_teleport();
    new spell_illidan_despawn_akama();
}
