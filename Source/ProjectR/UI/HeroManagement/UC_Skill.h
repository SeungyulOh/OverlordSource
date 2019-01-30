// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"
#include "Table/SkillResourceTableInfo.h"
#include "Network/PacketDataStructures.h"
#include "UC_Skill.generated.h"

class URGameInstance;
class USkillIcon;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EUISkillTypeEnum : uint8
{
	VE_HeroManagement		UMETA(DisplayName = "HeroManagement"),
	VE_Display				UMETA(DisplayName = "Display"),
};


/**
 * BP: UC_Skill
 */
UCLASS()
class PROJECTR_API UUC_Skill : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct()  override;
	void NativeDestruct()	override;
	void Init();
	void InvalidateByHeroUD(const FString& InHeroUD);
	void InvalidateByHeroID(const FName& InHeroID);

	bool GetSkillResourceInfoBySkillCategory(ESkillCategoryTypeEnum InSkillCategoryType, FSkillResourceTableInfo& InSkillResourceTableInfo);
	bool GetHeroSkillInfoBySkillCategory(ESkillCategoryTypeEnum InSkillCategoryType, FHERO_SKILL& InHeroSkillInfo);

public:
	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Skill)
	TArray<USkillIcon*>	SkillIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Skill)
	EUISkillTypeEnum	CurUISkillType = EUISkillTypeEnum::VE_HeroManagement;

private:
	void ClearHeroInfo();
	void ClearSkillInfoContainers();
	
	void InvalidateSkillInfo();
	void InvalidateSkilInfoByHeroUD();
	void InvalidateSkilInfoByHeroID();

	void InvalidateSkillInfoContainers(const FHERO_SKILL& HeroSkillElem);
	void InvaldateHeroSkillInfo(const FHERO_SKILL& HeroSkillElem);
	void InvalidateCurrentAndNextSkillInfo(const FHERO_SKILL& HeroSkillElem);
	void InvalidateCurrentSkillResourceInfo(const FHERO_SKILL& HeroSkillElem);
	
	void Refresh();

private:
	UPROPERTY()
	TArray<FHERO_SKILL>				HeroSkillInfos;
	UPROPERTY()
	TArray<FSkillResourceTableInfo> SkillResourceInfos;
	UPROPERTY()
	TArray<FWorkedSkillTableInfo>	SkillTableInfos;

	UPROPERTY()
	FString			CurrentHeroUD;
	UPROPERTY()
	FName			CurrentHeroID;

	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;
};
