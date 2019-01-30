// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Dialogue.h"

#include "GameInfo/PlayStateInfo.h"

#include "GlobalIntegrated.h"




//

#include "CustomStructures/HeroSlotInfo.h"
#include "CustomActors/3DDialogueActor.h"




#include "UI/Component/LocalizingTextBlock.h"
#include "UtilFunctionIntegrated.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "TutorialLevelScriptActor.h"
#include "DirectionTrackManager.h"
#include "UtilFunctionLibrary.h"


void UUP_Dialogue::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (SkipButton)
	{
		SkipButton->OnClicked.RemoveDynamic(this, &UUP_Dialogue::SkipUI);
		SkipButton->OnClicked.AddDynamic(this, &UUP_Dialogue::SkipUI);
	}

	if (InputShieldButton)
	{
		InputShieldButton->OnClicked.RemoveDynamic(this, &UUP_Dialogue::ClickEvent);
		InputShieldButton->OnClicked.AddDynamic(this, &UUP_Dialogue::ClickEvent);
	}

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->EventManager)
	{
		RGameInstance->EventManager->OnEndMediaEvent.AddDynamic(this, &UUP_Dialogue::ClickEvent);
	}
		
	
	Animation_Btn_Dialogue_Looping = GetAni("Btn_Dialogue_Looping");
	if (Animation_Btn_Dialogue_Looping)
	{
		PlayAnimation(Animation_Btn_Dialogue_Looping);
	}

	FindChildWidgetsWithCertainName(this, NameTextArray, TEXT("NameText_"));


}


void UUP_Dialogue::NativeDestruct()
{
	if (Dialogue3DActor)
		Dialogue3DActor->Destroy();
	ContentsStringAnimation = nullptr;
	UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Story_Dialogue);

	Super::NativeDestruct();
}


void UUP_Dialogue::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	return; // No FadeIn/Out Effects

	
	if (eFadeState == IDLE)
		return;

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
	
	if (eFadeState == FADEOUT)
	{
		if (PCM->FadeTimeRemaining == 0)
		{
			FadeIntervalTime -= InDeltaTime;
			if (FadeIntervalTime < 0)
			{
				StartFadeInOut(FADEIN);
				FadeIntervalTime = 0.1f;
			}
		}
			
	}
	else if (eFadeState == FADEIN)
	{
		if (PCM->FadeTimeRemaining == 0)
		{
			eFadeState = IDLE;
		}
	}
	FLinearColor newColor = ColorAndOpacity;
	newColor.A = FMath::Min<float>(FadeInOutTime - PCM->FadeAmount * FadeInOutTime, 1.f);
	SetColorAndOpacity(newColor);
}

void UUP_Dialogue::Init_AfterCreation(TArray<FDialogueInfo>& InArray, TMap<int32, struct FCustomizeDialogue> InMap)
{
	Dialogues = InArray;
	OverrideMap = InMap;

	Create3DDialogueActor();
	CreateContentsStringAnimation();
	Invalidate();

}

void UUP_Dialogue::Create3DDialogueActor()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (Dialogue3DActor == nullptr)
	{
		FString AssetName = TEXT("/Game/Blueprints/Editor/BP_DialogueActor.BP_DialogueActor_C");
		FStringAssetReference AssetRef(AssetName);
		TAssetSubclassOf<A3DDialogueActor> BluePrintClass(AssetRef);

		if (BluePrintClass.IsValid() == false)
		{
			BluePrintClass.LoadSynchronous();
		}
		UClass* CheckClass = BluePrintClass.Get();
// 		if (CheckClass == nullptr)
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "error - UDialogueManager::LoadSynchronous");
// 		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.bNoFail = true;
		Dialogue3DActor = RGameInstance->GetWorld()->SpawnActor<A3DDialogueActor>(CheckClass, SpawnParams);		
		Dialogue3DActor->UP_Dialogue = this;

		
	}
}

void UUP_Dialogue::Invalidate()
{
	//Show Movie
	if (Dialogues.IsValidIndex(CurrentTalkIndex))
	{
		if (!Dialogues[CurrentTalkIndex].Movie.IsNone())
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);

			UUIFunctionLibrary::ShowUIMediaFrameWork(Dialogues[CurrentTalkIndex].Movie, true);

			if (Dialogue3DActor)
			{
				Dialogue3DActor->HideActor(false);
			}
			IsSpecialDialogeState = true;
			return;
		}
		else if (!Dialogues[CurrentTalkIndex].Sequence.IsNone())
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);

			ABaseSequenceActor* SequenceActor = RGameInstance->SequenceManager->SpawnSequence(Dialogues[CurrentTalkIndex].Sequence);

			if (SequenceActor)
			{
				if (false == SequenceActor->OnSequenceEndEvent.IsAlreadyBound(this, &UUP_Dialogue::EndSequence))
					SequenceActor->OnSequenceEndEvent.AddDynamic(this, &UUP_Dialogue::EndSequence);
			}

			UStoryDirectionState* StoryDS = Cast<UStoryDirectionState>(UUtilFunctionLibrary::GetDirectionTrackManager()->GetCurrentState());
			if (StoryDS)
				StoryDS->Cache_PPSettings();

			if (IsValid(chapter_text))
				chapter_text->SetVisibility(ESlateVisibility::Collapsed);
			if (IsValid(Dialogue))
				Dialogue->SetVisibility(ESlateVisibility::Collapsed);
			if (IsValid(Modal))
				Modal->SetVisibility(ESlateVisibility::Collapsed);
			if (IsValid(InputShieldButton))
				InputShieldButton->SetVisibility(ESlateVisibility::Collapsed);


			if (Dialogue3DActor)
				Dialogue3DActor->HideActor(false);

			IsSpecialDialogeState = true;
			return;
		}
		else if (Dialogues[CurrentTalkIndex].SpeakerName.IsNone())
		{
			//Show First Title
			if (Dialogue3DActor)
			{
				Dialogue3DActor->HideActor();
				Dialogue3DActor->Set2DImageBack(Dialogues[CurrentTalkIndex]);
			}

			if (IsValid(chapter_text))
				chapter_text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (IsValid(Dialogue))
				Dialogue->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (IsValid(Modal))
				Modal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (IsValid(InputShieldButton))
				InputShieldButton->SetVisibility(ESlateVisibility::Visible);
			if (IsValid(CharacterName_Switcher))
				CharacterName_Switcher->SetVisibility(ESlateVisibility::Collapsed);

			FText msg = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Dialogue, Dialogues[CurrentTalkIndex].Dialogue_ID);
			ContentsStringAnimation->StartStringAnimation(msg.ToString());
			IsSpecialDialogeState = false;
			return;
		}
	}
	
	IsSpecialDialogeState = false;

	if (IsValid(chapter_text))
		chapter_text->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(CharacterName_Switcher))
		CharacterName_Switcher->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (IsValid(Dialogue))
		Dialogue->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (IsValid(Modal))
		Modal->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(InputShieldButton))
		InputShieldButton->SetVisibility(ESlateVisibility::Visible);

	if (Dialogue3DActor)
	{
		Dialogue3DActor->HideActor();
		Dialogue3DActor->Set2DImageBack(Dialogues[CurrentTalkIndex]);
		Dialogue3DActor->ShowActor(Dialogues[CurrentTalkIndex]);
	}

	int32 idx = (int32)Dialogue3DActor->CurrentCharacterPos;
	CharacterName_Switcher->SetActiveWidgetIndex(idx);
	FText name = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, Dialogues[CurrentTalkIndex].SpeakerName);
	NameTextArray[idx]->SetText(name);

	if (ContentLocTextBlock)
	{
		ETextJustify::Type TextJustify = ETextJustify::Center;
		switch (Dialogue3DActor->CurrentCharacterPos)
		{
			case EDialogueCharPosition::VE_Left: TextJustify = ETextJustify::Left; break;
			case EDialogueCharPosition::VE_Right: TextJustify = ETextJustify::Right; break;
			case EDialogueCharPosition::VE_Center: TextJustify = ETextJustify::Center; break;
		}
		ContentLocTextBlock->SetJustification(TextJustify);
	}

	if (IsValid(ContentsStringAnimation))
	{
		FText msg = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Dialogue, Dialogues[CurrentTalkIndex].Dialogue_ID);
		if (msg.ToString().Contains(FString(TEXT("{0}"))))
		{
			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			msg = FText::Format(msg, FText::FromString(RGameInstance->UserInfo->Avatar.nick));
		}

		ContentsStringAnimation->StartStringAnimation(msg.ToString());
	}
}


void UUP_Dialogue::CheckAutoPlay()
{
// #if !UE_BUILD_SHIPPING
// 	if ( RGameInstance->IsAutoPlayMode())
// 	{
// 		SkipUI();
// 	}
// #endif

// 	
// 	{
// 		if (RGameInstance->IsAutoPlayMode() || RGameInstance->IsRetryPlay())
// 			SkipUI();
// 		else
// 		{
// 			GetWorld()->GetTimerManager().ClearTimer(SkipForAutoPlayHandle);
// 		}
// 	}
}


void UUP_Dialogue::HideUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));

	//CharacterTexture = nullptr;
	if (ContentLocTextBlock)
	{
		ContentLocTextBlock->LocalizingKey = FName("");
		ContentLocTextBlock->SetText(FText::GetEmpty());
		ContentString.Reset();
		VisibleString.Reset();
	}
	
	CurrentTalkIndex = 0;

	CurrentCharNameID = FName();

	this->SetVisibility(ESlateVisibility::Collapsed);

	if (TimerHandle_StringAnimation.IsValid())
	{
		RGameInstance->TimerManager->ClearTimer(TimerHandle_StringAnimation);
	}
	RemoveFromViewport();
}

void UUP_Dialogue::SkipUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));

	ABaseSequenceActor* CurSequenceActor = RGameInstance->SequenceManager->GetCurrentSequenceActor();

	if (CurSequenceActor && CurSequenceActor->IsPlaying())
		CurSequenceActor->Stop();

	if (RGameInstance)
	{
		HideUI();
		
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if (IsValid(BaseStageInfo))
		{
			ALevelActor* LvActor = BaseStageInfo->GetLevelActor();
			if (IsValid(LvActor))
			{
				if (LvActor->CurPhase == 0)
					UUIFunctionLibrary::ReadyToStartUI_Campaign(BaseStageInfo->StageID);
			}

			if (RGameInstance->EventManager)
				RGameInstance->EventManager->OnHideDialogueEvent.Broadcast();
		}
	}
}

void UUP_Dialogue::ClickEvent()
{
	bool bIsEndAnimation = (!IsSpecialDialogeState && ContentsStringAnimation && ContentsStringAnimation->ComputeSkipAnimation());
	if(bIsEndAnimation) 
		return;
	
	++CurrentTalkIndex;
	if (Dialogues.Num() <= CurrentTalkIndex)
	{
		SkipUI();
		return;
	}
	

	Invalidate();
	//StartFadeInOut(FADEIN);
}


void UUP_Dialogue::ShowSkipButton(bool bShow)
{
	if (IsValid(SkipButton))
	{
		if(bShow)
			SkipButton->SetVisibility(ESlateVisibility::Visible);
		else
			SkipButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_Dialogue::CreateContentsStringAnimation()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));

	if (!IsValid( ContentsStringAnimation) )
	{
		ContentsStringAnimation = NewObject< UDialogueStringAnimation>((UObject*)GetTransientPackage(), UDialogueStringAnimation::StaticClass());	
		if (IsValid(ContentsStringAnimation))
		{
			ContentsStringAnimation->Init();
			ContentsStringAnimation->ContentsAnimEvent.AddUniqueDynamic(this, &UUP_Dialogue::CallbackStringAnimation);
		}
	}
}

void UUP_Dialogue::CallbackStringAnimation(FString InString)
{
	if (ContentLocTextBlock)
	{
		FText Tempcontents = FText::FromString(InString);
		ContentLocTextBlock->SetText(Tempcontents);
	}
}


void UUP_Dialogue::EndSequence(FName InSequenceKey)
{
	UStoryDirectionState* StoryDS = Cast<UStoryDirectionState>(UUtilFunctionLibrary::GetDirectionTrackManager()->GetCurrentState());
	if (StoryDS)
		StoryDS->Revert_PPSettings();

	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	ClickEvent();
	
}

bool UUP_Dialogue::IsPlayingStringAnimation()
{
	return TimerHandle_StringAnimation.IsValid();
}

void UUP_Dialogue::PlayingStringAnimation()
{
	++StringAnimationIndex;

	VisibleString = ContentString.Left(StringAnimationIndex);
	
	//ContentString = LocText.ToString();
	if (ContentLocTextBlock)
	{
		FText Tempcontents = FText::FromString(VisibleString);
		ContentLocTextBlock->SetText(Tempcontents);
	}

	//if (ContentString.Compare(VisibleString) == 0)
	if(ContentString.Len() <= StringAnimationIndex)
	{
		EndStringAnimation();
		
		//Contents
	}
}

void UUP_Dialogue::EndStringAnimation(bool InForce /*= false*/)
{
	if (InForce && ContentLocTextBlock)
	{
		FText Tempcontents = FText::FromString(ContentString);
		ContentLocTextBlock->SetText(Tempcontents);
	}

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StringAnimation);
	StringAnimationIndex = 0;
}

void UUP_Dialogue::StartFadeInOut(FADESTATE InFadeState)
{
	eFadeState = InFadeState;

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
	if (eFadeState == FADEOUT)
		PCM->StartCameraFade(0, 1, FadeInOutTime, FLinearColor::Black, false, true);
	else if(eFadeState == FADEIN)
		PCM->StartCameraFade(1, 0, FadeInOutTime, FLinearColor::Black, false);
}

//////////////////////////////////////////////////////////////////////////
// Dialogue Text Animation
void UDialogueStringAnimation::Init()
{
	
}

void UDialogueStringAnimation::StartStringAnimation(FString contents)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));

	Clear();
	
	Contents = contents;

	CurrentIndex = 0;
	MaxIndex = Contents.Len();

	if(contents.IsEmpty())
		return;
	
	if ( RGameInstance->TimerManager)
	{		
		RGameInstance->TimerManager->SetTimer(TimerHandle_StringAnimation, this, &UDialogueStringAnimation::CallbackContentsTimer, TIMER_RATE, true);
	}
	
}

void UDialogueStringAnimation::EndAnimation()
{
	CurrentIndex = Contents.Len();
	ClearTimer();

	ContentsAnimEvent.Broadcast(Contents);

}

void UDialogueStringAnimation::Clear()
{
	Contents.Reset();
	VisibleString.Reset();

	ClearTimer();
}

void UDialogueStringAnimation::ClearTimer()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));

	if (IsActiveAnimation())
	{
		RGameInstance->TimerManager->ClearTimer(TimerHandle_StringAnimation);
	}
}


void UDialogueStringAnimation::CallbackContentsTimer()
{
	++CurrentIndex;

	VisibleString = Contents.Left(CurrentIndex);

	//UE_LOG(LogDialogue, Log, TEXT("[Dialogue Contents] :: %s "), *VisibleString);


	ContentsAnimEvent.Broadcast(VisibleString);

	if (CurrentIndex == MaxIndex)
	{
		Clear();
	}
		
}

bool UDialogueStringAnimation::IsActiveAnimation()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("RGameInstance is Empty"));

	if ( RGameInstance->TimerManager)
	{
		if (RGameInstance->TimerManager->IsTimerActive(TimerHandle_StringAnimation))
		{
			return true;
		}
	}

	return false;
}

bool UDialogueStringAnimation::ComputeSkipAnimation()
{
	//IsActiveAnimation()
	if (CurrentIndex != MaxIndex)
	{
		//VisibleString = Contents;
		CurrentIndex = MaxIndex;

		ContentsAnimEvent.Broadcast(Contents);

		Clear();
		return true;
	}
	
	return false;
}
