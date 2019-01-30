// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Character/Components/WidgetDecoratorComponent.h"
#include "SpeechBubbleComponent.generated.h"

class UUCW_SpeechBubble;

USTRUCT(BlueprintType)
struct PROJECTR_API FSpeechBubbleInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpeechBubbleInfo)
	FName			Key;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FSpeechBubbleInfo)
	float			DelayTime = 0.0f;
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTR_API USpeechBubbleComponent : public UWidgetDecoratorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpeechBubbleComponent();

	void BeginPlay() override;

	// Called when the game starts
	void InitializeComponent() override;
	
	// Called every frame
	void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void SetSpeechBubbleShowTime(float time) { SpeechBubbleShowTime = time; }
	void AddSpeechBubble(FName key, float fDelay);
	void ClearList();
		
private:
	float							SpeechBubbleShowTime = 0.0f;

	UPROPERTY()
	TArray< FSpeechBubbleInfo >		SpeechBubbleList;
	UPROPERTY()
	TArray< FSpeechBubbleInfo >		QueueList;

	UPROPERTY()
	UUCW_SpeechBubble*				SpeechBubbleWidget = nullptr;
};
