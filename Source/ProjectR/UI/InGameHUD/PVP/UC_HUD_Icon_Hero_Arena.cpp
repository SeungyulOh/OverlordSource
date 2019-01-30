// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_Icon_Hero_Arena.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"
#include "UI/UI_Battle/Component/RBuffIcon.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Object/BaseCameraActor.h"

#include "UI/UI_Battle/InGameHeroUI/HeroUIforPvp.h"


void UUC_HUD_Icon_Hero_Arena::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();

// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( RGameInstance->EventManager)
// 	{
// 		RGameInstance->EventManager->OnDeadActor.AddDynamic(this, &UUC_HUD_Icon_Hero_Arena::OnDeadActor);
// 	}

	//FindChildWidgetsWithCertainName<URBuffIcon>(this, BuffIconArray, TEXT("BuffIcon_"));

	if (IsValid(PortraitButton))
	{
		PortraitButton->OnClicked.RemoveDynamic(this, &UUC_HUD_Icon_Hero_Arena::SetCameraFocusOnSelectHero);
		PortraitButton->OnClicked.AddDynamic(this, &UUC_HUD_Icon_Hero_Arena::SetCameraFocusOnSelectHero);
	}
}

void UUC_HUD_Icon_Hero_Arena::NativeDestruct()
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if ( RGameInstance->EventManager)
// 	{
// 		RGameInstance->EventManager->OnDeadActor.RemoveDynamic(this, &UUC_HUD_Icon_Hero_Arena::OnDeadActor);
// 	}

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(HeroActor.Get());
	if (SkillEntityComponent)
	{
		SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &UUC_HUD_Icon_Hero_Arena::SetCurrentSkillIcon);
		SkillEntityComponent->OnSetAbnormalIcon.RemoveDynamic(this, &UUC_HUD_Icon_Hero_Arena::CheckAbnormal);
	}

	Super::NativeDestruct();
}

void UUC_HUD_Icon_Hero_Arena::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HeroUIforPvp->UpdateIcon();
}

void UUC_HUD_Icon_Hero_Arena::Init(AActor* hero_actor, bool my_hero)
{
	checkf(hero_actor != nullptr, TEXT("UUC_HUD_Icon_Hero_Arena::Init Actor is Empty"));
	HeroActor = hero_actor;

	IEntityBaseProperty* EntityProperty = Cast< IEntityBaseProperty >(HeroActor.Get());
	checkf(EntityProperty != nullptr, TEXT("UUC_HUD_Icon_Hero_Arena::Init Actor is Not IEntityBaseProperty[%s]"), *(HeroActor->GetName()));
	FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(EntityProperty->GetCharacterCategory(), EntityProperty->GetCharacterID());
	HeroID = EntityProperty->GetCharacterID();
	MyHero = my_hero;
	auto battle_info = UCharacterHelpFunctionLibrary::GetBattleInfo(hero_actor);
	checkf(battle_info != nullptr, TEXT("UUC_HUD_Icon_Hero_Arena::Init Actor is Not GetBattleInfo[%s]"), *(HeroActor->GetName()));
	HeroCurrentHP = battle_info->GetHP();
	HeroMaxHP = battle_info->GetCharacterFinalProperty<float>(ECharacterPropertyTypeEnum::VE_MaxHP);

	HeroLevel = battle_info->Level;
	HeroPortrait = UUIFunctionLibrary::GetCharacterPortraitSprite(ECharacterCategoryTypeEnum::VE_Hero, HeroID);

	UpdateHeroHP();
}

void UUC_HUD_Icon_Hero_Arena::InitHero()
{
// 	SetImageHeroPortrait(HeroUIforPvp->Image_Hero);
// 	SetLevelText(HeroUIforPvp->Level_Text);
	HeroUIforPvp->SetHero(HeroActor.Get(), HeroID, HeroLevel);
//	SetTextkHeroName(HeroUIforPvp->NameText);
	SetImageHeroClass(Icon_Class);
	SetAbnormalIcon(BuffIconArray);
	SetSkillIcon(SkillIcon);
	SetTeamColor();
}

void UUC_HUD_Icon_Hero_Arena::SetLevelText(UTextBlock* text_wdg)
{
	if (text_wdg)
	{
		text_wdg->SetText(FText::AsNumber(HeroLevel));
	}
}

void UUC_HUD_Icon_Hero_Arena::SetImageHeroPortrait(UImage* img_wdg)
{
	if (img_wdg)
	{
		UUIFunctionLibrary::SetImageBrush(HeroPortrait, img_wdg);		
	}
}

void UUC_HUD_Icon_Hero_Arena::SetImageHeroClass(UImage* img_wdg)
{
	if (img_wdg)
	{
		EClassTypeEnum hero_class = UCharacterHelpFunctionLibrary::GetCharacterClassType(HeroActor.Get());
		UPaperSprite* Sprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(hero_class);
		UUIFunctionLibrary::SetImageBrush(Sprite, img_wdg);
	}
}

void UUC_HUD_Icon_Hero_Arena::SetAbnormalIcon(TArray<URBuffIcon*> icon_array)
{
	AbnormalIconList.Init(nullptr, icon_array.Num());
	AbnormalIconList = icon_array;

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(HeroActor.Get());
	if (IsValid(SkillEntityComponent))
	{
		SkillEntityComponent->OnSetAbnormalIcon.RemoveDynamic(this, &UUC_HUD_Icon_Hero_Arena::CheckAbnormal);
		SkillEntityComponent->OnSetAbnormalIcon.AddDynamic(this, &UUC_HUD_Icon_Hero_Arena::CheckAbnormal);
	}
}

void UUC_HUD_Icon_Hero_Arena::SetTextkHeroName(UTextBlock* textblock_wdg)
{
	if (textblock_wdg && HeroActor.IsValid())
	{
		FText NameText = UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum::VE_Hero, HeroID);
		textblock_wdg->SetText(NameText);
	}
}

void UUC_HUD_Icon_Hero_Arena::SetSkillIcon(URSkillIcon* skill_icon)
{
	if (skill_icon)
	{
		UseSkillIcon = skill_icon;
		UseSkillIcon->SetOnlyDisplay(true);
		UseSkillIcon->SetVisibility(ESlateVisibility::Collapsed);

		OwnSkillList.Init(0, 3);
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(HeroActor.Get());
		if (IsValid(SkillEntityComponent))
		{
			OwnSkillList[0] = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill01);
			OwnSkillList[1] = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
			OwnSkillList[2] = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);

			SkillEntityComponent->OnNotifySkillCasting.RemoveDynamic(this, &UUC_HUD_Icon_Hero_Arena::SetCurrentSkillIcon);
			SkillEntityComponent->OnNotifySkillCasting.AddDynamic(this, &UUC_HUD_Icon_Hero_Arena::SetCurrentSkillIcon);
		}
	}
}

void UUC_HUD_Icon_Hero_Arena::SetTeamColor()
{
//	HeroUIforPvp->SetTeamColorSwitcherIndex(IsMyHeroIcon() ? 0 : 1);
}

void UUC_HUD_Icon_Hero_Arena::SetCameraFocusOnSelectHero()
{
	ABasePlayerController* controller = UUtilFunctionLibrary::GetBasePlayerController(GetWorld());
	if (controller && HeroActor.IsValid())
	{
		ABaseCameraActor* camera = controller->GetCameraActor();
		if (camera)
		{
			if (false == camera->IsDynamicCamera)
			{
				camera->SetCameraMoveEnable(true);
				camera->ShowTargetEffect(false);
				if (HeroActor.IsValid())
				{
					FTransform TargetTransform = HeroActor->GetTransform();
					camera->ZoomInAndOut(1000.0f, false);
					camera->TransformStageCamera(TargetTransform, 0.5f, 0.5f);
					camera->SetOwnerCharacter(HeroActor.Get());
				}
			}
		}
	}
}

//void UUC_HUD_Icon_Hero_Arena::BP_UpdateHeroHP()
//{
//	UpdateHeroHP();
//}
//
//void UUC_HUD_Icon_Hero_Arena::BP_DeadHero()
//{
//	Image_Death->SetVisibility(ESlateVisibility::HitTestInvisible);
//}

void UUC_HUD_Icon_Hero_Arena::SetProgressHP(UProgressBar* hp_prog)
{
	if (hp_prog)
	{
		float percent = 0.0f;
		if (0 < HeroMaxHP)
		{
			percent = float(HeroCurrentHP) / float(HeroMaxHP);
		}
		hp_prog->SetPercent(percent);
	}
}

void UUC_HUD_Icon_Hero_Arena::UpdateHeroHP()
{
	if(HeroActor.IsValid())
	{
		auto battle_info = UCharacterHelpFunctionLibrary::GetBattleInfo(HeroActor.Get());
		if (battle_info)
		{
			HeroCurrentHP = battle_info->GetHP();
		}
	}

	//BP_UpdateHeroHP();
//	SetProgressHP(HeroUIforPvp->HpProgerssBar);
}

// void UUC_HUD_Icon_Hero_Arena::OnDeadActor(AActor* DeadActor, AActor* FromActor)
// {
// 	if (HeroActor == DeadActor)
// 	{
// 		Image_Death->SetVisibility(ESlateVisibility::HitTestInvisible);
// 	}
// }

void UUC_HUD_Icon_Hero_Arena::SetCurrentSkillIcon(AActor* InCaster, UISkill_V2* UseSkill)
{
	if (UseSkillIcon && HeroActor == InCaster)
	{
		if (IsValid(UseSkill))
		{
			FSkillTableInfo_V2* SkillInfo = UseSkill->GetSkillInfo();
			FSkillResourceTableInfo* SkillResourceInfo = UseSkill->GetSkillResourceInfo();
			if (SkillInfo && SkillResourceInfo)
			{
				UseSkillIcon->SetSkillIconImage(SkillResourceInfo);
				UseSkillIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
				UseSkillIcon->SetSkillIconState(ESkillIconState::VE_Normal);

				if (SkillIconTimerHandler.IsValid())
				{
					HeroActor->GetWorldTimerManager().ClearTimer(SkillIconTimerHandler);
				}
				HeroActor->GetWorldTimerManager().SetTimer(SkillIconTimerHandler, this, &UUC_HUD_Icon_Hero_Arena::HideSkillIcon, 1.5f);
			}
			else
			{
				UseSkillIcon->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UUC_HUD_Icon_Hero_Arena::CheckAbnormal(EAbnormalSlotTypeEnum InSlotType, bool InSet)
{
	for (auto Icon : AbnormalIconList)
	{
		if(nullptr == Icon)
			continue;

		if (InSet)
		{
			if (Icon->IsActive())
			{
				if (Icon->IsTypeCheck(InSlotType))
				{
					break;
				}
				continue;
			}

			Icon->SetAbnormalState(InSlotType);
			break;
		}
		else
		{
			if (Icon->IsTypeCheck(InSlotType))
			{
				Icon->SetAbnormalState(EAbnormalSlotTypeEnum::VE_None);
				break;
			}
		}
	}
}

void UUC_HUD_Icon_Hero_Arena::HideSkillIcon()
{
	if (SkillIconTimerHandler.IsValid())
	{
		HeroActor->GetWorldTimerManager().ClearTimer(SkillIconTimerHandler);
	}

	if (UseSkillIcon)
	{
		UseSkillIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_HUD_Icon_Hero_Arena::PlayChildIconAni(FString name, bool bReverse /* = false */)
{
	HeroUIforPvp->PlayAni(name, bReverse);
	HeroUIforPvp->SetEnemyHPColor();
}