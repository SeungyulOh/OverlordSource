// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Table/LevelGrowth.h"
#include "Table/CharacterTableInfo.h"

#include "UC_HeroManagement.generated.h"

class ACharacterSetCameraActor;
class UCharacterBattleInfo;
class URGameInstance;

/**
 * BP: UC_HeroManagement
 */
UCLASS()
class PROJECTR_API UUC_HeroManagement : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;
	//void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//void InvalidateData() override;

	void InvalidateByHeroUD(const FString& InHeroUD);
	/************************************************************************/
	/* OnClick Event                                                        */
	/************************************************************************/
	//UFUNCTION(BlueprintCallable, Category = UC_HeroManagement)
	//void OnClick_ShowHeroExp();

	//UFUNCTION(BlueprintCallable, Category = UC_HeroManagement)
	//void OnClick_ShowHeroDetailStat();

	//UFUNCTION(BlueprintCallable, Category = UC_HeroManagement)
	//void OnClick_ToggleToSkill();

	void SetEnableHeadTracking(bool InEnable);



	/************************************************************************/
	/* Getter																*/
	/************************************************************************/
	//UCharacterBattleInfo* GetCharacterBattleInfo();
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroManagement|Ref")
	UButton*			Button_Social;

private:
	

	bool CanInvalidate(const FString& InHeroUD);
	

	void ClearData();

private:
	UPROPERTY()
	AActor*			LobbyCameraActor = nullptr;
	
	UPROPERTY()
	URGameInstance*	RGameInstance = nullptr;
	
	UPROPERTY()
	FString			CurrentHeroUD;





};