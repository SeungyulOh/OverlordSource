// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "UI/Dialogue/UP_Dialogue.h"
#include "Global/EventManager.h"
#include "TutorialLevelScriptActor.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API ATutorialLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	ShowDialogue(TArray<int32> DialogueIndexes, TMap<int32, FCustomizeDialogue> OverrideMap);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	AActor*	SpawnLeader(AActor* PositionActor, FName CharacterID);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	AActor*	SpawnEnemy(AActor* PositionActor, FName EnemyID);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	AActor*	SpawnNeutral(AActor* PositionActor, FName NeutralID);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	SpeechBubblePlay(AActor* TargetActor, FName InSpeechKey, float InSpeechShowTime, float InDelay);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	MoveCamera(AActor* PositionActor, float InCameraMove, float InCameraExp);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	ActorsForceStop(TArray<AActor*> ActorList, bool bStop);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	ForceTargetSetting(AActor* OwnerActor, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	ChangeLeaderAI(EAIControlModeEnum InType);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	CameraFade(bool bStart, float Duration);

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	BattleStart();

	UFUNCTION(BlueprintCallable, Category = "ATutorialLevelScriptActor")
	void	EndTutorial();


	UPROPERTY(BlueprintAssignable , Category = "ATutorialLevelScriptActor | Delegates")
	FDelegate_NoParam	OnDialogueCompleted;

	UPROPERTY(BlueprintAssignable, Category = "ATutorialLevelScriptActor | Delegates")
	FDelegate_OnTwoParamActorAndfloat OnHPChanged;

	UPROPERTY(BlueprintAssignable, Category = "ATutorialLevelScriptActor | Delegates")
	FDelegate_NoParam	OnReceiveHeal;

	
	
};
