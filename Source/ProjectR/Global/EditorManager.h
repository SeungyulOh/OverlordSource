// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Editor/BaseEditor.h"
#include "EditorManager.generated.h"

//class URGameInstance;

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EEditorEnum : uint8
{
	VE_SpawnerEditor	UMETA(DisplayName = "SpawnerEditor"),
	VE_Max,
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UEditorManager : public UObject
{
	GENERATED_BODY()

public:
	enum eEditorMgrState
	{
		eNoState,
		eNormalLoadState,
	};
	
public:
	UEditorManager();

	UFUNCTION(BlueprintCallable, Category = PDEditorManager)
	void Initialize(/*URGameInstance* instance,*/ EEditorEnum editorType);

	UFUNCTION(BlueprintCallable, Category = PDEditorManager)
	void CreateEditorActor();

	UFUNCTION(BlueprintCallable, Category = PDEditorManager)
	void OnCompleteLoadingLevel();

	void LoadComplate();
	void RefreshState();
	void LoadMap(FName keyName);

	FName GetLoadMap() { return LoadMapKey; }
	bool IsLogicMapLoad() { return LogicMapLoad; }

	EEditorEnum GetEditorType() { return EditorType; }
	ABaseEditor* GetEditorActor() { return EditorActor; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UEditorManager)
	TArray<FString>					ExeCommandList;

private:
	eEditorMgrState					EditorState = eEditorMgrState::eNoState;
	EEditorEnum						EditorType = EEditorEnum::VE_SpawnerEditor;
	TSubclassOf<class ABaseEditor>		EditorBlueprintList[(int32)EEditorEnum::VE_Max];
	ABaseEditor*						EditorActor = nullptr;
	bool							LogicMapLoad = false;
	FName							LoadMapKey;
	int32							SubLevelCount = 0;
};
