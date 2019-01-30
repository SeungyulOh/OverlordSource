// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"
//

#include "UI/UIDefs.h"
//#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
//

#include "HUDBase.h"

void AHUDBase::BeginPlay()
{
	Super::BeginPlay();

	RGameInstance = RGAMEINSTANCE(this);
	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
}

void AHUDBase::DrawHUD()
{
// 	if (RGameInstance->NewTutorialManager == nullptr)
// 	{
// 		bShowHUD = 0;
// 		return;
// 	}
// 
// 	if (RGameInstance->NewTutorialManager->IsDebugHud == false)
// 	{
// 		bShowHUD = 0;
// 		return;
// 	}
// 
// 	AHUDBase* HUDBase = Cast< AHUDBase >(BasePlayerController->GetHUD());
// 	if (IsValid(HUDBase))
// 	{
// 		if (RGameInstance->NewTutorialManager->SubHUDControl == nullptr)
// 			return;
// 
// 		int32 hicon = int32(EHudIcon::VE_HeroIcon1);
// 		float hiconcolor = 0;
// 		int32 sicon = int32(EHudIcon::VE_SkillSet);
// 		float siconcolor = 0;
// 		int32 aicon = int32(EHudIcon::VE_AttackIcon);
// 		
// 		int32 Max = int32(EHudIcon::VE_MovePad) - 1;
// 		for (int32 i = 0; i < Max; ++i)
// 		{
// 			FLinearColor ChangeColor(0, 0, 0);
// 			if (hicon <= i + 1 
// 				&& i + 1 <= hicon + 4)
// 			{
// 				ChangeColor = FLinearColor(1, hiconcolor, 0);
// 				hiconcolor += 0.25f;
// 			}
// 			else if(sicon <= i + 1
// 				&& i + 1 <= sicon + 3)
// 			{
// 				ChangeColor = FLinearColor(0, 1, siconcolor);
// 				siconcolor += 0.25f;
// 			}
// 			else if (aicon == i + 1)
// 			{
// 				ChangeColor = FLinearColor(0, 0, 1);
// 			}
// 			else
// 			{
// 				continue;
// 			}
// 
// 			FIntRect Rect = RGameInstance->NewTutorialManager->SubHUDControl->GetIconPos(EHudIcon(i + 1));
// 			HUDBase->DrawRect(ChangeColor, Rect.Min.X, Rect.Min.Y, Rect.Max.X - Rect.Min.X, Rect.Max.Y - Rect.Min.Y);
// 		}
// 	}
}