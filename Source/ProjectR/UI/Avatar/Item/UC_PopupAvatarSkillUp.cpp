

#include "ProjectR.h"
#include "UC_PopupAvatarSkillUp.h"

#include "GameInfo/UserInfo.h"
#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"
#include "Network/PacketFunctionLibrary.h"

#include "UI/RWidgetManager.h"
#include "UI/Avatar/Item/UC_AvatarSkill.h"
#include "UI/Common/UC_CurrencyDisplay.h"

#include "Network/PacketFunctionLibrary.h"


#define AVATAR_LEVELMAX 5

void UUC_PopupAvatarSkillUp::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Close)
	{
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_PopupAvatarSkillUp::OnClickClose);
		Button_Close->OnClicked.AddDynamic(this, &UUC_PopupAvatarSkillUp::OnClickClose);
	}
	if (Button_Cancel)
	{
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_PopupAvatarSkillUp::OnClickClose);
		Button_Cancel->OnClicked.AddDynamic(this, &UUC_PopupAvatarSkillUp::OnClickClose);
	}
	if (Button_OK)
	{
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_PopupAvatarSkillUp::OnClickOK);
		Button_OK->OnClicked.AddDynamic(this, &UUC_PopupAvatarSkillUp::OnClickOK);
	}
}

void UUC_PopupAvatarSkillUp::NativeDestruct()
{
	if (Button_Close)
		Button_Close->OnClicked.RemoveDynamic(this, &UUC_PopupAvatarSkillUp::OnClickClose);
	if (Button_Cancel)
		Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_PopupAvatarSkillUp::OnClickClose);
	if (Button_OK)
		Button_OK->OnClicked.RemoveDynamic(this, &UUC_PopupAvatarSkillUp::OnClickOK);

	Super::NativeDestruct();
}

void UUC_PopupAvatarSkillUp::OnClickClose()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_PopupAvatarSkillUp::OnClickOK()
{
	// ..
	auto RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	if (bShortageMoney)
	{
		if (IsValid(RGameInstance->RWidgetManager))
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_up_ItemManage_tip07")));
		}
	}
	else if (AVATAR_LEVELMAX < NextLevel)
	{
		if(IsValid(RGameInstance->RWidgetManager))
		{
			UUIFunctionLibrary::ShowCommonPopupSplash(
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Notify")),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Account_Ability_0004")));
		}
	}
	else
	{
		UPacketFunctionLibrary::AVATAR_ABILITY_UP_RQ(Index, NextLevel);
	}
//	OnClickClose();
}

void UUC_PopupAvatarSkillUp::SetEmpty()
{

}

void UUC_PopupAvatarSkillUp::SetInfo(UUC_AvatarSkill* InSelectSkill)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;
// 
// 	Index = InSelectSkill->GetIndex();
// 
// 	auto Avatar = RGameInstance->UserInfo->GetAvatarData();
// 
// 	if (Avatar.ability.IsValidIndex(Index) == false)
// 		return;
// 
// 	int32 Level = Avatar.ability[Index];
// 	NextLevel = Level + 1;
// 
// // 	BeforeSkill;
// // 	AfterSkill;
// // 	BeforeSkillText;
// // 	AfterSkillText;
// // 	Text_NeedPoint;
// // 	NeedCurrency;
// 	if (Text_NeedPoint)
// 	{
// 		Text_NeedPoint->SetText(FText::AsNumber(1));
// 	}
// 	
// 
// 	auto Key = InSelectSkill->GetTableKey();
// 	auto Table = UTableManager::GetInstancePtr()->GetAvatarAbilityTableRow(Key);
// 	if (Table == nullptr)
// 		return;
// 
// 	//auto OwnerWidget = InSelectSkill->GetOwner();
// 	if (BeforeSkill)
// 	{
// 		BeforeSkill->SetIndex(Index);
// 		BeforeSkill->SetSkillImage(InSelectSkill->ImageKey, InSelectSkill->BGTint);
// 		//BeforeSkill->SetOwner(OwnerWidget);
// 		BeforeSkill->Refresh();
// 	}
// 
// 	if (AfterSkill)
// 	{
// 		AfterSkill->SetIndex(Index);
// 		AfterSkill->SetSkillImage(InSelectSkill->ImageKey, InSelectSkill->BGTint);
// 		//AfterSkill->SetOwner(OwnerWidget);
// 		AfterSkill->Refresh(true);
// 	}
// 	if (BeforeSkillText)
// 	{
// 		FFormatOrderedArguments Args;
// 		FText DescText;
// 		int32 PrevValue;
// 		switch (Level)
// 		{
// 		case 1: PrevValue = (Table->Ability_Value1); break;
// 		case 2: PrevValue = (Table->Ability_Value2); break;
// 		case 3: PrevValue = (Table->Ability_Value3); break;
// 		case 4: PrevValue = (Table->Ability_Value4); break;
// 		case 5: PrevValue = (Table->Ability_Value5); break;
// 		default: PrevValue = 0; break;
// 		}
// 
// 		Args.Emplace(PrevValue);
// 
// 		if (Args.Num() > 0)
// 		{
// 			DescText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, InSelectSkill->UIStringKey);
// 			DescText = FText::Format(DescText, Args);
// 		}
// 
// 		BeforeSkillText->SetText(DescText);
// 	}
// 
// 	if (AfterSkillText)
// 	{
// 		FFormatOrderedArguments Args;
// 		FText NextText;
// 		int32 NextValue = 0;
// 		switch (NextLevel)
// 		{
// 		case 1: NextValue = (Table->Ability_Value1); break;
// 		case 2: NextValue = (Table->Ability_Value2); break;
// 		case 3: NextValue = (Table->Ability_Value3); break;
// 		case 4: NextValue = (Table->Ability_Value4); break;
// 		case 5: NextValue = (Table->Ability_Value5); break;
// 		default: NextValue = 0; break;
// 		}
// 
// 		if (NextValue != 0)
// 		{
// 			Args.Emplace(NextValue);
// 
// 			if (Args.Num() > 0)
// 			{
// 				NextText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, InSelectSkill->UIStringKey);
// 				NextText = FText::Format(NextText, Args);
// 			}
// 
// 			AfterSkillText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 			AfterSkillText->SetText(NextText);
// 		}
// 		else
// 		{
// 			AfterSkillText->SetVisibility(ESlateVisibility::Hidden);
// 		}
// 	}
// 	
// 	if (NeedCurrency)
// 	{
// 		int32 UseGold = 0;
// 		switch (NextLevel)
// 		{
// 		case 1: UseGold = Table->Point1_gold; break;
// 		case 2: UseGold = Table->Point2_gold; break;
// 		case 3: UseGold = Table->Point3_gold; break;
// 		case 4: UseGold = Table->Point4_gold; break;
// 		case 5: UseGold = Table->Point5_gold; break;
// 		default: UseGold = 0; break;
// 		}
// 		NeedCurrency->SetCurrency(EWALLET_TYPE::GOLD, UseGold);
// 		
// 		bShortageMoney = (RGameInstance->UserInfo->GetUserCurrency(EREWARD_TYPE::GOLD) < UseGold);
// 	}
}