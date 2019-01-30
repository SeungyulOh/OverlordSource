// Generated from PacketDataStructures.h.handlebars
#pragma once


#include "PacketDataEnum.h"
#include "PacketDataStructures.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FDECKS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		campaignThree;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		campaignOne;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		mirror;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		arena;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		defense;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		exp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		now;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		ability;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		icon;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		heroUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		heroId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		exp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		equipedItemUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		skills;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		enchantStat;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		enchantPoint;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		locked;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO_CONFIG
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		heroUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	ECONTENTS_TYPE		contents;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EPREFER_TARGET_TYPE		preferTarget;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<bool>		skills;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FITEM_OPTION
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		idx;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	float		factor;
	
	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FITEM_OPTION& Data)
	{
		Ar << Data.idx;
		Ar << Data.factor;
		return Ar;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FITEM
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		itemUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		itemId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		enchant;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		enchantLocked;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		exp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FITEM_OPTION>		options;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FName>		runes;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		equipedHeroUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		locked;
	
	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FITEM& Data)
	{
		Ar << Data.itemUD;
		Ar << Data.itemId;
		Ar << Data.enchant;
		Ar << Data.enchantLocked;
		Ar << Data.level;
		Ar << Data.exp;
		Ar << Data.options;
		Ar << Data.runes;
		Ar << Data.equipedHeroUD;
		Ar << Data.locked;
		return Ar;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FREWARD
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EREWARD_TYPE		type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		rewardId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		amt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		prob;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FREWARDSET
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FREWARD>		rewards;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPLAY_DECK
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		deckUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EPLAY_DECK_TYPE		pno;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EPLAY_DECK_FORMATION		formation;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		heroUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		power;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FEQUIPED_HERO
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		heroUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		heroId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FITEM>		equipedItems;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		skills;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		enchantStat;
	
	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FEQUIPED_HERO& Data)
	{
		Ar << Data.heroUD;
		Ar << Data.heroId;
		Ar << Data.level;
		Ar << Data.equipedItems;
		Ar << Data.skills;
		Ar << Data.enchantStat;
		return Ar;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO_SNIPPET
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		heroUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		heroId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		grade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRAID_STAGE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		boss;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		stageId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		modes;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		left;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		ds;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		de;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRAID_STAGE_PLAY
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		stageId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		c;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		m;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FEPISODE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		episodeId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		max;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		trial;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		stages;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCHAPTER
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		chapterId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		stars;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		clears;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FEPISODE>		episodes;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FACHIEVE_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EACHIEVE_TYPE		type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		srl;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		amt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		passed_level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		unlocked;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FACHIEVE_UPDATE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EACHIEVE_TYPE		type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		srl;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		amt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		passed_level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		unlocked;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FERROR_INFO
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		code;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		desc;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMAIL
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		mailUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		from;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		subject;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		desc;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		created;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		expire;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FREWARD>		attach;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		isNew;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPD
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		sublevel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		exp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EPD_TYPE		type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		shards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		ticket;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		chip;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		gp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		cp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		ap;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		dp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		defWin;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		defCnt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EPD_LOCK_TYPE		lock;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPD_CHRONICLE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		when;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		msg;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPD_DEFENCE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		defeated;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		chip;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		gp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		dp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		deadHeros;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPD_SNIPPET
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		defWin;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		defCnt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rank;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPD_MINION
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		UD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		mId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EPD_MINION_TYPE		mType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		left;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPD_MINION_INVEN
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FPD_MINION>		supplied;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FPD_MINION>		purchased;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPD_TARGET
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FAVATAR		avatar;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FEQUIPED_HERO>		heros;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FPD_MINION_INVEN		minions;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPD_TREASURENPC
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		mId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		room;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_PRODUCT_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		productId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		buyCount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		coolTime;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_USER_DATA
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FSHOP_PRODUCT_STATUS>		productStatus;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_INFO
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		guild_id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point_rank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		gold;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		desc;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		notice;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		create_date;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		member_count;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		member_limit;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		auto_grade_up;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_MEMBER
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		guild_id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EGUILD_GRADE		grade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		join_date;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		last_update;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_AWAITER
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		elapsedSec;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_HISTORY
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EGUILD_HISTORY_TYPE		type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		target_kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		target_nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EGUILD_GRADE		grade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		time;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RAID_CHALLENGE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		trial;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		spawn;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		leftSec;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RAID
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		modId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		grade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		isOpen;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		left;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		captain;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		members;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		boss;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_MINE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FGUILD_INFO		guild_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FGUILD_MEMBER		guild_member;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FGUILD_MEMBER		guild_master;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		guild_exist;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		isActiveAttendance;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		isActiveContribute;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_EVENT
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	ECHANNEL_EVENT_TYPE		eventType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		value;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		open;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		close;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_FEATURE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	ECHANNEL_EVENT_TYPE		eventType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		value;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_GUILD
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		count;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_DETAIL
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		limit_lv;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		guild_only;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		owner_guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		count_current;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		count_max;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FCHANNEL_GUILD>		guildList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		desc;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FCHANNEL_FEATURE>		feature;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FCHANNEL_EVENT>		event;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		map_name;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_INFO
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		no;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		ip;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		port;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		enter_current;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		enter_max;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		owner_guild_name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FCHANNEL_EVENT>		event_current;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FPLAY_CLEAR_GRADE_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		stageId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		clearGrade;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_REQUESTER
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		timeLeft;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EGUILD_GRADE		grade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		heroId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		maxAmount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		currentAmount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		gettable;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCAMPAIGN_QUEST_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		questId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		stageId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		value;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		trial;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCAMPAIGN_QUEST
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FCAMPAIGN_QUEST_STATUS>		list;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EREWARDABLE_STATUS		rewardable;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		rewardKey;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCAMPAIGN_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		clearGrades;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		episodeRewardStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		dailyClearGrades;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		dailyTicket;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		dailyOpenCards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FCAMPAIGN_QUEST		Elite_1;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		articleId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		category;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		contents;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		time;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		up;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		down;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FEVENT_ATTENDANCE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		attId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		days;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		stamp;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		currency1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		currency2;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		currency3;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FITEM>		items;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FHERO>		heros;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		mailN;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		mailTag;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FDECKS		decks;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		tut;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		size;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_UPDATE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		currency1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		currency2;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		currency3;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FITEM>		items_update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		items_delete;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FHERO>		heros_update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FString>		heros_delete;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		mailN;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		mailTag;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FDECKS		decks;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		tut;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		size;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_CURRENCY
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		currency1;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_EQUIPED
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FITEM>		items;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FHERO>		heros;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FDECKS		decks;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FUNLOCK_NOTIFY
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		notify;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FUNLOCK_LIST
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		list;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDAILY_QUEST_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		score;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		rewardable;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		clearcount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		timeleft;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDAILY_QUEST_UPDATE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		score;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		rewardable;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		clearcount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		timeleft;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FREALTIME_USER_BASIC_INFO
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		guild_id;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FREALTIME_COLOSSEUM_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		league;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		power;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rPoint;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		win;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		lose;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		contWin;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO_SHOP_PRODUCT
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		productId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		currencyType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		price;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		priceTotal;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		primeCostPrice;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		primeCostTotalPrice;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		maxQuantity;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		remainQuantity;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO_SHOP_PANEL
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		panelId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		resetCurrencyType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		resetPrice;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		resetRemainCount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		timer;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FHERO_SHOP_PRODUCT>		productList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		synergyId;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FROA_QUEST
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		quest;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		max;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		now;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		rewardKey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		rewarded;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		complete;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FROA_CHAPTER_QUESTS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		day;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FROA_QUEST>		quests;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FROA_ADVENTURE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		chapter;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		day;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		rewardKey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		rewarded;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		complete;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FROA_CHAPTER_QUESTS>		days;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FBILLING_PREPARE_KAKAOPAY
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		tid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		next_redirect_app_url;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		next_redirect_mobile_url;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		next_redirect_pc_url;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		android_app_scheme;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		ios_app_scheme;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FREALTIME_REWARD
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FREWARD>		rewards;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FREALTIME_BG_REWARD
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FREWARD>		pouch;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FREWARD>		rank;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FACCOUNT_RECORD
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		winCount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		totalCount;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO_USAGE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		GradeID;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		count;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FHERO_USAGE_TAB
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		tab;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FHERO_USAGE>		usages;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCRUSADER_HERO_HP
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		heroUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		hp;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTREASURE
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		treasureId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		left;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		key;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FBG_USER
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rank;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FFLOOR_QUEST_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		questId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		progress;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FFLOOR_MISSION
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		stageId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EFLOOR_MISSION_CLASS_CONDITION_TYPE		cls;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rewardAmt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		goal;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		current;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FFLOOR_MISSION_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		floor;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FFLOOR_MISSION>		missions;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		step;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		timeleft;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		currentFloor;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		clearGrades;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		raidClearGrades;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<bool>		treasureBoxes;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FFLOOR_QUEST_STATUS		floorQuestStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FFLOOR_MISSION_STATUS		floorMissionStatus;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FBATTLE_USER
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		ability;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		icon;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FEQUIPED_HERO>		heros;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FDECKS		decks;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_HISTORY
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		isDefense;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		isWin;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FName>		heroIDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		power;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		deltaPoint;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		seasonEnd;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		seasonLeft;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rankPosition;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FARENA_HISTORY>		history;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		defenseRewardEnabled;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		defenseCount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		defenseCountMax;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		defenseLeftTime;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_RANK_USER
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		power;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_HERO_HP
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		heroUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		hp;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		mirrorUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EMIRROR_TYPE		mirrorType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FBATTLE_USER		user;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		hpList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FREWARD>		rewards;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FMIRROR>		mirrors;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		resetLeftTime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FMIRROR_HERO_HP>		heroHPs;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_HISTORY
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		kId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		gradeId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		isWin;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FName>		heroIDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		power;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		deltaPoint;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	ECOLOSSEUM_PLAY_TYPE		playType;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	bool		seasonEnd;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		seasonLeft;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		gradeId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FCOLOSSEUM_HISTORY>		history;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		win;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		lose;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		continuous_win;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_RANK_USER
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		guild;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rank;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		point;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_FLOORSHOP_SLOT
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		slotId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	ESHOP_FLOORSHOP_SLOT_STATE		state;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		openGem;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	EREWARD_TYPE		type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FName		rewardId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		amt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FString		currencyType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		price;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_FLOORSHOP_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		shopId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		timeLeft;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FSHOP_FLOORSHOP_SLOT>		slots;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FWORLDBOSS_STATUS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		clearGrades;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSELECTIVE_TUTORIAL
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<int32>		flags;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FUPDATE_PACKET
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FAVATAR		avatar;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FINVENTORY_UPDATE		inventoryUpdate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	int32		rewardLength;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FREWARDSET>		rewards;
	
};

USTRUCT(BlueprintType)
struct PROJECTR_API FUPDATE_PACKET_SLIM
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FAVATAR		avatar;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FINVENTORY_UPDATE		inventoryUpdate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FUNLOCK_NOTIFY		unlockNotify;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	TArray<FACHIEVE_UPDATE>		achieveUpdate;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DataStructure")
	FDAILY_QUEST_UPDATE		dailyQuestUpdate;
	
};

