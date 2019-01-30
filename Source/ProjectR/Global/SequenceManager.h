// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "CustomActors/BaseSequenceActor.h"
#include "SharedConstants/GlobalConstants.h"
#include "Tracks/IMovieSceneTransformOrigin.h"
#include "SequenceManager.generated.h"


//class URGameInstance;
class ABaseSequenceActor;


/** UDefaultLevelSequenceInstanceData copied with the members' accessibility set to public */
UCLASS()
class URDefaultLevelSequenceInstanceData
	: public UObject
	, public IMovieSceneTransformOrigin
{
	GENERATED_BODY()

	URDefaultLevelSequenceInstanceData(const FObjectInitializer& Init)
		: Super(Init)
	{
		TransformOriginActor = nullptr;
		TransformOrigin = FTransform::Identity;
	}

	virtual FTransform NativeGetTransformOrigin() const override { return TransformOriginActor ? TransformOriginActor->ActorToWorld() : TransformOrigin; }

public:
	/** When set, this actor's world position will be used as the transform origin for all absolute transform sections */
	UPROPERTY(EditAnywhere, Category="General")
	AActor* TransformOriginActor;

	/** Specifies a transform from which all absolute transform sections inside the sequence should be added to. Scale is ignored. */
	UPROPERTY(EditAnywhere, Category="General")
	FTransform TransformOrigin;
};


UCLASS(BlueprintType)
class PROJECTR_API USequenceManager : public UObject
{
	GENERATED_BODY()
		
public:
	void								Initialize(void);
	void								ClearSequence();
	void								ClearSequence_ForGacha();
	ABaseSequenceActor*					SpawnSequence(FName id, bool bPlay = true, bool bLoop = false);
	// Test Level Sequence Dynamic Transform Origin
	ABaseSequenceActor*					SpawnSequenceWithTransformOrigin(FName id, const FTransform& Offset, bool bPlay = true, bool bLoop = false);
	ABaseSequenceActor*					FindSequence(FName name);
	ABaseSequenceActor*					PlaySequence(FName name = FName());
	bool								IsPlaying(FName name = FName());
	void								StopSequence(FName name = FName());
	void								PauseSequence(FName name = FName());
	void								ResumeSequence(FName name = FName());
	void								CleanUpSequence(FName name = FName());
	void								AllPause();

	void								CreateSubTitle();
	void								RemoveSubTitle();
	void								SetSubTitleLegacy(FName id);//4.14
	void								SetSubTitle(FName InNextSubtitle);//4.15

	//////////////////////////////////////////////////////////////////////////

	ABaseSequenceActor*					PlaySequenceAtTargetActorLocation(FName SequenceKey, ESequenceActorType InSequenceType, AActor* TargetActor);
	
	AActor*								SpawnSequenceTargetActor(FName& InTargetActorName, FTransform& InTargetTransform);

	//////////////////////////////////////////////////////////////////////////
	// Event
	UFUNCTION()
	void								OnSequenceEnd_UltimateSkill(FName InSequenceKey);
	

	FORCEINLINE ABaseSequenceActor* GetCurrentSequenceActor() { return CurPlaySequenceActor.Get(); }

private:
	//UPROPERTY()
	//URGameInstance*						GameInstance = nullptr;
	UPROPERTY()
	TWeakObjectPtr< ABaseSequenceActor >					CurPlaySequenceActor = nullptr;

	UPROPERTY()
	TArray< TWeakObjectPtr< ABaseSequenceActor > >			SequenceList;
	UPROPERTY()
	TMap<FName, TWeakObjectPtr< ABaseSequenceActor > >		SequenceMap;

	UPROPERTY()
	FName CurrentSubtitle;

	UPROPERTY()
	class UUP_MediaFrameWorkPlayer* SubtitleWidget = nullptr;

	int32 PrevIdx = 0;
	
};
