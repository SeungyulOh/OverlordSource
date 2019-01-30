// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_InGame_Tutorial_Popup.h"
#include "Utils/UtilFunctionLibrary.h"
#include "GamePlayBluePrintFunction.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "Global/TableManager.h"
#include "Global/RGameInstance.h"
#include "Global/RGameUserSetting.h"
#include "Global/ForceTutorialManager.h"

#define NORMAL_TEXTURE_NUM 6
#define HARD_TEXTURE_NUM (NORMAL_TEXTURE_NUM + 1)
void UUP_InGame_Tutorial_Popup::NativeConstruct()
{
	Super::NativeConstruct();

	Init();

	if (Button_Close)
		Button_Close->OnClicked.AddDynamic(this, &UUP_InGame_Tutorial_Popup::OnClickClose);
	if (Button_Prev)
		Button_Prev->OnClicked.AddDynamic(this, &UUP_InGame_Tutorial_Popup::OnClickPrev);
	if (Button_Next)
		Button_Next->OnClicked.AddDynamic(this, &UUP_InGame_Tutorial_Popup::OnClickNext);
}

void UUP_InGame_Tutorial_Popup::NativeDestruct()
{
	if (Button_Close)
		Button_Close->OnClicked.RemoveDynamic(this, &UUP_InGame_Tutorial_Popup::OnClickClose);
	if (Button_Prev)
		Button_Prev->OnClicked.RemoveDynamic(this, &UUP_InGame_Tutorial_Popup::OnClickPrev);
	if (Button_Next)
		Button_Next->OnClicked.RemoveDynamic(this, &UUP_InGame_Tutorial_Popup::OnClickNext);

	Super::NativeDestruct();
}

void UUP_InGame_Tutorial_Popup::Init()
{
	UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	FName stageid = BaseStageInfo->StageID;
	pageIndex = 1;

	SetTextureData();

	if (stageid == TEXT("N_1_0"))
	{
		tutorial_btn_prev->SetVisibility(ESlateVisibility::Collapsed);
		Image_ForTexture->SetBrushFromTexture(NormalTextureArray[pageIndex - 1]);
	}
	else if (stageid == TEXT("H_1_6"))
	{
		tutorial_btn_prev->SetVisibility(ESlateVisibility::Collapsed);
		tutorial_btn_next->SetVisibility(ESlateVisibility::Collapsed);

		Image_ForTexture->SetBrushFromTexture(HardTexture);
	}
}
FName UUP_InGame_Tutorial_Popup::GetLocalTextureName(FGuideResourceTableInfo* InGuideTable)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	switch (RGameInstance->RGameUserSetting->LaguageType)
	{
	case ELanguageType::VE_KOR:
		return InGuideTable->KOR;
	case ELanguageType::VE_ENG:
		return InGuideTable->ENG;
	case ELanguageType::VE_CHN:
		return InGuideTable->CHN;
	case ELanguageType::VE_TWN:
		return InGuideTable->TWN;
	default:
		break;
	}

	return NAME_None;
}

void UUP_InGame_Tutorial_Popup::SetTextureData()
{
	NormalTextureArray.Empty();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	for (int32 i = 0; i < NORMAL_TEXTURE_NUM; i++)
	{
		UTexture2D* pNormalTexture = nullptr;
		FGuideResourceTableInfo* pGuideInfo = UTableManager::GetInstancePtr()->GetGuideResourceTableRow(FName(*FString::FromInt(i+1)));
		if (pGuideInfo)
		{
			pNormalTexture = UUtilFunctionLibrary::GetTexture2DFromTable(GetLocalTextureName(pGuideInfo));
		}

		if(pNormalTexture)
			NormalTextureArray.Emplace(pNormalTexture);
	}

	UTexture2D* pHardTexture = nullptr;
	FGuideResourceTableInfo* pGuideInfo = UTableManager::GetInstancePtr()->GetGuideResourceTableRow(FName(*FString::FromInt(HARD_TEXTURE_NUM)));
	pHardTexture = UUtilFunctionLibrary::GetTexture2DFromTable(GetLocalTextureName(pGuideInfo));
	if (pHardTexture)
		HardTexture = pHardTexture;
}

void UUP_InGame_Tutorial_Popup::OnClickClose()
{
	RGAMEINSTANCE(this)->ForceTutorialManager->OnExeNextNode();
}

void UUP_InGame_Tutorial_Popup::OnClickPrev()
{
	if (pageIndex == 2)
		tutorial_btn_prev->SetVisibility(ESlateVisibility::Collapsed);

	if (pageIndex > 1)
	{
		pageIndex--;
		Image_ForTexture->SetBrushFromTexture(NormalTextureArray[pageIndex - 1]);
	}
}

void UUP_InGame_Tutorial_Popup::OnClickNext()
{
	tutorial_btn_prev->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (pageIndex < 6)
	{
		pageIndex++;
		Image_ForTexture->SetBrushFromTexture(NormalTextureArray[pageIndex - 1]);
	}
	else if (pageIndex == 6)
	{
		OnClickClose();
	}
}
