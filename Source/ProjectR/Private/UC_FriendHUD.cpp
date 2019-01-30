// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_FriendHUD.h"

#include "Utils/GamePlayBluePrintFunction.h"
#include "Utils/UIFunctionLibrary.h"
#include "Utils/CharacterHelpFunctionLibrary.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "Character/Interfaces/EntityBaseProperty.h"

void UUC_FriendHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo->IsValidLowLevel())
		LevelActor = BaseStageInfo->GetLevelActor();

	/*Renderer Settings*/
	Renderer.variables = &variables;
	Renderer.Prepare();
	/**/


}

void UUC_FriendHUD::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_FriendHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (LevelActor.IsValid())
	{
		TMap<int32, FProtectNpcList> ProtectMap = LevelActor->GetProtectList();
		int32 iNumProtectNPC = INDEX_NONE;
		for (auto& Element : ProtectMap)
		{
			for (size_t i = 0; i < Element.Value.ActorList.Num(); ++i)
			{
				++iNumProtectNPC;
				if(iNumProtectNPC == variables.ProtectIdx)
					Renderer.Render(Element.Value.ActorList[i]);
			}
		}
	}
}

void FRenderer_FriendHUD::Prepare()
{
	if (variables->CanvasPanel->IsValidLowLevel())
		variables->CanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void FRenderer_FriendHUD::Render(AActor* TargetActor)
{
	if (variables->CanvasPanel->IsValidLowLevel())
		variables->CanvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	IEntityBaseProperty* prop = Cast<IEntityBaseProperty>(TargetActor);
	if (prop)
	{
		UObject* Portrait = UUIFunctionLibrary::GetHeroRhombusPortrait(prop->GetCharacterID());
		UUIFunctionLibrary::SetImageBrush(Portrait, variables->Image_Hero);
	}

	UBattleEntityComponent* BEC = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
	if (BEC)
	{
		float CurrentHP = BEC->GetHP();
		float MaxHP = BEC->GetMaxHP();
		float HPRatio = CurrentHP / MaxHP;
		if (variables->ProgressBar_HP)
			variables->ProgressBar_HP->SetPercent(HPRatio);
// 		if (variables->Text_HP)
// 			variables->Text_HP->SetText(FText::AsNumber(HPRatio * 100.f));
	}
	


}
