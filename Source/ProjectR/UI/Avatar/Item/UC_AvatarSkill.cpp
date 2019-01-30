// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_AvatarSkill.h"

#include "GlobalIntegrated.h"
#include "GameInfo/UserInfo.h"

#include "UtilFunctionIntegrated.h"
#include "UI/Avatar/SubPage/UP_AvatarAbility.h"

void UUC_AvatarSkill::NativeConstruct()
{
	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);

	if (Button_Click)
	{
		Button_Click->OnClicked.RemoveDynamic(this, &UUC_AvatarSkill::ClickIcon);
		Button_Click->OnClicked.AddDynamic(this, &UUC_AvatarSkill::ClickIcon);
	}

	SetSkillImage(ImageKey, BGTint);

// 	auto Image = UUIFunctionLibrary::GetIconImageFromTable(ImageKey);
// 	UUIFunctionLibrary::SetImageBrush(Image, Image_SkillIcon);

// 	if (Image_BG)
// 	{
// 		Image_BG->Brush.TintColor = BGTint;
// 	}
}

void UUC_AvatarSkill::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_AvatarSkill::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (Image_BG)
	{
		Image_BG->Brush.TintColor = BGTint;
	}
}

void UUC_AvatarSkill::Refresh(bool bNextLevel)
{
	SetSkill(bNextLevel);
}

void UUC_AvatarSkill::SetSkill(bool bNextLevel)
{
	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
	int32 nNext = bNextLevel ? 1 : 0;
	if (Avatar.ability.IsValidIndex(Index))
	{
		if (Text_CurrentValue)
		{
			int32 UseValue = Avatar.ability[Index];
			FText Text;
			if (UseValue < 5)
				Text = FText::AsNumber(UseValue + nNext);
			else
				Text = FText::AsNumber(UseValue);
			Text_CurrentValue->SetText(Text);
		}

		if (Text_MaxValue)
		{
			FText Text = FText::AsNumber(5);
			Text_MaxValue->SetText(Text);
		}
	}
}

void UUC_AvatarSkill::SetSkillImage(FName InImageKey, FSlateColor InBGTint)
{
	ImageKey = InImageKey;
	auto Image = UUIFunctionLibrary::GetIconImageFromTable(InImageKey);
	UUIFunctionLibrary::SetImageBrush(Image, Image_SkillIcon);
	BGTint = InBGTint;
	if (Image_BG)
	{
		Image_BG->Brush.TintColor = InBGTint;
	}
}

void UUC_AvatarSkill::ClickIcon()
{
	if (OwnerWidget)
	{
		OwnerWidget->SelectIndex(Index);
	}
	//ToggleSelected();
}

void UUC_AvatarSkill::ToggleSelected()
{
	IsClick = !(IsClick);
	if (Overlay_Select)
	{
		if (IsClick)
		{
			Overlay_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			Overlay_Select->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
