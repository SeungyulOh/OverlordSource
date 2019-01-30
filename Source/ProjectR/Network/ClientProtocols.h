// Generated from ClientProtocols.h.handlebars
#pragma once


#include "PacketTypeEnum.h"
#include "PacketDataEnum.h"
#include "PacketDataStructures.h"
#include "ClientProtocols.generated.h"

// PDU.ver
#define PROTOCOL_VERSION TEXT("17001.56819.1545809356798")

USTRUCT(BlueprintType)
struct PROJECTR_API FLOGIN_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::LOGIN_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		flag;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FLOGIN_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::LOGIN_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		success;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FACCOUNT_CREATE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ACCOUNT_CREATE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		language;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		nick;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FACCOUNT_CREATE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ACCOUNT_CREATE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		exists;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FACCOUNT_REMOVE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ACCOUNT_REMOVE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FACCOUNT_REMOVE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ACCOUNT_REMOVE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		exists;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		nick;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINIT_CLIENT_KEY_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INIT_CLIENT_KEY_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		pwd;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		redirect_uri;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		access_token;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		code;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		publisher;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINIT_CLIENT_KEY_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INIT_CLIENT_KEY_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FADD_REFER_PUBLISHER_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ADD_REFER_PUBLISHER_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		pwd;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		redirect_uri;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		access_token;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		code;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		publisher;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FADD_REFER_PUBLISHER_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ADD_REFER_PUBLISHER_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGET_REFER_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GET_REFER_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGET_REFER_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GET_REFER_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		publisher_list;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FACCOUNT_CHANGE_LANGUAGE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ACCOUNT_CHANGE_LANGUAGE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		language;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FACCOUNT_CHANGE_LANGUAGE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ACCOUNT_CHANGE_LANGUAGE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		success;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCLIENT_SDK_ACTION_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CLIENT_SDK_ACTION_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		flag;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCLIENT_SDK_ACTION_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CLIENT_SDK_ACTION_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FAVATAR		avatar;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FARENA_STATUS		arenaStatus;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_TARGET_INFO_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_TARGET_INFO_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		refresh;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_TARGET_INFO_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_TARGET_INFO_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FBATTLE_USER		target;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_PLAY_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_PLAY_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		target_power;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		power;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		heroUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		target_kId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_PLAY_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_PLAY_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		playUD;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_PLAY_DONE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_PLAY_DONE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		isWin;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		playUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_PLAY_DONE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_PLAY_DONE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FARENA_STATUS		arenaStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_DEFENSE_REWARD_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_DEFENSE_REWARD_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_DEFENSE_REWARD_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_DEFENSE_REWARD_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FARENA_STATUS		arenaStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_SEASON_DONE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_SEASON_DONE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_SEASON_DONE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_SEASON_DONE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FARENA_STATUS		arenaStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_RANKING_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_RANKING_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_RANKING_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_RANKING_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FARENA_RANK_USER>		rankingList;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_DEFENSE_DECK_SET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_DEFENSE_DECK_SET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		defense;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FARENA_DEFENSE_DECK_SET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ARENA_DEFENSE_DECK_SET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_ABILITY_UP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_ABILITY_UP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		point;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		abilityIndex;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_ABILITY_UP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_ABILITY_UP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_ABILITY_RESET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_ABILITY_RESET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_ABILITY_RESET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_ABILITY_RESET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_ICON_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_ICON_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_ICON_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_ICON_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<int32>		iconList;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_ICON_SET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_ICON_SET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		icon;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_ICON_SET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_ICON_SET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_PROFILE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_PROFILE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_PROFILE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_PROFILE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FHERO_USAGE_TAB>		heroUse;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FACCOUNT_RECORD>		records;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_LOBBY_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_LOBBY_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_LOBBY_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_LOBBY_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FDAILY_QUEST_STATUS		daily_quest_status;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTREASURE		treasure;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_GAME_DATA_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_GAME_DATA_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_GAME_DATA_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_GAME_DATA_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FSELECTIVE_TUTORIAL		selectiveTutorial;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FDAILY_QUEST_STATUS		dailyQuestStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FACHIEVE_STATUS>		achieveStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUNLOCK_LIST		unlockList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTOWER_STATUS		towerStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FEVENT_ATTENDANCE		chart;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_MINE		guild_mine;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FINVENTORY		inventory;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FAVATAR		avatar;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		colosseumGradeId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		value;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		idx;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FAVATAR_SELECTIVE_TUTORIAL_UPDATE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FSELECTIVE_TUTORIAL		selectiveTutorial;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_INFO_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CHANNEL_INFO_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_INFO_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CHANNEL_INFO_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FCHANNEL_INFO>		channel_list;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_DETAIL_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CHANNEL_DETAIL_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		no;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCHANNEL_DETAIL_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CHANNEL_DETAIL_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FCHANNEL_DETAIL		channel_detail;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FBILLING_TRANSACTION_CREATE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::BILLING_TRANSACTION_CREATE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		redirect;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		product;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		purchase_system;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FBILLING_TRANSACTION_CREATE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::BILLING_TRANSACTION_CREATE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FBILLING_PREPARE_KAKAOPAY		kakaopay;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		payload;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		transaction_id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		product;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		purchase_system;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FBILLING_TRANSACTION_SUCCESS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::BILLING_TRANSACTION_SUCCESS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		transaction_id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		product;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		receipt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		purchase_system;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FBILLING_TRANSACTION_SUCCESS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::BILLING_TRANSACTION_SUCCESS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		receipt;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		purchase_system;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COLOSSEUM_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COLOSSEUM_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FCOLOSSEUM_STATUS		colosseumStatus;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_PLAY_RESULT_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COLOSSEUM_PLAY_RESULT_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		roomKey;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_PLAY_RESULT_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COLOSSEUM_PLAY_RESULT_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FCOLOSSEUM_STATUS		colosseumStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		isWin;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_SEASON_DONE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COLOSSEUM_SEASON_DONE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_SEASON_DONE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COLOSSEUM_SEASON_DONE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FCOLOSSEUM_STATUS		colosseumStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		gradeRewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rankRewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_RANKING_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COLOSSEUM_RANKING_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOLOSSEUM_RANKING_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COLOSSEUM_RANKING_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FCOLOSSEUM_RANK_USER>		rankingList;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE_WRITE_HERO_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COMMUNITY_ARTICLE_WRITE_HERO_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		contents;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE_WRITE_HERO_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COMMUNITY_ARTICLE_WRITE_HERO_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FCOMMUNITY_ARTICLE		article;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE_DELETE_HERO_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COMMUNITY_ARTICLE_DELETE_HERO_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		articleId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE_DELETE_HERO_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COMMUNITY_ARTICLE_DELETE_HERO_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		success;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE_LIST_HERO_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COMMUNITY_ARTICLE_LIST_HERO_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		sortType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE_LIST_HERO_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COMMUNITY_ARTICLE_LIST_HERO_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FCOMMUNITY_ARTICLE>		articleList;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE_VOTE_HERO_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COMMUNITY_ARTICLE_VOTE_HERO_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		recommend;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		articleId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCOMMUNITY_ARTICLE_VOTE_HERO_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::COMMUNITY_ARTICLE_VOTE_HERO_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FCOMMUNITY_ARTICLE		article;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCONTENT_RESET_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CONTENT_RESET_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		resetKey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		resetType;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCONTENT_RESET_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CONTENT_RESET_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		contentResetCount;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FCONTENT_RESET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CONTENT_RESET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		resetKey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		resetType;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCONTENT_RESET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::CONTENT_RESET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FATTENDANCE_GET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ATTENDANCE_GET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FATTENDANCE_GET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::ATTENDANCE_GET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FEVENT_ATTENDANCE		chart;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CREATE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CREATE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		auto_grade_up;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		notice;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		desc;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		name;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CREATE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CREATE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_INFO		guild_info;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CLOSE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CLOSE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CLOSE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CLOSE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		success;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_GET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_GET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_GET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_GET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_MINE		guild_mine;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_GET_GUEST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_GET_GUEST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		page;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		guild_id;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_GET_GUEST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_GET_GUEST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_MEMBER>		guild_member_list;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_INFO		guild_info;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_MEMBER_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_MEMBER_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		page;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_MEMBER_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_MEMBER_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_MEMBER>		guild_member_list;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_AWAITER_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_AWAITER_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		page;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_AWAITER_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_AWAITER_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_AWAITER>		guild_awaiter_list;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_AWAITER_ACCEPT_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_AWAITER_ACCEPT_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		accept;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		target_kId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_AWAITER_ACCEPT_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_AWAITER_ACCEPT_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		target_kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_AWAITER_CANCEL_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_AWAITER_CANCEL_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		guild_id;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_AWAITER_CANCEL_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_AWAITER_CANCEL_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		guild_id;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_FIND_NAME_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_FIND_NAME_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		name;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_FIND_NAME_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_FIND_NAME_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<int32>		awaiting_guild_ids;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_INFO>		guild_list;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CHANGE_NAME_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CHANGE_NAME_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		name;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CHANGE_NAME_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CHANGE_NAME_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_INFO		guild_info;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_EDIT_DESC_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_EDIT_DESC_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		desc;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_EDIT_DESC_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_EDIT_DESC_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_INFO		guild_info;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_EDIT_NOTICE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_EDIT_NOTICE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		notice;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_EDIT_NOTICE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_EDIT_NOTICE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_INFO		guild_info;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_EDIT_AUTO_GRADE_UP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_EDIT_AUTO_GRADE_UP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		auto_grade_up;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_EDIT_AUTO_GRADE_UP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_EDIT_AUTO_GRADE_UP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_INFO		guild_info;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_JOIN_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_JOIN_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		guild_id;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_JOIN_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_JOIN_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		auto_grade_up;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		guild_id;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_JOIN_AUTO_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_JOIN_AUTO_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_JOIN_AUTO_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_JOIN_AUTO_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		auto_grade_up;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		guild_id;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_MEMBER_GRADE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_MEMBER_GRADE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		grade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		target_kId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_MEMBER_GRADE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_MEMBER_GRADE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_MEMBER		target_member;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_MEMBER		guild_member;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_MEMBER_KICK_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_MEMBER_KICK_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		target_kId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_MEMBER_KICK_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_MEMBER_KICK_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		target_kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CONTRIBUTION_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CONTRIBUTION_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		contributionType;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CONTRIBUTION_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CONTRIBUTION_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	EGUILD_CONTRIBUTION_TYPE		contributionType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_MEMBER		guild_member;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_INFO		guild_info;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CONTRIBUTE_RANKING_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CONTRIBUTE_RANKING_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		page;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CONTRIBUTE_RANKING_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CONTRIBUTE_RANKING_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<int32>		awaiting_guild_ids;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_INFO>		guild_list;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CONTRIBUTION_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CONTRIBUTION_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_CONTRIBUTION_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_CONTRIBUTION_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	EGUILD_CONTRIBUTION_TYPE		contributionType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	EREWARDABLE_STATUS		contributable;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_ATTENDANCE_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_ATTENDANCE_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_ATTENDANCE_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_ATTENDANCE_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		previousWeeklyCount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		weeklyTimeLeft;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		weeklyCount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	EREWARDABLE_STATUS		weeklyRewardable;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	EREWARDABLE_STATUS		dailyRewardable;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_ATTENDANCE_DAILY_REWARD_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_ATTENDANCE_DAILY_REWARD_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_ATTENDANCE_DAILY_REWARD_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_ATTENDANCE_DAILY_REWARD_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		weeklyCount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	EREWARDABLE_STATUS		dailyRewardable;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_ATTENDANCE_WEEKLY_REWARD_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_ATTENDANCE_WEEKLY_REWARD_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_ATTENDANCE_WEEKLY_REWARD_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_ATTENDANCE_WEEKLY_REWARD_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	EREWARDABLE_STATUS		weeklyRewardable;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_RUNE_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_RUNE_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_RUNE_REQUESTER>		requests;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_REQUEST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_RUNE_REQUEST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FName		heroId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_REQUEST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_RUNE_REQUEST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_RUNE_REQUESTER>		requests;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_SUPPORT_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_RUNE_SUPPORT_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		amount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FName		heroId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		requester_kId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_SUPPORT_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_RUNE_SUPPORT_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_RUNE_REQUESTER>		requests;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_GET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_RUNE_GET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_RUNE_GET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_RUNE_GET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_RUNE_REQUESTER>		requests;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_HISTORY_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_HISTORY_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_HISTORY_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_HISTORY_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FGUILD_HISTORY>		historyList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_LEVEL_UP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_LEVEL_UP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		level;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGUILD_LEVEL_UP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GUILD_LEVEL_UP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FGUILD_INFO		guild_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_UPGRADE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_UPGRADE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		materials4;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		materials3;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		materials2;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		materials1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_UPGRADE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_UPGRADE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_LEVELUP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_LEVELUP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_LEVELUP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_LEVELUP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_GRIND_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_GRIND_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		heroUDs;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_GRIND_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_GRIND_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_EQUIP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_EQUIP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		itemUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_EQUIP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_EQUIP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_UNEQUIP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_UNEQUIP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		itemUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_UNEQUIP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_UNEQUIP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_GRIND_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_GRIND_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		itemUDs;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_GRIND_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_GRIND_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_RUNE_EQUIP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_RUNE_EQUIP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		equipRunes;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		itemUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_RUNE_EQUIP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_RUNE_EQUIP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_RUNE_UNEQUIP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_RUNE_UNEQUIP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<bool>		unequipSlots;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		itemUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_RUNE_UNEQUIP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_RUNE_UNEQUIP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_RUNE_MAKE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_RUNE_MAKE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		amount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		rune;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_RUNE_MAKE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_RUNE_MAKE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_LEVELUP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_LEVELUP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		amount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		itemUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_LEVELUP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_LEVELUP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		isLucky;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_ENCHANT_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_ENCHANT_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		useProtection;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		specialEnchant;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		itemUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_ENCHANT_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_ENCHANT_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		affectedEnchant;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_HERO_LOCK_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_HERO_LOCK_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		heroUDs_unlock;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		heroUDs_lock;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		itemUDs_unlock;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		itemUDs_lock;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_HERO_LOCK_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_HERO_LOCK_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_SKILL_LEVELUP_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_SKILL_LEVELUP_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		slot;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_SKILL_LEVELUP_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_SKILL_LEVELUP_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_ENCHANT_CONSUME_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_ENCHANT_CONSUME_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		spiritUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_ENCHANT_CONSUME_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_ENCHANT_CONSUME_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<bool>		spiritResult;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_ENCHANT_STAT_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_ENCHANT_STAT_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<int32>		enchantStat;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		heroUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_HERO_ENCHANT_STAT_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_HERO_ENCHANT_STAT_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_SELL_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_SELL_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<int32>		walletQtyList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		walletIdList;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		itemUDs;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_SELL_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_SELL_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_UPGRADE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_UPGRADE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		material_itemUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		itemUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_UPGRADE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_UPGRADE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		isSuccess;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_OPTION_RESET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_OPTION_RESET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		material_itemUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		itemUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_ITEM_OPTION_RESET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_ITEM_OPTION_RESET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_TIMETREASURE_GET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_TIMETREASURE_GET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_TIMETREASURE_GET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_TIMETREASURE_GET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_UPGRADE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_UPGRADE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		upgradeType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		upgrade;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FINVENTORY_UPGRADE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::INVENTORY_UPGRADE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGIFT_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GIFT_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		size;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		holds;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGIFT_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GIFT_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FMAIL>		mails;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FMAIL>		news;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		mailsize;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGIFT_OPEN_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GIFT_OPEN_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		mailUDs;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGIFT_OPEN_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GIFT_OPEN_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		mailUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSPECIAL_GIFT_LIST_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SPECIAL_GIFT_LIST_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		size;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		holds;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSPECIAL_GIFT_LIST_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SPECIAL_GIFT_LIST_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FMAIL>		mails;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FMAIL>		news;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		mailsize;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSPECIAL_GIFT_OPEN_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SPECIAL_GIFT_OPEN_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		giftIdx;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		mailUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSPECIAL_GIFT_OPEN_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SPECIAL_GIFT_OPEN_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		mailUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FMIRROR_STATUS		mirror_status;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_PLAY_START_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_PLAY_START_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		heroUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		mirrorUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_PLAY_START_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_PLAY_START_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		playUD;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_PLAY_DONE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_PLAY_DONE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<int32>		target_heroHPs_hp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<int32>		heroHPs_hp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		clearGrade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		playUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_PLAY_DONE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_PLAY_DONE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FMIRROR_STATUS		mirror_status;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_RESET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_RESET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		useGem;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_RESET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_RESET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FMIRROR_STATUS		mirror_status;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_DEFENSE_DECK_SET_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_DEFENSE_DECK_SET_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		defense;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FMIRROR_DEFENSE_DECK_SET_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::MIRROR_DEFENSE_DECK_SET_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGIVE_ACHIEVE_REWARD_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GIVE_ACHIEVE_REWARD_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		target_level;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		srl;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGIVE_ACHIEVE_REWARD_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GIVE_ACHIEVE_REWARD_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FACHIEVE_STATUS>		achieveStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		kId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FDAILY_QUEST_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::DAILY_QUEST_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDAILY_QUEST_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::DAILY_QUEST_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FDAILY_QUEST_STATUS		daily_quest_status;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FDAILY_QUEST_REWARD_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::DAILY_QUEST_REWARD_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		idx;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FDAILY_QUEST_REWARD_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::DAILY_QUEST_REWARD_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FDAILY_QUEST_STATUS		daily_quest_status;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_USER_DATA_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_USER_DATA_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_USER_DATA_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_USER_DATA_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FSHOP_USER_DATA		shopUserData;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_BUY_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_BUY_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		price;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		currencyType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		amount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		productType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		productId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_BUY_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_BUY_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FSHOP_USER_DATA		shopUserData;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARDSET>		rewardSet;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		productId;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_CHARACTER_SUMMON_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_CHARACTER_SUMMON_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		summonId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_CHARACTER_SUMMON_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_CHARACTER_SUMMON_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		extraRewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		summonRewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_FLOORSHOP_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_FLOORSHOP_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_FLOORSHOP_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_FLOORSHOP_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FSHOP_FLOORSHOP_STATUS		status;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_FLOORSHOP_BUY_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_FLOORSHOP_BUY_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		slotId;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		shopId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_FLOORSHOP_BUY_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_FLOORSHOP_BUY_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FSHOP_FLOORSHOP_STATUS		status;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_FLOORSHOP_OPEN_SLOT_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_FLOORSHOP_OPEN_SLOT_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		slotId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FSHOP_FLOORSHOP_OPEN_SLOT_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::SHOP_FLOORSHOP_OPEN_SLOT_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FSHOP_FLOORSHOP_STATUS		status;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTOWER_STATUS		tower_status;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_PLAY_START_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_PLAY_START_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FString>		heroUDs;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		stageId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_PLAY_START_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_PLAY_START_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		playUD;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		dropRewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_PLAY_DONE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_PLAY_DONE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		playTime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		clearGrade;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		playUD;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_PLAY_DONE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_PLAY_DONE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTOWER_STATUS		tower_status;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		questRewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		clearRewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		dropRewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_TREASURE_BOX_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_TREASURE_BOX_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		boxIdx;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_TREASURE_BOX_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_TREASURE_BOX_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTOWER_STATUS		tower_status;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_BOSS_PLAY_CHECK_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_BOSS_PLAY_CHECK_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		stageId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_BOSS_PLAY_CHECK_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_BOSS_PLAY_CHECK_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		isValid;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_BOSS_PLAY_RESULT_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_BOSS_PLAY_RESULT_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		roomKey;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_BOSS_PLAY_RESULT_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_BOSS_PLAY_RESULT_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTOWER_STATUS		tower_status;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		playTime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	EPLAY_CLEAR_GRADE		clearGrade;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_FLOOR_MISSION_ACTIVE_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_FLOOR_MISSION_ACTIVE_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32		floor;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_FLOOR_MISSION_ACTIVE_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_FLOOR_MISSION_ACTIVE_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTOWER_STATUS		tower_status;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_FLOOR_MISSION_REWARD_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_FLOOR_MISSION_REWARD_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		id;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTOWER_FLOOR_MISSION_REWARD_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TOWER_FLOOR_MISSION_REWARD_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		complete_rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTOWER_STATUS		tower_status;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FTREASURE_GET_REWARD_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TREASURE_GET_REWARD_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FTREASURE_GET_REWARD_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::TREASURE_GET_REWARD_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FTREASURE		treasure;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FGET_TARGET_SERVER_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GET_TARGET_SERVER_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGET_TARGET_SERVER_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::GET_TARGET_SERVER_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		manifestBuildNumber;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		tcpServerPort;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		tcpServerHost;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		cdn;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		serverHost;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		env;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FWORLDBOSS_STATUS_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::WORLDBOSS_STATUS_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FWORLDBOSS_STATUS_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::WORLDBOSS_STATUS_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FWORLDBOSS_STATUS		worldbossStatus;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FWORLDBOSS_PLAY_CHECK_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::WORLDBOSS_PLAY_CHECK_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		worldBossStageId;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FWORLDBOSS_PLAY_CHECK_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::WORLDBOSS_PLAY_CHECK_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		isValid;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FWORLDBOSS_PLAY_RESULT_RQ
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::WORLDBOSS_PLAY_RESULT_RQ;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		clientkey;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		deviceid;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		protocol;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		version;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FString		roomKey;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FWORLDBOSS_PLAY_RESULT_RP
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	int32 type = EPacketType::WORLDBOSS_PLAY_RESULT_RP;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FERROR_INFO		error_info;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	ECRUD		is;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FUPDATE_PACKET_SLIM		update;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	FWORLDBOSS_STATUS		worldbossStatus;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	TArray<FREWARD>		rewards;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Network")
	bool		isWin;
};


