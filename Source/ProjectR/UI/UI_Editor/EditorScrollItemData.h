// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Control/RScrollItemData.h"
#include "SharedConstants/GlobalConstants.h"
#include "EditorScrollItemData.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EEditorSItemTypeEnum : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_PDEditor_Map			UMETA(DisplayName = "PDEditor_Map"),
	VE_PDEditor_Socket		UMETA(DisplayName = "PDEditor_Socket"),
	VE_PDEditor_SocketList	UMETA(DisplayName = "PDEditor_SocketList"),
	VE_PDEditor_LogicMap	UMETA(DisplayName = "PDEditor_LogicMap"),
	VE_TestPlay				UMETA(DisplayName = "TestPlay"),
	VE_TestSequence			UMETA(DisplayName = "TestSequence"),
	VE_TestSpawn			UMETA(DisplayName = "TestSpawn"),
	VE_TestItem				UMETA(DisplayName = "TestItem"),
	VE_TestGem				UMETA(DisplayName = "TestGem"),
	VE_Max,
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UEditorScrollItemData : public URScrollItemData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EditorScrollItemData)
	EEditorSItemTypeEnum			SItemType = EEditorSItemTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EditorScrollItemData)
	UTexture2D*						PortraitTextureRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EditorScrollItemData)
	UObject*						PortraitRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EditorScrollItemData)
	FName							KeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EditorScrollItemData)
	FName							DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EditorScrollItemData)
	int32							BuffStackCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EditorScrollItemData)
	ECharacterCategoryTypeEnum		CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EditorScrollItemData)
	EItemGradeEnum					ItemGrade;
};
