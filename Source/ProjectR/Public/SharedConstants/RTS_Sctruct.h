// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Network/ClientProtocols.h"
#include "Network/RTSProtocol/rts.struct_generated.h"
#include "Network/RTSProtocol/rts.s2c_generated.h"
#include "RTS_Sctruct.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FRTSInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int16		kId;		//	내 유니크 아이디
	UPROPERTY()
	int16		partyNo;	//	내가 접속해 있는 방의 번호
	UPROPERTY()
	FString		modId;		//	내가 접속해 있는 룸
	UPROPERTY()
	EPVP		modType;	//	내가 접속해 있는 모드 타입
	UPROPERTY()
	bool		bPublic;	//	내가 접속해 있는 룸의 비공개 여부
	UPROPERTY()
	uint8		mSlotNo;	//	내가 접속해 있는 룸에서 나의 번호
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDeckSelected
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int16		slot;	//	내가 접속해 있는 룸에서 나의 번호
	UPROPERTY()
	int16		crewNo;	//	내 캐릭터의 역활 [리터,크루1,크루2,대기중인크루1, 대기중인 크루2]
	UPROPERTY()
	int16		level;	//	내 캐릭터 레벨
	UPROPERTY()
	FString		heroUD;	//	내 케릭터 UD
	UPROPERTY()
	FString		heroId;	//	내 캐릭터 ID

	void SetData(OVERLORD::RTS::S2C::ACK_DECK_SELECTT* InData)		//	서버에서 보내는 데이터를 클라 데이터로 대입
	{
		slot = InData->slot;
		level = InData->level;
		crewNo = InData->crewNo;
		heroUD = UTF8_TO_TCHAR(InData->heroUD.c_str());
		heroId = UTF8_TO_TCHAR(InData->heroId.c_str());
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCSDeckSelected
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int32		kId;		//	아바타의 유니크 ID
	UPROPERTY()
	int16		crewNo;		//	해당 아바타의 캐릭터의 역활 [리터,크루1,크루2,대기중인크루1, 대기중인 크루2]
	UPROPERTY()
	int16		level;		//	해당 아바타의 캐릭터의 레벨
	UPROPERTY()
	FString		heroUD;		//	해당 아바타의 캐릭터의 UD
	UPROPERTY()
	FString		heroId;		//	해당 아바타의 캐릭터의 ID

	void SetData(OVERLORD::RTS::S2C::ACK_CS_DECK_SELECTT* InData)		//	서버에서 보내는 데이터를 클라 데이터로 대입
	{
		kId = InData->kId;
		level = InData->level;
		crewNo = InData->crewNo;
		heroUD = UTF8_TO_TCHAR(InData->heroUD.c_str());
		heroId = UTF8_TO_TCHAR(InData->heroId.c_str());
	}
};

//////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRTS_PARTY_SLOT
{
	GENERATED_USTRUCT_BODY()
public:
	uint8	slot;			//	접속해 있는 룸에서 번호
	int16	kId;			//	아바타의 유니크 ID
	uint16	actorUK;

	void SetData(OVERLORD::RTS::STRUCT::PARTY_SLOTT* InData)	//	서버에서 보내는 데이터를 클라 데이터로 대입
	{
		slot = InData->slot;
		kId = InData->kId;
		actorUK = InData->actorUK;
	}
};

USTRUCT(BlueprintType)
struct FRTS_PARTY
{
	GENERATED_USTRUCT_BODY()
public:
	uint8					party;			//	파티 번호
	UPROPERTY()
		TArray<FRTS_PARTY_SLOT>	slots;		//	파티 내 여려명의 FRTS_PARTY_SLOT 정보

	void SetData(OVERLORD::RTS::STRUCT::PARTYT* InData)	//	서버에서 보내는 데이터를 클라 데이터로 대입
	{
		party = InData->party;

		slots.Empty();
		for (auto& slotData : InData->slots)
		{
			FRTS_PARTY_SLOT newSlotData;
			newSlotData.SetData(slotData.get());
			slots.Add(newSlotData);
		}
	}
};

USTRUCT(BlueprintType)
struct FRTS_TEAM
{
	GENERATED_USTRUCT_BODY()
public:
	uint16				team;			//	팀의 번호
	UPROPERTY()
	TArray<FRTS_PARTY>	parties;		//	해당 팀내에서의 FRTS_PARTY 정보

	void SetData(OVERLORD::RTS::STRUCT::TEAMT* InData)
	{
		team = InData->team;

		parties.Empty();
		for (auto& partyData : InData->parties) {
			FRTS_PARTY newPartyData;
			newPartyData.SetData(partyData.get());
			parties.Add(newPartyData);
		}
	}
};

//////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRTS_PLAYER
{
	GENERATED_USTRUCT_BODY()
public:
	int16			kId;			//	아바타의 유니크 ID
	FString			nick;			//	아바타의 닉네임
	int16			level;			//	아바타의 레벨
	FString			guild;			//	아바타의 길드
	int16			guild_id;		//	아바타의 길드 ID
	TArray<int16>	ability;		//	아바타의 스킬
	uint16			team;			//	아바타가 속해 있는 팀
	uint8			party;			//	아바타가 속해 있는 파티
	uint8			slot;			//	아바타의 슬롯

	FRTS_PLAYER() : kId(0), level(0), guild_id(0), team(0), party(0), slot(0)
	{
		nick.Empty();
		guild.Empty();
		ability.Empty();
	}

	void SetData(OVERLORD::RTS::STRUCT::PLAYERT* InData)
	{
		kId = InData->kId;
		nick = UTF8_TO_TCHAR(InData->nick.c_str());
		level = InData->level;
		guild = UTF8_TO_TCHAR(InData->guild.c_str());
		guild_id = InData->guild_id;

		ability.Empty();
		for (auto& abilityData : InData->ability) {
			ability.Add(abilityData);
		}

		team = InData->team;
		party = InData->party;
		slot = InData->slot;
	}
};

//////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRTS_ITEM_OPTION
{
	GENERATED_USTRUCT_BODY()
public:
	FString		option;			//	아이템 옵션의 정보
	int16		value;			//	아이템 옵션의 수치

	void SetData(OVERLORD::RTS::STRUCT::ITEM_OPTIONT* InData)
	{
		option = UTF8_TO_TCHAR(InData->option.c_str());
		value = InData->value;
	}
};

USTRUCT(BlueprintType)
struct FRTS_ITEM
{
	GENERATED_USTRUCT_BODY()
public:
	FString		itemId;							//	아이템 ID
	int16		enchant;						//	인첸트 정보
	int16		enchantValue;
	TArray<FRTS_ITEM_OPTION>	options;		//	아이템 랜덤 옵션 ?
	TArray<FString>				jewels;			//	아이템 룬 옵션 ?

	void SetData(OVERLORD::RTS::STRUCT::ITEMT* InData)
	{
		itemId = UTF8_TO_TCHAR(InData->itemId.c_str());
		enchant = InData->enchant;
		enchantValue = InData->enchantValue;

		options.Empty();
		for (auto& optionData : InData->options) {
			FRTS_ITEM_OPTION newItemOptionData;
			newItemOptionData.SetData(optionData.get());
			options.Add(newItemOptionData);
		}

		jewels.Empty();
		for (auto& jewelData : InData->jewels) {
			jewels.Add(UTF8_TO_TCHAR(jewelData.c_str()));
		}
	}
};

USTRUCT(BlueprintType)
struct FRTS_ACTOR
{
	GENERATED_USTRUCT_BODY()
public:
	uint16		actorUK;					
	uint8		actorCategory;				//	캐릭터의 카테고리	(영웅인지, npc인지, 몬스터인지 등등)
	FName		actorId;					//	캐릭터의 ID
	int16		kId;						
	FString		nick;						//	캐릭터 이름
	FString		subNick;					//	길드명
	TArray<int16>		ability;			//	아바타 스킬
	bool		isSummon;					//	소환수인가
	uint16		primaryUK;					//  주인 UK
	float		primaryFE;
	uint16		power;						//	전투력
	uint16		maxHp;						//	최대 체력
	int16		level;						//	레벨
	int16		grade;						//	등급
	TArray<uint8>		skills;				//	스킬
	TArray<FRTS_ITEM>	items;				//	아이템
	uint16		team;						//	아바타가 속해 있는 팀
	uint8		party;						//	아바타가 속해 있는 파티
	uint8		slot;						//	아바타가 속해 있는 슬롯
	uint16		teamCode;					//  ????????
	uint16		curHp;						//	현재 체력
	uint8		role;
	FString		heroUD;						//	캐릭터의 UD

	void SetData(OVERLORD::RTS::STRUCT::ACTORT* InData)
	{
		actorUK = InData->actorUK;
		actorCategory = InData->actorCategory;
		actorId = FName(UTF8_TO_TCHAR(InData->actorId.c_str()));
		kId = InData->kId;
		nick = UTF8_TO_TCHAR(InData->nick.c_str());
		subNick = UTF8_TO_TCHAR(InData->subNick.c_str());
		role = InData->role;
		ability.Empty();
		for (auto& abilityData : InData->ability) {
			ability.Add(abilityData);
		}

		isSummon = InData->isSummon;
		primaryUK = InData->primaryUK;
		primaryFE = InData->primaryFE;
		power = InData->power;
		maxHp = InData->maxHp;
		level = InData->level;
		grade = InData->grade;

		skills.Empty();
		for (auto& skillData : InData->skills) {
			skills.Add(skillData);
		}

		items.Empty();
		for (auto& itemData : InData->items) {
			FRTS_ITEM newItemData;
			newItemData.SetData(itemData.get());
			items.Add(newItemData);
		}

		team = InData->team;
		party = InData->party;
		slot = InData->slot;
		teamCode = InData->teamCode;
		curHp = InData->curHp;

		heroUD = FString::FromInt(actorUK);
	}
};

USTRUCT(BlueprintType)
struct FRTS_ACTOR_UPDATE
{
	GENERATED_USTRUCT_BODY()
public:
	uint16		actorUK;			//	캐릭터의 유니크 아이디
	bool		isStop;				//	캐릭터가 멈춰 있는 상태 인가
	FVector		location;			//	캐릭터의 위치
	FVector		direction;			//	캐릭터의 방향

	void SetData(OVERLORD::RTS::STRUCT::ACTOR_UPDATET* InData)
	{
		actorUK = InData->actorUK;
		isStop = InData->isStop;
		location = FVector(InData->location->X, InData->location->Y, InData->location->Z);
		direction = FVector(InData->direction->X, InData->direction->Y, InData->direction->Z);
	}
};
//////////////////////////////////////////////////////////////////////////
// USTRUCT(BlueprintType)
// struct PROJECTR_API FBattleGroundTeamData
// {
// 	GENERATED_USTRUCT_BODY()
// public:
// 	UPROPERTY()
// 		int16		GroupKey = 0;
// 	UPROPERTY()
// 		int16		TeamMedalCount = 0;
// 	TArray<OVERLORD::RTS::STRUCT::PLAYER_SCORET>	TeamRanking;
// 
// 	FBattleGroundTeamData() {}
// 	FBattleGroundTeamData(int16 InGroupKey) : GroupKey(InGroupKey) {
// 		TeamMedalCount = 0;
// 		TeamRanking.Empty();
// 	}
// };