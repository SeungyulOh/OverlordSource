// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "Object/NextTargetActor.h"
#include "GameDataManager.generated.h"


//class URGameInstance;

/**
 * 
 */

USTRUCT(BlueprintType)
struct PROJECTR_API FGameDataItemEnchantInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FGameDataItemEnchantInfo)
	int32							ItemEnchantLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FGameDataItemGradeInfo)
	FSlateColor						ItemEnchantColor;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FGameDataWarningTextColorInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FGameDataWarningTextColorInfo)
	FText							WarningTextColorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FGameDataWarningTextColorInfo)
	FSlateColor						TextColor;
};


UCLASS(Config=Game)
class PROJECTR_API UGameDataManager : public UObject
{
	GENERATED_BODY()

public:
	void		Initialize(/*URGameInstance* GameInstance*/);

//	void		SetGameInstance(URGameInstance* GameInstance){ RGameInstance = GameInstance; }

	void					SpawnNextTargetActor();
	ANextTargetActor*		GetNextTargetActor(bool InCheckPrevHide = true);
	FORCEINLINE void		ClearNextTargetActor() { NextTargetActorList.Empty(); }

	UFUNCTION(BlueprintCallable, Category = UGameDataManager)
	FLinearColor			GetItemEnchantLevelColor(int32 InLevel);

	UFUNCTION(BlueprintCallable, Category = UGameDataManager)
	FLinearColor			GetWarningTextColor(FText InType);



private:
	UPROPERTY()
	FString				NextTargetActorPath;

	UPROPERTY()
	TArray<FGameDataItemEnchantInfo>	ItemEnchantColorList;

	UPROPERTY()
	TArray<FGameDataWarningTextColorInfo>	WarningTextColorList;

	int32				CurIdx = 0;

public:
	UPROPERTY()
	TArray< TWeakObjectPtr< ANextTargetActor > >		NextTargetActorList;

};