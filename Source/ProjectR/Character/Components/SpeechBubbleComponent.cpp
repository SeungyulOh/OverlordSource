// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SpeechBubbleComponent.h"

#include "Character/BaseAIController.h"
#include "UtilFunctionIntegrated.h"

#include "UI/CharacterUI/UCW_SpeechBubble.h"

//#include "AIController.h"
//#include "BehaviorTree/BehaviorTreeComponent.h"
//#include "BehaviorTree/BlackboardComponent.h"
//#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
//#include "AIController.h"
//#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"


// Sets default values for this component's properties
USpeechBubbleComponent::USpeechBubbleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USpeechBubbleComponent::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetDecorator.IsValid())
	{
		SpeechBubbleWidget = Cast<UUCW_SpeechBubble>(WidgetDecorator.Get());
		if (IsValid(SpeechBubbleWidget))
		{
			SpeechBubbleWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// Called when the game starts
void USpeechBubbleComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	OwnerActor = GetOwner();
}


// Called every frame
void USpeechBubbleComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (QueueList.Num() > 0)
	{
		int32 removeIdx = -1;
		for (int32 i = 0; i < QueueList.Num(); ++i)
		{
			QueueList[i].DelayTime -= DeltaTime;
			if (QueueList[i].DelayTime <= 0.0f)
			{
				removeIdx = i;
				if (IsValid(SpeechBubbleWidget))
				{
					SpeechBubbleWidget->Show(QueueList[i].Key, SpeechBubbleShowTime);
				}
			}
		}

		if (removeIdx != -1)
		{
			if (QueueList.IsValidIndex(removeIdx))
			{
				QueueList.RemoveAt(removeIdx);
			}
		}
	}
}

void USpeechBubbleComponent::AddSpeechBubble(FName key, float fDelay)
{
	FSpeechBubbleInfo info;
	info.Key = key;
	info.DelayTime = fDelay;
	QueueList.Emplace(info);
}

void USpeechBubbleComponent::ClearList()
{
	QueueList.Empty();
}

