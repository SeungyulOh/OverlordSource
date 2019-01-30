// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//////////////////////////////////////////////////////////////////////////
// AnimInstance 

// Group
#define MONTAGE_GROUP_ACTION				FName(TEXT("ActionGroup"))

// Slot
#define MONTAGE_SLOT_BASIC					FName(TEXT("Basic"))
#define MONTAGE_SLOT_ATTACK					FName(TEXT("Attack"))
#define MONTAGE_SLOT_SKILL					FName(TEXT("Skill"))
#define MONTAGE_SLOT_ABNORMAL				FName(TEXT("Abnormal"))
#define MONTAGE_SLOT_EVENT					FName(TEXT("Event"))
#define MONTAGE_SLOT_DAMAGERESULT			FName(TEXT("DamageResult"))
#define MONTAGE_SLOT_LOBBY					FName(TEXT("Lobby"))

//////////////////////////////////////////////////////////////////////////

#define ANIM_MONTAGE_STATE_FLY				FString(TEXT("Fly"))

#define ANIM_MONTAGE_DEFAULT				FString(TEXT("Default"))
#define ANIM_MONTAGE_AIR					FString(TEXT("Air"))

//////////////////////////////////////////////////////////////////////////
// Basic
#define ANIM_MONTAGE_BASIC_IDLE				FString(TEXT("Idle"))
#define ANIM_MONTAGE_BASIC_RUN				FString(TEXT("Run"))

// Attack
#define ANIM_MONTAGE_ATTACK					FString(TEXT("Attack"))			// 01, 02, 03 ...

// Skill
#define ANIM_MONTAGE_SKILL_CASTING			FString(TEXT("SkillCasting"))		// 01, 02, 03 ...
#define ANIM_MONTAGE_SKILL_ACTION			FString(TEXT("SkillAction"))		// 01, 02, 03 ...
#define ANIM_MONTAGE_SKILL_ACTION_END		FString(TEXT("SkillActionEnd"))		// 01, 02, 03 ...

// Abnormal
#define ANIM_MONTAGE_ABNORMAL_STUN			FName(TEXT("Stun"))
#define ANIM_MONTAGE_ABNORMAL_PUSH			FName(TEXT("Push"))
#define ANIM_MONTAGE_ABNORMAL_KNOCKBACK		FName(TEXT("Knockback"))
#define ANIM_MONTAGE_ABNORMAL_DOWNSTART		FName(TEXT("DownStart"))
#define ANIM_MONTAGE_ABNORMAL_DOWNEND		FName(TEXT("DownEnd"))
#define ANIM_MONTAGE_ABNORMAL_AIRSTART		FName(TEXT("AirStart"))
#define ANIM_MONTAGE_ABNORMAL_AIREND		FName(TEXT("AirEnd"))
#define ANIM_MONTAGE_ABNORMAL_UNACTABLE		FName(TEXT("UnactableState"))

// Event
#define ANIM_MONTAGE_EVENT_BATTLEWIN		FName(TEXT("BattleWin"))
#define ANIM_MONTAGE_EVENT_BATTLEREADY		FName(TEXT("BattleReady"))
#define ANIM_MONTAGE_EVENT_DIE_01			FName(TEXT("Die01"))
#define ANIM_MONTAGE_EVENT_DIE_02			FName(TEXT("Die02"))
#define ANIM_MONTAGE_EVENT_SPAWN			FString(TEXT("Spawn"))			// 01, 02, 03 ...

#define ANIM_MONTAGE_EVENT_LOBBYIDLE		FName(TEXT("LobbyIdle"))
#define ANIM_MONTAGE_EVENT_LOBBYSOCIAL		FName(TEXT("LobbySocial"))

// DamageResult
#define ANIM_MONTAGE_DAMAGERESULT_HIT		FString(TEXT("Hit"))
#define ANIM_MONTAGE_DAMAGERESULT_DODGE		FString(TEXT("Dodge"))
#define ANIM_MONTAGE_DAMAGERESULT_PARRY		FString(TEXT("Parry"))

// Lobby
#define ANIM_MONTAGE_LOBBY_IDLE				FName(TEXT("DefaultIdle"))
#define ANIM_MONTAGE_LOBBY_SOCIAL			FName(TEXT("TouchAction"))

//////////////////////////////////////////////////////////////////////////