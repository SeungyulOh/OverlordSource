// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CoreMinimal.h"
#include "Engine/InputDelegateBinding.h"
#include "GlobalIntegrated.h"

//#include "Character/BasePlayerController.h"
//#include "Controllers/DPadController.h"
#include "UtilFunctionIntegrated.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

#include "InputManager.h"

UInputManager::UInputManager()
{
	InitDefault();
}

UInputManager::UInputManager(const FObjectInitializer& ObjectInitializer)
{	
	InitDefault();
}

void UInputManager::InitDefault()
{
	IsSetupInputBinding		= false;
	IsEnableInputComponent	= false;
	CurrentGameStateType	= EGameStateEnum::VE_None;

	bBlockInput		= false;
	IsDrag			= false;
	IsPinch			= false;

	ClickToleranceDist = 100.f;
	PinchCancelToleranceDist = 100.f;

	TouchInfoMap.Empty();

	
}



void UInputManager::Tick(float DeltaTime)
{
	if (!InputComponent)
	{
		if (RGameInstance == nullptr) return;

#if LOG_INPUT
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGameStateEnum"), true);
		UE_LOG(LogInputMgr, Log, TEXT("Input Manager Current GameState  : %s,    Target Gamestate : %s"), 
			*(EnumPtr->GetEnumName((int32)CurrentGameStateType)), *(EnumPtr->GetEnumName((int32)RGameInstance->GetCurrentStateType())));
#endif

		if (InputComponent)
		{
			InputComponent->TouchBindings.Empty();
			InputComponent = nullptr;
		}

		EnableInputComponent();
		SetBindInputComponent();
	}
}

bool UInputManager::IsTickable() const { return true;}

TStatId UInputManager::GetStatId() const { 	return Super::GetStatID(); }

bool UInputManager::IsTickableInEditor() const { return false; }


void UInputManager::EnableInputComponent()
{
	auto World = RGameInstance->GetWorld();
	if (World)
	{
		auto PlayerController = World->GetFirstPlayerController();

		ABasePlayerController* BasePC = Cast<ABasePlayerController>(PlayerController);
		if (!BasePC)
			return;

		if (PlayerController)
		{
			InputComponent = PlayerController->InputComponent;
			//DPadController = Cast<ADPadController>(PlayerController);
		}

		if (InputComponent)
		{
			CurrentGameStateType = RGameInstance->GetCurrentStateType();
#if LOG_INPUT
			UE_LOG(LogInputMgr, Log, TEXT("First Player Controller Name in World  : %s"), *RGameInstance->GetWorld()->GetFirstPlayerController()->GetName());
#endif
		}
		else
		{
#if LOG_INPUT
			UE_LOG(LogInputMgr, Log, TEXT("InputComponent in InputManager failed Inputcomp by FisrstPlayerController in World   "));
#endif
		}
	}
}

void UInputManager::SetBindInputComponent()
{
	if (InputComponent)
	{
		InputComponent->BindTouch(IE_Pressed, this, &UInputManager::CallbackInputTouchBegin);
		InputComponent->BindTouch(IE_Repeat, this, &UInputManager::CallbackInputTouchOver);
		InputComponent->BindTouch(IE_Released, this, &UInputManager::CallbackInputTouchEnd);

		InputComponent->BindAxis(FName(TEXT("MoveUpDown")), this, &UInputManager::CallbackAxis_MoveUpDown);
		InputComponent->BindAxis(FName(TEXT("MoveLeftRight")), this, &UInputManager::CallbackAxis_MoveLeftRight);
		InputComponent->BindAxis(FName(TEXT("SkillUpDown")), this, &UInputManager::CallbackAxis_SkillUpDown);
		InputComponent->BindAxis(FName(TEXT("SkillLeftRight")), this, &UInputManager::CallbackAxis_SkillLeftRight);

		/*InputComponent->BindAction(FName(TEXT("ActiveSkill_1")), IE_Pressed, this, &UInputManager::CallbackSkillAction_Pressed);
		InputComponent->BindAction(FName(TEXT("ActiveSkill_2")), IE_Pressed, this, &UInputManager::CallbackSkillAction_Pressed);
		InputComponent->BindAction(FName(TEXT("ActiveSkill_3")), IE_Pressed, this, &UInputManager::CallbackSkillAction_Pressed);

		InputComponent->BindAction(FName(TEXT("ActiveSkill_1")), IE_Released, this, &UInputManager::CallbackSkillAction_Released);
		InputComponent->BindAction(FName(TEXT("ActiveSkill_2")), IE_Released, this, &UInputManager::CallbackSkillAction_Released);
		InputComponent->BindAction(FName(TEXT("ActiveSkill_3")), IE_Released, this, &UInputManager::CallbackSkillAction_Released);

		InputComponent->BindAction(FName(TEXT("ActiveSkill_1")), IE_Repeat, this, &UInputManager::CallbackSkillAction_Repeat);
		InputComponent->BindAction(FName(TEXT("ActiveSkill_2")), IE_Repeat, this, &UInputManager::CallbackSkillAction_Repeat);
		InputComponent->BindAction(FName(TEXT("ActiveSkill_3")), IE_Repeat, this, &UInputManager::CallbackSkillAction_Repeat);*/
	}
}



void UInputManager::CallbackInputTouchBegin(ETouchIndex::Type TouchIndex, FVector Location)
{
		
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		int32 iCount = 0;
		for (auto& Element : TouchInfoMap)
		{
			if (Element.Value.StartPos.X > ViewportSize.X * 0.5f)
				iCount++;
		}

		//if user trying to touch skill button with two finger, then block
		if (iCount > 0)
			return;
	}

	//UE_LOG(LogInputMgr, Log, TEXT("UInputManager::CallbackInputTouchBegin 1 TouchIndex %d TouchInfos Count %d"), (int32)TouchIndex, TouchInfoMap.Num())
	//FString Msg = FString::Printf(TEXT("UInputManager Touch Begin Event => Touch Index : %d"), (int32)TouchIndex);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Msg);

		
	if (TouchInfoMap.Num() == 0)
		IsDrag = IsPinch = false;
	

	if (!TouchInfoMap.Contains(TouchIndex))
	{		
		FInputTouchInfo TouchInfo;
		TouchInfoMap.Emplace(TouchIndex, TouchInfo);
		TouchInfoMap[TouchIndex].SetData(true, false, Location, Location, Location);
	}
	else
	{
		TouchInfoMap[TouchIndex].SetData(true, false, Location, Location, Location);
	}	

	OnInputTouchBeginDelegate.Broadcast(TouchIndex, Location);

	
}


void UInputManager::CallbackInputTouchOver(ETouchIndex::Type TouchIndex, FVector Location)
{	

	if (!TouchInfoMap.Contains(TouchIndex))
		return;
	
	if (TouchInfoMap.Contains(TouchIndex))
	{
		TouchInfoMap[TouchIndex].IsRepeat = true;
		TouchInfoMap[TouchIndex].LastPos = TouchInfoMap[TouchIndex].CurrentPos;
		TouchInfoMap[TouchIndex].CurrentPos = Location;
	}
	
	if (TouchInfoMap.Num() >= 2)
	{	
		OnProcessPinch();
	}
	else
	{
		OnProcessDrag();
	}

	OnInputTouchOverDelegate.Broadcast(TouchIndex, Location);
}


void UInputManager::CallbackInputTouchEnd(ETouchIndex::Type TouchIndex, FVector Location)
{	
	if (!TouchInfoMap.Contains(TouchIndex))
		return;
	
	if (TouchInfoMap.Num() == 1 && TouchInfoMap.Contains(TouchIndex ) )
	{
		if (IsDrag)
		{
			OnInputDragEndDelegate.Broadcast(TouchIndex, Location);
		}
		else if (IsPinch)
		{
			OnInputPinchEndDelegate.Broadcast(TouchIndex, Location);
		}
		else
		{
			OnInputTouchClick(Location);
		}
	}

	IsDrag = false;
	IsPinch = false;

	if (TouchInfoMap.Contains(TouchIndex))
	{
		TouchInfoMap.Remove(TouchIndex);
	}

	if (IsCheckDragState())
	{
		TouchInfoMap[ETouchIndex::Touch1].StartPos = TouchInfoMap[ETouchIndex::Touch1].CurrentPos;
		TouchInfoMap[ETouchIndex::Touch1].LastPos = TouchInfoMap[ETouchIndex::Touch1].CurrentPos;
	}

	OnInputTouchEndDelegate.Broadcast(TouchIndex, Location);

	

		
}


void UInputManager::OnProcessDrag()
{
	if (!IsCheckDragState())
		return;
	
	float dist = FVector::Dist(TouchInfoMap[ETouchIndex::Touch1].StartPos, TouchInfoMap[ETouchIndex::Touch1].CurrentPos);

	if(dist < ClickToleranceDist)
	{
		//UE_LOG(LogInputMgr, Log, TEXT("UInputManager::OnProcessDrag - Cancel, Delta Dist  = %f"), dist);
		return;
	}	

	IsDrag			= true;
	FDragInfo info	= SetDragInfo(TouchInfoMap[ETouchIndex::Touch1]);
	OnInputDragDelegate.Broadcast( info);
}


FDragInfo UInputManager::SetDragInfo(FInputTouchInfo TouchInfo)
{
	FDragInfo info;
	info.StartPos		= TouchInfo.StartPos;
	info.LastPos		= TouchInfo.LastPos;
	info.CurrentPos		= TouchInfo.CurrentPos;
	info.DeltaDist		= FVector::Dist(TouchInfo.LastPos, TouchInfo.CurrentPos);
	info.TotalDeltaDist = FVector::Dist(TouchInfo.StartPos, TouchInfo.CurrentPos);

	return info;

}

void UInputManager::OnProcessPinch()
{
	if (!IsCheckPinchState())
	{
		return;
	}

	float prevDist = FVector::Dist(TouchInfoMap[ETouchIndex::Touch1].StartPos, TouchInfoMap[ETouchIndex::Touch2].StartPos);
	float curDist = FVector::Dist(TouchInfoMap[ETouchIndex::Touch1].CurrentPos, TouchInfoMap[ETouchIndex::Touch2].CurrentPos);

	if (FMath::Abs(prevDist - curDist) < PinchCancelToleranceDist)
	{
		//	UE_LOG(LogInputMgr, Log, TEXT("UInputManager:: OnProcessPinch Ignor Event "));
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("UInputManager Pinch Event Cancel"));
		return;
	}	

	FPinchInfo Pinch = SetPinchInfo(TouchInfoMap[ETouchIndex::Touch1], TouchInfoMap[ETouchIndex::Touch2]);
	OnInputPinchDelegate.Broadcast(Pinch);
	
}


FPinchInfo UInputManager::SetPinchInfo(FInputTouchInfo Touch1, FInputTouchInfo Touch2)
{
	FPinchInfo PinchInfo;

	FDragInfo info1;
	info1.StartPos = Touch1.StartPos;
	info1.LastPos = Touch1.LastPos;
	info1.CurrentPos = Touch1.CurrentPos;
	info1.DeltaDist = FVector::Dist(Touch1.LastPos, Touch1.CurrentPos);
	info1.TotalDeltaDist = FVector::Dist(Touch1.StartPos, Touch1.CurrentPos);

	FDragInfo info2;
	info2.StartPos = Touch2.StartPos;
	info2.LastPos = Touch2.LastPos;
	info2.CurrentPos = Touch2.CurrentPos;
	info2.DeltaDist = FVector::Dist(Touch2.LastPos, Touch2.CurrentPos);
	info2.TotalDeltaDist = FVector::Dist(Touch2.StartPos, Touch2.CurrentPos);

	PinchInfo.FingerIndex1 = info1;
	PinchInfo.FingerIndex2 = info2;
	PinchInfo.BothFingerDist = FVector::Dist(info1.CurrentPos, info2.CurrentPos);	

	float LastDist = FVector::Dist( info1.LastPos, info2.LastPos);
	float CurDist = FVector::Dist(info1.CurrentPos, info2.CurrentPos);

	PinchInfo.DeltaDist = CurDist - LastDist;
	

	return PinchInfo;

}

void UInputManager::OnInputTouchClick(FVector Location)
{
	//UE_LOG(LogInputMgr, Log, TEXT("UInputManager:: Click!!!!!! "));
	OnInputClickDelegate.Broadcast(Location);
}

void UInputManager::CallbackAxis_MoveUpDown(float AxisValue)
{
	/*AActor* Leader = RGAMEINSTANCE(this)->PlayStateInfo->GetOwnerCharacter(ROLE_LEADER_INDEX);
	if (!IsValid(Leader))
		return;*/

	ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
	if (!IsValid(BasePC))
		return;

	DPadMoveValue.X = AxisValue;
	
	OnInputAxisValueDelegate.Broadcast(EInputCommandTypeEnum::VE_LeftAxis, DPadMoveValue);
}

void UInputManager::CallbackAxis_MoveLeftRight(float AxisValue)
{
	/*AActor* Leader = RGAMEINSTANCE(this)->PlayStateInfo->GetOwnerCharacter(ROLE_LEADER_INDEX);
	if (!IsValid(Leader))
		return;*/

	ABasePlayerController* BasePC = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
	if (!IsValid(BasePC))
		return;

	DPadMoveValue.Y = AxisValue;
	
	OnInputAxisValueDelegate.Broadcast(EInputCommandTypeEnum::VE_LeftAxis, DPadMoveValue);
}

void UInputManager::CallbackAxis_SkillUpDown(float AxisValue)
{
	if (DPadSkillValue.X != AxisValue)
	{
#if LOG_SKILLPAD
		UE_LOG(LogInputMgr, Log, TEXT("CallbackAxis_SkillUpDowm:: %f"), AxisValue);
#endif
	}

	DPadSkillValue.X = AxisValue;
	OnInputSkillAxisValueDelegate.Broadcast(EInputCommandTypeEnum::VE_RightAxis, DPadSkillValue);
}

void UInputManager::CallbackAxis_SkillLeftRight(float AxisValue)
{
	if (DPadSkillValue.Y != AxisValue)
	{
#if LOG_SKILLPAD
		UE_LOG(LogInputMgr, Log, TEXT("CallbackAxis_SkillLeftRight:: %f"), AxisValue);
#endif
	}

	DPadSkillValue.Y = AxisValue;
	OnInputSkillAxisValueDelegate.Broadcast(EInputCommandTypeEnum::VE_RightAxis, DPadSkillValue);
}

void UInputManager::CallbackSkillAction_Pressed(FKey Key)
{
#if LOG_SKILLPAD
	UE_LOG(LogInputMgr, Log, TEXT("CallbackSkillAction_Pressed:: %s"), *(Key.ToString()));
#endif

	LastInputKey = Key;

	if (false == ReadyToSkillKey.IsValid())
	{
		ReadyToSkillKey = Key;
		UsingSkillDPad = false;
	}
}

void UInputManager::CallbackSkillAction_Released(FKey Key)
{
#if LOG_SKILLPAD
	UE_LOG(LogInputMgr, Log, TEXT("CallbackSkillAction_Released:: %s"), *(Key.ToString()));
#endif

	if (ReadyToSkillKey == Key)
	{
		ReadyToSkillKey = FKey();

#if LOG_SKILLPAD
		UE_LOG(LogInputMgr, Log, TEXT("USE ACTIVE SKILL :: %s"), *(Key.ToString()));
#endif
	}
}

void UInputManager::CallbackSkillAction_Repeat(FKey Key)
{
#if LOG_SKILLPAD
	UE_LOG(LogInputMgr, Log, TEXT("CallbackSkillAction_Repeat:: %s"), *(Key.ToString()));
#endif

	if (ReadyToSkillKey == Key)
	{
		UsingSkillDPad = true;
	}
}

bool UInputManager::IsCheckDragState()
{
	if( TouchInfoMap.Num() != 1)
		return false;

	if( TouchInfoMap.Contains(ETouchIndex::Touch1))
		return true;

	return false;
}

bool UInputManager::IsCheckPinchState()
{
	if( TouchInfoMap.Num() < 2)
		return false;

	if( TouchInfoMap.Contains(ETouchIndex::Touch1) && TouchInfoMap.Contains(ETouchIndex::Touch2) )
		return true;

	return false;
}

int32 UInputManager::GetTouchCount()
{
	return TouchInfoMap.Num();
}