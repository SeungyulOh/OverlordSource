// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CustomStructures/DialogueInfos.h"
#include "3DDialogueActor.generated.h"

struct FChapterInfo;

UCLASS()
class PROJECTR_API A3DDialogueActor : public AActor
{
	GENERATED_BODY()
	
public:
	A3DDialogueActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;
	//void Tick(float DeltaSeconds) override;
	
	void ShowActor(FDialogueInfo& InInfo);
	void HideActor(bool InIsReset = true);
	void Set2DImageBack(FDialogueInfo& InInfo);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class USkeletalMeshComponent*		LeftSkelComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class USkeletalMeshComponent*		RightSkelComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class USkeletalMeshComponent*		CenterSkelComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class USkeletalMeshComponent*		LeftDecoComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class USkeletalMeshComponent*		RightDecoComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class UDirectionalLightComponent*	LeftLight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class UDirectionalLightComponent*	RightLight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class UPointLightComponent* LeftPointLight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class UPointLightComponent* RightPointLight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	UStaticMeshComponent*		LeftWeaponL = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	UStaticMeshComponent*		LeftWeaponR = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	UStaticMeshComponent*		RightWeaponL = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	UStaticMeshComponent*		RightWeaponR = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	UStaticMeshComponent*		CenterWeaponL = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	UStaticMeshComponent*		CenterWeaponR = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	class URWidgetComponent*		WidgetComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Ref")
	USceneComponent*				MyRootComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DDialogueActor|Value")
	FColor						HideColor = FColor(130, 130, 130, 255);

	UPROPERTY()
	TWeakObjectPtr<class UUP_Dialogue> 		UP_Dialogue;

	UPROPERTY()
	TArray<FVector> InitialPositions;
	UPROPERTY()
	TArray<FQuat> InitialRotations;

private:
	UFUNCTION()
	void SyncPos();
	UFUNCTION()
	void ProcessPos();

	bool ComputeCurrentCharacter();
	void CreateCharacter();	
	void SetCharacterOffset();
	void SetCharacterAnimation();
	

	void SetMaterial();
	void ResetCharacter();

	void ComputeWeapon();
	void ResetWeapon();

	UAnimationAsset* GetAnimation();
	
	bool SyncViewCameraPos();
	void PrevCharacterEffect(bool InDeafult = true);

private:
	UPROPERTY()
	TMap< FName, USkeletalMesh* >	CharacterMap;

	UPROPERTY()
	TMap< FName, USkeletalMesh* >	CharacterDecoMap;

	UPROPERTY()
	TMap< FName, UStaticMesh* >		WeaponMap;
	
	UPROPERTY()
	FName							CurrentCharacterName;

	UPROPERTY()
	FName							CurrentSpeakerResourceID;

	UPROPERTY()
	FName							CurrentWeaponName;

	UPROPERTY()
	USkeletalMesh*					CurrentCharacterMesh = nullptr;

	UPROPERTY()
	USkeletalMesh*					CurrentCharacterDecoMesh = nullptr;

	UPROPERTY()
	UStaticMesh*					CurrentWeaponMesh = nullptr;

	UPROPERTY()
	USkeletalMeshComponent*			CurrentTargetCharacter = nullptr;

	UPROPERTY()
	USkeletalMeshComponent*			CurrentTargetDeco = nullptr;

	



	UPROPERTY()
	FTimerHandle					SyncHandler;

	UPROPERTY()
	FDialogueInfo					CurrentDialogue;

public:
	EDialogueCharPosition			CurrentCharacterPos;
	EDialogueCharPosition			PrevCharacterPos;

	bool							IsCompleteInit = false;
	bool							IsReverseAni = false;
	int32							SyncCount = 0;
};
