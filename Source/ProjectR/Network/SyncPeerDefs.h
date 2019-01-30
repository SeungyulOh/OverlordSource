// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SharedConstants/GlobalConstants.h"
#include "SyncPeerDefs.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FCharacterMoveData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	bool		UseState = true;
	UPROPERTY()
	int32		UserKID = KID_NONE;
	UPROPERTY()
	int32		PlayerUK = PLAYER_UK_NONE;
	UPROPERTY()
	int32		PartyID = PartyID_NONE;
	UPROPERTY()
	FVector		CurrentLocation = FVector::ZeroVector;
	UPROPERTY()
	FVector		ForwardVector = FVector::ZeroVector;
	UPROPERTY()
	FVector		TargetLocation = FVector::ZeroVector;

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FCharacterMoveData& Data)
	{
		Ar << Data.UseState;
		Ar << Data.UserKID;
		Ar << Data.PlayerUK;
		Ar << Data.PartyID;
		Ar << Data.CurrentLocation;
		Ar << Data.ForwardVector;
		Ar << Data.TargetLocation;

		return Ar;
	}
};

USTRUCT(BlueprintType)
struct PROJECTR_API FColosseumAvatarData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int32		UserKID = 0;
	UPROPERTY()
	int32		UserLevel = 0;
	UPROPERTY()
	FString		UserNick = "";
	UPROPERTY()
	FString		GuildName = "";
	UPROPERTY()
	int32		WinningRate = 0;

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FColosseumAvatarData& Data)
	{
		Ar << Data.UserKID;
		Ar << Data.UserLevel;
		Ar << Data.UserNick;
		Ar << Data.GuildName;
		Ar << Data.WinningRate;

		return Ar;
	}
};