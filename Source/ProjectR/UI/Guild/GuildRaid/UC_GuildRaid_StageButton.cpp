// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_GuildRaid_StageButton.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"

void UUC_GuildRaid_StageButton::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUC_GuildRaid_StageButton::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_GuildRaid_StageButton::SetStageInfo(int32 InStageNO, bool InLocked)
{
	StageNumber = InStageNO;

	if (Text_StageLevel)
		Text_StageLevel->SetText(FText::FromString(FString::FromInt(InStageNO)));

	SetButtonLockState(InLocked);
}

void UUC_GuildRaid_StageButton::SetButtonLockState(bool InLocked)
{
	bLocked = InLocked;

	if (Overlay_LockPanel)
	{
		if (InLocked)
			Overlay_LockPanel->SetVisibility(ESlateVisibility::Visible);
		else
			Overlay_LockPanel->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUC_GuildRaid_StageButton::SelectStage(bool bSelected)
{
	if (Image_SelectedStage)
	{
		if (bSelected)
			Image_SelectedStage->SetVisibility(ESlateVisibility::Visible);
		else
			Image_SelectedStage->SetVisibility(ESlateVisibility::Hidden);
	}

	//SetStageBaseIcon(bSelected);

}

void UUC_GuildRaid_StageButton::InitButton(int32 InDifficulty)
{
	StageNumber = InDifficulty;

	FName textureName = *(FText::Format(FText::FromString("Btn_GuildRaidStage0{0}_Default_Sprite"), FText::AsNumber(InDifficulty)).ToString());
	UUIFunctionLibrary::SetImageBrush(UUIFunctionLibrary::GetIconImageFromTable(textureName), Image_StageBaseIcon);

	if(IsValid(Text_StageLevel))
		Text_StageLevel->SetText(FText::AsNumber(InDifficulty));
}

void UUC_GuildRaid_StageButton::OnClickStage()
{
	if (bLocked)
		return;

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( IsValid(RGameInstance->EventManager))
	{
		RGameInstance->EventManager->OnSelectDifficulty.Broadcast(StageNumber - 1);
	}
}

void UUC_GuildRaid_StageButton::SetStageBaseBrushFromTexture(UTexture2D* Texture, bool bMatchSize/* = false*/)
{
	Image_StageBaseIcon->SetBrushFromTexture(Texture, bMatchSize);
}