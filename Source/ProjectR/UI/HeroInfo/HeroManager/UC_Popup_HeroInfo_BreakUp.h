// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"

#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"

#include "Table/CharacterTableInfo.h"

#include "UC_Popup_HeroInfo_BreakUp.generated.h"


UCLASS()
class PROJECTR_API UUC_Popup_HeroInfo_BreakUp : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void Init();
	void Release();
	void SetInfo(FString InCurHeroUD);

	UFUNCTION()
	void OnClick_OK();
	UFUNCTION()
	void OnClick_Close();
protected:
private:

//	¸É¹ö º¯¼ö
public:

	UPROPERTY()
	URenewal_BaseHeroIcon*			Icon_Hero		=	nullptr;
	UPROPERTY()
	UTextBlock*						Text_Name		=	nullptr;
	
	
	UPROPERTY()
	UButton*						Button_OK		=	nullptr;
	UPROPERTY()
	UButton*						Button_Cancel	=	nullptr;
	UPROPERTY()
	UButton*						Button_Close	=	nullptr;
protected:
private:
	FString CurHeroUD;
};