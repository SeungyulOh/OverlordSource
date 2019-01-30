// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/UserInfo.h"
#include "Table/LevelGrowth.h"
#include "UP_Popup_Avatar.h"
#include "UI/Common/UC_LevelUp.h"
#include "Global/TableManager.h"
//#include "UI/Result/UC_ResultEffect.h"
#include "Global/SoundManager.h"

void UUP_Popup_Avatar::NativeConstruct()
{
	Super::NativeConstruct();
	Text_After_AvatarLevel = UC_LevelUp->TextBlock_Level;

	PlayAni(TEXT("Popup_Appear"));

	USoundBase* Sound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, TEXT("SoundCue'/Game/Sound/ui/UP_Campaign_Campaign_Stage_Appear_Cue.UP_Campaign_Campaign_Stage_Appear_Cue'")));
	USoundManager::GetInstancePtr()->PlayComputeSound2D(GetWorld(), Sound);
	//UGameplayStatics::PlaySound2D(GetWorld(), Sound);

	RefreshUI();

	if (IsValid(VerticalBox_QuestGuide))
	{
		VerticalBox_QuestGuide->OnClicked.RemoveDynamic(this, &UUP_Popup_Avatar::OnClick_OK);
		VerticalBox_QuestGuide->OnClicked.AddDynamic(this, &UUP_Popup_Avatar::OnClick_OK);
	}
}

void UUP_Popup_Avatar::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUP_Popup_Avatar::RefreshUI()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	if ( !IsValid(RGameInstance->RWidgetManager) || !IsValid(RGameInstance->UserInfo))
		return;

	auto AvatarInfo = RGameInstance->UserInfo->Avatar;
// 	if (Text_NickName)
// 		Text_NickName->SetText(FText::FromString(AvatarInfo.nick));
	
	if (Text_After_AvatarLevel)
		Text_After_AvatarLevel->SetText(FText::AsNumber(AvatarInfo.level));
	
	FLevelGrowthTableInfo* tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(AvatarInfo.level - 1)));
	if (tableInfo == nullptr)
		return;
	
	if(BP_Text_Before_Stamina)
		BP_Text_Before_Stamina->SetText(FText::AsNumber(tableInfo->StaminaMax));
	
	if (BP_Text_Before_PVPStamina)
		BP_Text_Before_PVPStamina->SetText(FText::AsNumber(tableInfo->StaminaPVPMax));
	
	tableInfo = UTableManager::GetInstancePtr()->GetLevelGrowthRow(FName(*FString::FromInt(AvatarInfo.level)));
	if (tableInfo == nullptr)
		return;
	
	if (BP_Text_After_Stamina)
		BP_Text_After_Stamina->SetText(FText::AsNumber(tableInfo->StaminaMax));
		
	if (BP_Text_After_PVPStamina)
		BP_Text_After_PVPStamina->SetText(FText::AsNumber(tableInfo->StaminaPVPMax));

// 	if (IsValid(ResultEffect))
// 	{
// 		ResultEffect->ShowVictoryText(false);
// 		ResultEffect->PlayAni(TEXT("Victory"));
// 	}
}

void UUP_Popup_Avatar::OnClick_OK()
{
// 	auto RGameInstance = RGAMEINSTANCE(this);
// 	if ( !IsValid(RGameInstance->RWidgetManager))
// 		return;

	UUIFunctionLibrary::ShowAvatarLevelUpPopup(false);
}