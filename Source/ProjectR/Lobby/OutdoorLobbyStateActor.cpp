#include "ProjectR.h"
#include "OutdoorLobbyStateActor.h"
#include "Global/ColosseumManager.h"
#include "UtilFunctionIntegrated.h"

void AOutdoorLobbyStateActor::EnterSublevel()
{
	Super::EnterSublevel();

	UMapFunctionLibrary::LoadSubLevel(FName("Lobby_Out"), FName("LoadLobbyOutLevelCompleted") , this);
	
}

void AOutdoorLobbyStateActor::LeaveSublevel()
{
	Super::LeaveSublevel();
	// Loose Tower actor

	UMapFunctionLibrary::UnLoadSubLevel(FName("Lobby_Out"), FName("OnLeaveSublevelComplete"), this);
}

void AOutdoorLobbyStateActor::BeginSublevel()
{
	check(OutdoorCameraActor.IsValid());
	DefaultOutdoorCameraTransform = OutdoorCameraActor->GetActorTransform();
}

void AOutdoorLobbyStateActor::EndSublevel()
{
	Super::EndSublevel();

	ResetTouchInteraction();
}


void AOutdoorLobbyStateActor::LoadLobbyOutLevelCompleted()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	OnEnterSublevelComplete.Broadcast();
	RGameInstance->ColosseumManager->openColosseumUI.Broadcast();
}

void AOutdoorLobbyStateActor::SetVisibilityAll(bool IsVisible)
{
	/*Super::SetVisibilityAll(!IsVisible);

	TArray<class ULevelStreaming*> LSArray = RGAMEINSTANCE(GEngine)->GetWorld()->StreamingLevels;
	for (size_t i = 0; i < LSArray.Num(); ++i)
	{
		if(LSArray[i]->bShouldBeLoaded == true)
			LSArray[i]->bShouldBeVisible = IsVisible;
	}*/

	/*ULevelStreaming* LobbyLevel = UGameplayStatics::GetStreamingLevel(RGAMEINSTANCE(this), FName("Lobby_Out"));
	if(LobbyLevel)
		LobbyLevel->bShouldBeVisible = IsVisible;*/
}

void AOutdoorLobbyStateActor::ChangeState(ETowerLobbySubstateEnum InSubstate)
{
	//SetVisibilityAll(true);

	if (InSubstate == CurrentSubstate)
	{
		ResetTouchInteraction();
		
		switch (CurrentOutdoorLobbyState)
		{
		case EOutdoorLobbyStateEnum::NormalView:
			UUtilFunctionLibrary::GetPlayerController(GetWorld())->SetViewTargetWithBlend(OutdoorCameraActor.Get());
			break;
		case EOutdoorLobbyStateEnum::PortraitMode:
			UUtilFunctionLibrary::GetPlayerController(GetWorld())->SetViewTargetWithBlend(OutdoorPortraitModeCameraActor.Get());
			break;
		default:
			ensure(false);
			break;
		}

		OnStateChanged.Broadcast();
		return;
	}


	CurrentSubstate = InSubstate;
	switch (CurrentSubstate)
	{
	case ETowerLobbySubstateEnum::NormalView:
		GoToNormalView();
		break;
	case ETowerLobbySubstateEnum::PortraitView:
		GoToPortraitMode();
		break;
	default:
		UE_LOG(LogLobby, Log, TEXT("AOutdoorLobbyStateActor::ChangeOutdoorLobbyState: unexpected CurrentOutdoorLobbyState"));
		return;
	}
}

void AOutdoorLobbyStateActor::GoToPortraitMode()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<AActor>(OutdoorPortraitModeCameraActor), 0.2, EViewTargetBlendFunction::VTBlend_Linear, 0.2);
	UKismetSystemLibrary::Delay(GetWorld(), 0.2, InitLatentActionInfo(FName("GoToPortraitModeComplete"), this, 1, 0));
}

void AOutdoorLobbyStateActor::GoToPortraitModeComplete()
{
	OnStateChanged.Broadcast();
}

void AOutdoorLobbyStateActor::GoToNormalView()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<AActor>(OutdoorCameraActor), 0.2, EViewTargetBlendFunction::VTBlend_Linear, 0.2);
	UKismetSystemLibrary::Delay(GetWorld(), 0.2, InitLatentActionInfo(FName("GoToNormalViewComplete"), this, 1, 0));
}

void AOutdoorLobbyStateActor::GoToNormalViewComplete()
{
	OnStateChanged.Broadcast();
}

void AOutdoorLobbyStateActor::RotateTower(float YawDelta)
{
	CurrentCameraOrbitYaw += YawDelta;
	CurrentCameraOrbitYaw = FMath::Clamp(CurrentCameraOrbitYaw, -(RGAMEINSTANCE(this)->GameEnvironmentVar.LeftAbsOrbitYaw), RGAMEINSTANCE(this)->GameEnvironmentVar.RightAbsOrbitYaw);

	UpdateOutdoorCameraTransform();
}

void AOutdoorLobbyStateActor::ScrollTower(float HeightDelta)
{
}

FName AOutdoorLobbyStateActor::GetCurrentLightingInFName()
{
	if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
		return FName("Lobby_Out_NightTime_Ls");
	else
		return FName("Lobby_Out_DayTime_Ls");
}

void AOutdoorLobbyStateActor::ResetTouchInteraction()
{
	CurrentCameraOrbitYaw = 0.0;

	UpdateOutdoorCameraTransform();
}

void AOutdoorLobbyStateActor::UpdateOutdoorCameraTransform()
{
	check(OutdoorCameraActor.IsValid());

	// Noes...
	FRotator OrbitRotator = FRotator(0.0, CurrentCameraOrbitYaw, 0.0);

	FVector Radial = DefaultOutdoorCameraTransform.GetLocation() - TowerPivotLocation;
	FVector RotatedRadial = OrbitRotator.RotateVector(Radial);
	FVector NewLocation = RotatedRadial + TowerPivotLocation;
	
	OutdoorCameraActor->SetActorLocation(FVector(NewLocation.X, NewLocation.Y, DefaultOutdoorCameraTransform.GetLocation().Z));
	OutdoorCameraActor->SetActorRotation(FRotator(DefaultOutdoorCameraTransform.Rotator().Pitch, DefaultOutdoorCameraTransform.Rotator().Yaw + CurrentCameraOrbitYaw, DefaultOutdoorCameraTransform.Rotator().Roll));
}

