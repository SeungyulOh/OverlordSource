// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RToggleCameraIcon.h"

#include "GlobalIntegrated.h"


#include "UtilFunctionIntegrated.h"




void URToggleCameraIcon::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentToggleIndex = 0;

	if (IsValid(Switcher_Icon))
	{
		Switcher_Icon->SetActiveWidgetIndex(CurrentToggleIndex);		
	}
	if (IsValid(Button_CameraViewToggle))
	{
		Button_CameraViewToggle->OnClicked.RemoveDynamic(this, &URToggleCameraIcon::OnClick_Icon);
		Button_CameraViewToggle->OnClicked.AddDynamic(this, &URToggleCameraIcon::OnClick_Icon);
	}


}

void URToggleCameraIcon::OnClick_Icon()
{
	if( IsClickDelayed)
		return;

	IsClickDelayed = true;

	CurrentToggleIndex++;

	if( CurrentToggleIndex >= ToggleImageCount)
		CurrentToggleIndex = 0;

	if (IsValid(Switcher_Icon))
		Switcher_Icon->SetActiveWidgetIndex(CurrentToggleIndex);
	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	SetClickDelayTimer();
	
	if (RGameInstance)
		RGameInstance->EventManager->OnClickToggleCameraIcon.Broadcast(CurrentToggleIndex);
}

void URToggleCameraIcon::SetActiveIconIndex(int32 Index)
{
	CurrentToggleIndex = Index;

	if (CurrentToggleIndex >= ToggleImageCount)
		CurrentToggleIndex = 0;

	if (IsValid(Switcher_Icon))
		Switcher_Icon->SetActiveWidgetIndex(CurrentToggleIndex);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);	

	if (RGameInstance)
		RGameInstance->EventManager->OnClickToggleCameraIcon.Broadcast(CurrentToggleIndex);
}

void URToggleCameraIcon::SetClickDelayTimer()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance)
		RGameInstance->TimerManager->SetTimer(TimerHandle_ClickDelayed, this, &URToggleCameraIcon::CallbackClickDelayed, ClickDelayedSecond);
}

void URToggleCameraIcon::CallbackClickDelayed()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance)
		RGameInstance->TimerManager->ClearTimer(TimerHandle_ClickDelayed);

	IsClickDelayed = false;	

}
