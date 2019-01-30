// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HeroManagement.h"

#include "UI/UI_HeroManagement/OrbitCameraController.h"
#include "Object/CharacterSetCameraActor.h"

#include "UtilFunctionIntegrated.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/AnimInstance/AnimInstanceDefs.h"
#include "Character/AnimInstance/LobbyAnimInstance.h"
#include "GlobalIntegrated.h"



#include "GameInfo/RInventory.h"

#include "Table/LevelGrowth.h"
#include "Table/CharacterTableInfo.h"


void UUC_HeroManagement::NativeConstruct()
{
#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif

	Super::NativeConstruct();

	RGameInstance = RGAMEINSTANCE(this);


#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartTime;

	UE_LOG(LogGamePlay, Log, TEXT("%f ____NativeConstruct(%s)"),
		PrintTime, *(this->GetName()));
#endif
}

void UUC_HeroManagement::NativeDestruct()
{
	ClearData();

	Super::NativeDestruct();
}

/*void UUC_HeroManagement::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
}*/

/*void UUC_HeroManagement::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}*/

void UUC_HeroManagement::ClearData()
{
	LobbyCameraActor = nullptr;;
	RGameInstance = nullptr;
	CurrentHeroUD.Empty();


	//UICharacterBattleInfo = nullptr;
	//UICharacterTableInfo = nullptr;
	//UICharacterLevelGrowthTableInfo = nullptr;
	//UICharacterNextLevelGrowthTableInfo = nullptr;
}




void UUC_HeroManagement::InvalidateByHeroUD(const FString& InHeroUD)
{
#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif

	UE_LOG(LogUI, Log, TEXT("CHECK UUC_HeroManagement::InvalidateByHeroUD %s"), *InHeroUD);
	if (CanInvalidate(InHeroUD) == false)
	{
		UE_LOG(LogUI, Log, TEXT("CHECK UUC_HeroManagement::InvalidateByHeroUD CanInvalidate FAIL"));
		return;
	}

	CurrentHeroUD = InHeroUD;

	/*if (CharacterSetCameraActor == nullptr)
	{
		CamearaControl();
	}*/


#if LOG_TIME
	double StartTime2 = FPlatformTime::Seconds();
#endif


#if LOG_TIME
	double StartTime3 = FPlatformTime::Seconds();
#endif

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTimeAll = EndTime - StartTime;
	double PrintTime1 = StartTime2 - StartTime;
	double PrintTime2 = StartTime3 - StartTime2;
	double PrintTime3 = EndTime - StartTime3;
	//if (PrintTime > 0.05f)
	{
		UE_LOG(LogGamePlay, Log, TEXT("%f ____HeroManagement_InvalidateData(%f, %f, %f)"), 
			PrintTimeAll, PrintTime1, PrintTime2, PrintTime3);
	}
#endif
}

bool UUC_HeroManagement::CanInvalidate(const FString& InHeroUD)
{
	
	if (InHeroUD.IsEmpty() == true)
	{
		UE_LOG(LogUI, Log, TEXT("CHECK UUC_HeroManagement::CanInvalidate InHeroUD EMPTY"));
		return false;
	}

	return true;
}










/************************************************************************/
/* OnClick Event                                                        */
/************************************************************************/

//void UUC_HeroManagement::OnClick_ShowHeroExp()
//{
//	if (UUIFunctionLibrary::IsOwnedTargetHeroByHeroUD(this->GetWorld(), CurrentHeroUD) == false)
//	{
//		return;
//	}
//
//	if (WidgetSwitcher_Toggle == nullptr)
//	{
//		return;
//	}
//
//
//	WidgetSwitcher_Toggle->SetActiveWidgetIndex(1);
//
////	RGameInstance->EventManager->OnClick_ShowHeroExp_HeroManagement.Broadcast();
//}


/*void UUC_HeroManagement::OnClick_ShowHeroDetailStat()
{
	if (UUIFunctionLibrary::IsOwnedTargetHeroByHeroUD(this->GetWorld(), CurrentHeroUD) == false)
	{
		return;
	}

	if (WidgetSwitcher_Toggle == nullptr)
	{
		return;
	}

	WidgetSwitcher_Toggle->SetActiveWidgetIndex(1);
	
	RGameInstance->EventManager->OnClick_ShowHeroDetailStat_HeroManagement.Broadcast();
	
}*/

//void UUC_HeroManagement::OnClick_ToggleToSkill()
//{
//	if (WidgetSwitcher_Toggle == nullptr)
//	{
//		return;
//	}
//
//	WidgetSwitcher_Toggle->SetActiveWidgetIndex(0);
//
////	RGameInstance->EventManager->OnClick_ToggleToSkill_HeroManagement.Broadcast();
//}


void UUC_HeroManagement::SetEnableHeadTracking(bool InEnable)
{
	UOrbitCameraController* cameracontroller = UOrbitCameraController::Get();
	if (cameracontroller != nullptr)
	{
		cameracontroller->TurnOnHeadTracking(InEnable);
	}
}

/************************************************************************/
/* Getter																*/
/************************************************************************/
// UCharacterBattleInfo* UUC_HeroManagement::GetCharacterBattleInfo()
//{ 
//	return UICharacterBattleInfo;
//}