// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketTypeEnum.h"
#include "PacketErrorManager.generated.h"

//class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UPacketErrorManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(void);
	bool IsCheckError(EPacketType InLastRequestPacketType, ECRUD is, FERROR_INFO InErrorInfo);
	void ProcessError(bool InCommon = true);
	void OnHTTPDisconnect();
	
	UFUNCTION()
	void Retransmit();
	UFUNCTION()
	void GameRestart();

	//Setter
	void SetLastRequestPacketErrorInfo(FERROR_INFO InErrorInfo);
	void SetLastRequestPacketType(EPacketType InLastRequestPacketType);
	void SetLastRequestJsonString(FString& InRequestJsonString);
	//Getter
	FERROR_INFO	GetLastPacketErrorInfo();
	EPacketType GetLastRequestPacketType();
	
private:
	FERROR_INFO		LastRequestPacketErrorInfo;
	EPacketType		LastRequestPacketType;
	//URGameInstance* RGameInstance = nullptr;
	FString			LastRequestJsonString;
	
};
