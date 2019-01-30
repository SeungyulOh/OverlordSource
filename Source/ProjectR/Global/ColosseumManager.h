// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataStructures.h"
#include "Network/RTSProtocol/rts.struct_generated.h"
#include "Network/RTSProtocol/rts.s2c_generated.h"
#include "EventManager.h"
#include "SaveObject/RSaveGame.h"
#include "ColosseumManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_UISetPlayer, int32, Int);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_UILobbySet);
//OnUpdateColosseumTeamAvatar




UCLASS()
class PROJECTR_API UColosseumManager : public UObject
{
	GENERATED_BODY()

public:
	//	INIT
	UFUNCTION()
	void										Initialize();
	UFUNCTION()
	void										ShutDown();

	UFUNCTION()
	void										RequestColosseumInfo();
	UFUNCTION()
		void OpenColosseumLobby();
	FORCEINLINE void							CurrentToPrevStatus() { PrevColosseumStatus = ColosseumStatus; }
	FORCEINLINE void							SetColosseumStatus(FCOLOSSEUM_STATUS InColosseumStatus) {ColosseumStatus = InColosseumStatus;}
	FORCEINLINE FCOLOSSEUM_STATUS				GetColosseumStatus() { return ColosseumStatus; }

	FORCEINLINE void							SetPrevColosseumStatus(FCOLOSSEUM_STATUS InColosseumStatus) { PrevColosseumStatus = InColosseumStatus; }
	FORCEINLINE FCOLOSSEUM_STATUS				GetPrevColosseumStatus() { return PrevColosseumStatus; }

	FORCEINLINE void							SetColoseumRanking(TArray<FCOLOSSEUM_RANK_USER>& InrankingList) { rankingList = InrankingList; }
	FORCEINLINE TArray<FCOLOSSEUM_RANK_USER>	GetColoseumRanking() { return rankingList; }
	FORCEINLINE void							SetPrevAvatarExp(int32 expVal) { PrevAvatarExp = expVal; }
	FORCEINLINE int32							GetPrevAvatarExp() { return PrevAvatarExp; }
	FORCEINLINE void							SetMaxAvatarExp(int32 expVal) { MaxAvatarExp = expVal; }
	FORCEINLINE int32							GetMaxAvatarExp() { return MaxAvatarExp; }
	FORCEINLINE void							SetAvatar(FAVATAR avatarData) { BeforeColosseumAvatar = avatarData; }
	FORCEINLINE FAVATAR							GetAvatar() { return BeforeColosseumAvatar; }
public:
	UPROPERTY()
	FDelegate_UISetPlayer			OnUpdateColosseumTeamAvatar;
	UPROPERTY()
	FDelegate_UILobbySet			ColosseumUI;
	UPROPERTY()
		FAVATAR				BeforeColosseumAvatar;
	UPROPERTY()
		FCOLOSSEUM_STATUS				PrevColosseumStatus;
	UPROPERTY()
	FCOLOSSEUM_STATUS				ColosseumStatus;
	UPROPERTY()
	TArray<FCOLOSSEUM_RANK_USER>	rankingList;
	UPROPERTY()
		TArray<FREWARD>		m_rewards;

	//event delegate
	UPROPERTY()
	FDelegate_NoParam openColosseumUI;

	UPROPERTY()
	FColosseumSaveData	CS_SaveData;

	int32 PrevAvatarExp = 0;
	int32 MaxAvatarExp = 0;
	int32 currentGradeIndex = 0;
};