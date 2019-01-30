// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Editor/BaseEditor.h"
#include "SharedConstants/SharedGlobalConstants.h"
#include "SpawnerEditor.generated.h"

class UUP_SpawnerEditor;
class UUP_EditorPlayTestInfo;

/**
 * 
 */
UCLASS()
class PROJECTR_API ASpawnerEditor : public ABaseEditor
{
	GENERATED_BODY()
	
public:
	
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnerEditor)
	TSubclassOf<class UUP_SpawnerEditor>			SpawnerEditorBpClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnerEditor)
	UUP_SpawnerEditor*								SpawnerEditorUI = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnerEditor)
	TSubclassOf<class UUP_EditorPlayTestInfo>		EditorPlayTestBpClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnerEditor)
	UUP_EditorPlayTestInfo*							EditorPlayTestUI = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnerEditor)
	TSubclassOf<class UUC_HeroStatMgr_Info>			EditorPlayHeroStatBpClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnerEditor)
	UUC_HeroStatMgr_Info*							EditorPlayHeroStateUI = nullptr;

// 	UFUNCTION(BlueprintCallable, Category = SpawnerEditor)
// 	void		ShowUI();
// 
// 	UFUNCTION(BlueprintCallable, Category = SpawnerEditor)
// 	void		HideUI();
//
// 	UFUNCTION(BlueprintCallable, Category = SpawnerEditor)
// 	void		ToggleUI();

//	void SetLoadMapName(FName name);
//	FName GetLoadMapName() { return LoadMapName; }

//	bool IsPlayTestInfoUI() { return bPlayTestInfoUI; }
//	void SetPlayTestInfoUI(bool InTestUI) { bPlayTestInfoUI = InTestUI; }

	void ChangeSkillLevel(TArray<FHERO_SKILL>& SkillData, AActor* InTargetActor);
	void HeroEquipItemToObject(AActor* Obj);

private:	
	bool							bFirst = false;
//	bool							bShowUI = false;
	bool							bPause = false;
	bool							bFlip = false;
	bool							bPlayTestInfoUI = false;
	float							GlobalTime = 1.0f;
	FName							LoadMapName;
};
