// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_KillNotify.h"

#include "GlobalIntegrated.h"



#include "UtilFunctionIntegrated.h"
#include "Network/HttpRequestClient.h"

void UUC_HUD_KillNotify::NativeConstruct()
{
	Super::NativeConstruct();

	PrepareWidget();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		CurGamePlayMode = UUtilFunctionLibrary::GetGamePlayModeEnum();
		if (CurGamePlayMode == EGamePlayModeTypeEnum::VE_BattleField ||
			CurGamePlayMode == EGamePlayModeTypeEnum::VE_Colosseum ||
			CurGamePlayMode == EGamePlayModeTypeEnum::VE_BattleGround ||
			CurGamePlayMode == EGamePlayModeTypeEnum::VE_GuildRaid ||
			CurGamePlayMode == EGamePlayModeTypeEnum::VE_Arena ||
			CurGamePlayMode == EGamePlayModeTypeEnum::VE_MirrorDungeon ||
			CurGamePlayMode == EGamePlayModeTypeEnum::VE_RaidKillBoss )
		{
			RGameInstance->BattleRecordManager->OnBattleRecord_UpdateKillCount.RemoveDynamic(this, &UUC_HUD_KillNotify::OnBattleRecord_UpdateKillCount);
			RGameInstance->BattleRecordManager->OnBattleRecord_UpdateKillCount.AddDynamic(this, &UUC_HUD_KillNotify::OnBattleRecord_UpdateKillCount);
		}

// 		if (CurGamePlayMode == EGamePlayModeTypeEnum::VE_BattleGround)
// 		{
// 			RGameInstance->EventManager->OnRealTimeModNotify_PointGet.RemoveDynamic(this, &UUC_HUD_KillNotify::ShowBattleGroundMessage);
// 			RGameInstance->EventManager->OnRealTimeModNotify_PointGet.AddDynamic(this, &UUC_HUD_KillNotify::ShowBattleGroundMessage);
// 		}
	}

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Kill_Message)
	{
		Kill_Message->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Kill_Count)
	{
		Kill_Count->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (BattleGroundMessage)
	{
		BattleGroundMessage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_HUD_KillNotify::NativeDestruct()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	
	{
		RGameInstance->BattleRecordManager->OnBattleRecord_UpdateKillCount.RemoveDynamic(this, &UUC_HUD_KillNotify::OnBattleRecord_UpdateKillCount);
//		RGameInstance->EventManager->OnRealTimeModNotify_PointGet.RemoveDynamic(this, &UUC_HUD_KillNotify::ShowBattleGroundMessage);
	}

	Super::NativeDestruct();
}

void UUC_HUD_KillNotify::OnBattleRecord_UpdateKillCount(AActor* DeadActor, AActor* FromActor)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	auto DeadActor_BaseProperty = Cast<IEntityBaseProperty>(DeadActor);
	if (DeadActor_BaseProperty == nullptr)
		return;
	auto FromActor_BaseProperty = Cast<IEntityBaseProperty>(FromActor);
	if (FromActor_BaseProperty == nullptr)
		return;

	if ((DeadActor_BaseProperty->GetCharacterCategory() != ECharacterCategoryTypeEnum::VE_Hero) ||
		(FromActor_BaseProperty->GetCharacterCategory() != ECharacterCategoryTypeEnum::VE_Hero))
		return;

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (Kill_Message)
	{
		Kill_Message->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	FText KillActorName;
	FText DeadActorName;
	bool IsMyTeamKiller = false;

	if (CurGamePlayMode == EGamePlayModeTypeEnum::VE_Arena ||
		CurGamePlayMode == EGamePlayModeTypeEnum::VE_Colosseum ||
		CurGamePlayMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
	{
		KillActorName = UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum::VE_Hero, FromActor_BaseProperty->GetCharacterID());
		DeadActorName = UUIFunctionLibrary::GetCharacterName(ECharacterCategoryTypeEnum::VE_Hero, DeadActor_BaseProperty->GetCharacterID());
	
		IsMyTeamKiller = RGameInstance->HttpClient->IsValidKID(FromActor_BaseProperty->GetUserKID());
	}
	else if (CurGamePlayMode == EGamePlayModeTypeEnum::VE_BattleField ||
		CurGamePlayMode == EGamePlayModeTypeEnum::VE_BattleGround ||
		CurGamePlayMode == EGamePlayModeTypeEnum::VE_GuildRaid ||
		CurGamePlayMode == EGamePlayModeTypeEnum::VE_RaidKillBoss)
	{
		FString KillActorString = RGameInstance->RealTimeModManager->GetPlayerNameByKID(FromActor_BaseProperty->GetUserKID());
		FString DeadActorString = RGameInstance->RealTimeModManager->GetPlayerNameByKID(DeadActor_BaseProperty->GetUserKID());
		KillActorName = FText::FromString(KillActorString);
		DeadActorName = FText::FromString(DeadActorString);

// 		IsMyTeamKiller = (RGameInstance->GameObjectMgr->GetMyGroupKey() == FromActor_BaseProperty->GetGroupKey()) ? true : false;
		IsMyTeamKiller = (UObjectFunctionLibrary::GetMyTeamKey() == FromActor_BaseProperty->GetGroupKey()) ? true : false;

	}

	if (IsValid(LeftSwitcher) && IsValid(RightSwitcher))
	{
		if (!IsMyTeamKiller)
		{
			LeftSwitcher->SetActiveWidgetIndex(0);
			RightSwitcher->SetActiveWidgetIndex(1);
		}
		else
		{
			LeftSwitcher->SetActiveWidgetIndex(1);
			RightSwitcher->SetActiveWidgetIndex(0);
		}
	}

	UObject* KillActorTextureRef = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, FromActor_BaseProperty->GetCharacterID());
	UUIFunctionLibrary::SetImageBrush(KillActorTextureRef, RightImage);

	UObject* DeadActorTextureRef = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, DeadActor_BaseProperty->GetCharacterID());
	UUIFunctionLibrary::SetImageBrush(DeadActorTextureRef, LeftImage);

	if (IsValid(TextBlock_LeftAccount) && IsValid(TextBlock_RightAccount))
	{
		TextBlock_LeftAccount->SetText(DeadActorName);
		TextBlock_RightAccount->SetText(KillActorName);

		if (!IsMyTeamKiller)
		{
			TextBlock_LeftAccount->SetColorAndOpacity(Color_Friend);
			TextBlock_RightAccount->SetColorAndOpacity(Color_Enemy);
		}
		else
		{
			TextBlock_LeftAccount->SetColorAndOpacity(Color_Enemy);
			TextBlock_RightAccount->SetColorAndOpacity(Color_Friend);
		}
	}

	//PlayAnimation(Notification_Kill);
	PlayAni("Notification_Kill");
	

	ShowKillCount();
}

void UUC_HUD_KillNotify::ShowKillCount(int32 KillCount)
{
	if (Kill_Count)
	{
		Kill_Count->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (IsValid(TextBlock_KillCount))
	{
		if (0 < KillCount)
		{
			TextBlock_KillCount->SetText(FText::AsNumber(KillCount));
			TextBlock_KillCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			TextBlock_KillCount->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//PlayAnimation(Count_Kill);
	PlayAni("Count_Kill");
}

void UUC_HUD_KillNotify::ShowBattleGroundMessage(AActor* TargetActor, FText msg)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (!IsValid(RGameInstance))
// 		return;

	auto TargetActor_BaseProperty = Cast<IEntityBaseProperty>(TargetActor);
	if (TargetActor_BaseProperty == nullptr)
		return;

	if (TargetActor_BaseProperty->GetCharacterCategory() != ECharacterCategoryTypeEnum::VE_Hero)
		return;

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (BattleGroundMessage)
	{
		BattleGroundMessage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

// 	bool IsMyTeamKiller = (RGameInstance->GameObjectMgr->GetMyGroupKey() == TargetActor_BaseProperty->GetGroupKey()) ? true : false;
	bool IsMyTeamKiller = (UObjectFunctionLibrary::GetMyTeamKey() == TargetActor_BaseProperty->GetGroupKey()) ? true : false;

	UObject* TargetHeroTextureRef = UUIFunctionLibrary::GetCharacterPortrait(ECharacterCategoryTypeEnum::VE_Hero, TargetActor_BaseProperty->GetCharacterID());
	UUIFunctionLibrary::SetImageBrush(TargetHeroTextureRef, BattleGroundImage);

	if (IsValid(SwitcherBattleGround))
	{
		if (IsMyTeamKiller)
		{
			SwitcherBattleGround->SetActiveWidgetIndex(0);
		}
		else
		{
			SwitcherBattleGround->SetActiveWidgetIndex(1);
		}
	}

	if (IsValid(TextBlock_Notify))
	{
		TextBlock_Notify->SetText(msg);
	}

	//PlayAnimation(BattleGround_Reward);
	PlayAni("BattleGround_Reward");
}

void UUC_HUD_KillNotify::PrepareWidget()
{

}
