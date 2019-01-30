// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/HeroManagement/UC_HeroInfoStat.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"

#include "Table/CharacterTableInfo.h"

#include "UC_HeroStatMgr_Info.generated.h"


UCLASS()
class PROJECTR_API UUC_HeroStatMgr_Info : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
	void Release();

	void SetStatInfo(UCharacterBattleInfo* InUIBattleInfo);

	UFUNCTION()
	void OnClick_Close();
protected:
private:

//	¸É¹ö º¯¼ö
public:
	UPROPERTY()
	TArray<UUC_HeroInfoStat*>		HeroStat;
	UPROPERTY()
	UButton*						Button_Close = nullptr;
protected:
private:
	FString							m_CurHeroUD;
	FHERO*							m_Hero;
	UCharacterBattleInfo*			m_UIBattleInfo;
	FCharacterTableInfo*			m_UITableInfo;
};