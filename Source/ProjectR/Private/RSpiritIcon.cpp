// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RSpiritIcon.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Table/SkillResourceTableInfo.h"
#include "Table/SkillTableInfo_V2.h"

#include "Global/TableManager.h"
#include "GameInfo/PlayStateInfo.h"

#include "UtilFunctionIntegrated.h"
#include "UI/UI_Battle/RCampaignHUD_OneControl.h"

void URSpiritIcon::NativeConstruct()
{
	Super::NativeConstruct();

	/*Renderer Settings*/
	Renderer.variables = &Variables;
	Renderer.ParentWidget = this;
	/*Renderer Settings End*/

	if (Variables.Button_Click)
	{
		Variables.Button_Click->OnClicked.Clear();
		Variables.Button_Click->OnClicked.AddDynamic(this, &URSpiritIcon::OnClickIcon);
	}
}

void URSpiritIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void URSpiritIcon::OnClickIcon()
{
	auto CrewStateChanger = [=]() -> FHeroSlotInfo* {
		FHeroSlotInfo* BattleSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetBattleActiveCrewSlot(Variables.OwnerRole);
		FHeroSlotInfo* RestSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetRestActiveCrewSlot(Variables.OwnerRole);

		BattleSlot->CharacterCrewState = ECharacterCrewStateEnum::VE_Rest;
		RestSlot->CharacterCrewState = ECharacterCrewStateEnum::VE_Battle;

		return BattleSlot;
	};

	FHeroSlotInfo* RestSlot = CrewStateChanger();
	Renderer.Prepare(*RestSlot);

	URCampaignHUD_OneControl* IngameHUD = Cast<URCampaignHUD_OneControl>(UUIFunctionLibrary::GetInGameHUD());
	if (IngameHUD)
		IngameHUD->SpiritHelper.Update(Variables.OwnerRole);

}

void FRenderer_SpiritIcon::Prepare(FHeroSlotInfo slotinfo)
{
	variables->OwnerRole = slotinfo.CharacterRole;

	FCharacterTableInfo* CharacterTableInfo1 = UTableManager::GetInstancePtr()->GetCharacterRow(slotinfo.HeroIDName);
	if (CharacterTableInfo1)
	{
		FString SkillIDName = CharacterTableInfo1->SpiritPassive.ToString();
		FSkillTableInfo_V2* SpiritSkillinfo = UTableManager::GetInstancePtr()->GetSkillRow_V2(FName(*SkillIDName));
		if (SpiritSkillinfo)
		{
			FSkillResourceTableInfo* ResourceInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SpiritSkillinfo->SkillResourceID);
			if (ResourceInfo)
			{
				if (IsValid(variables->Image_SkillIcon))
					variables->Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(ResourceInfo->SkillIconRef));
			}

			UObject* Portrait = UUIFunctionLibrary::GetHeroRoundPortrait(slotinfo.HeroIDName);
			if (Portrait)
			{
				UUIFunctionLibrary::SetImageBrush(Portrait, variables->Image_Portrait);
				variables->PortraitOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
				variables->PortraitOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
 	else
		ParentWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void FRenderer_SpiritIcon::Render()
{

}




// void URSkillSet::SetSpiritSkillIcon(ECharacterRoleEnum InRole, bool InbFirst /*= false*/, bool _bInit/* = false*/)
// {
// 	URSkillIcon* _icon = nullptr;
// 
// 	if (InRole == ECharacterRoleEnum::VE_Crew1)
// 		_icon = Spirit1SkillIconSecond;
// 	else
// 		_icon = Spirit2SkillIconSecond;
// 
// 	if (IsValid(_icon))
// 	{
// 		FName PrevSkillName = NAME_None;
// 		TArray<FHeroSlotInfo> SlotArray = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray();
// 
// 		_icon->SetSkillIconState(ESkillIconState::VE_Normal);
// 
// 		for (size_t i = 0; i < SlotArray.Num(); ++i)
// 		{
// 			FHeroSlotInfo& HeroSlot = SlotArray[i];
// 			if (HeroSlot.CharacterRole != InRole)
// 				continue;
// 
// 			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroSlot.HeroIDName);
// 			FSpiritSkillTableInfo* SpiritSkill = UTableManager::GetInstancePtr()->GetSpiritSkill(CharacterTableInfo->SpiritPassive);
// 
// 			if (HeroSlot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
// 			{
// 				if (!_bInit)
// 					PrevSkillName = CharacterTableInfo->SpiritPassive;
// 			}
// 			else if (HeroSlot.CharacterCrewState == ECharacterCrewStateEnum::VE_Rest)
// 			{
// 				if (SpiritSkill)
// 				{
// 					SetSpiritEffect(InRole, true);
// 
// 					FSkillResourceTableInfo* ResourceInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(SpiritSkill->SkillResourceID);
// 					_icon->SetSkillIconImage(ResourceInfo);
// 
// 					UObject* Portrait = UUIFunctionLibrary::GetHeroRoundPortrait(HeroSlot.HeroIDName);
// 					UUIFunctionLibrary::SetImageBrush(Portrait, _icon->Image_Portrait);
// 					_icon->Portrait->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 				}
// 				else
// 				{
// 					_icon->SetSkillIconState(ESkillIconState::VE_Locked);
// 					UE_LOG(LogUI, Log, TEXT("Not Found SpiritSkill Data"));
// 				}
// 			}
// 		}
// 
// 		if (PrevSkillName != NAME_None)
// 		{
// 			_icon->StartSpiritCoolTime(PrevSkillName);
// 		}
// 	}
// }