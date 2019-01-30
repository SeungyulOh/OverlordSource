// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Control/RScrollItem.h"
#include "UP_SpawnerEditor.generated.h"

class URScrollView;
class UUC_EquipItemButton;

UENUM(BlueprintType, meta = (Bitflags))		//"BlueprintType" is essential to include
enum class ETestSpawnTypeEnum : uint8
{
	VE_Npc				UMETA(DisplayName = "Npc"),
	VE_Minion			UMETA(DisplayName = "Minion"),
	VE_NeutralNpc		UMETA(DisplayName = "NeutralNpc"),
	VE_BattleObject		UMETA(DisplayName = "BObject"),
	VE_Hero				UMETA(DisplayName = "Hero"),
	VE_Max,
};

UENUM(BlueprintType, meta = (Bitflags))		//"BlueprintType" is essential to include
enum class ETestRightScrollTypeEnum : uint8
{
//	VE_Map				UMETA(DisplayName = "Map"),
	VE_Sequence			UMETA(DisplayName = "Sequence"),
	VE_Max,
};

/**
 * BP: UP_SpawnerEditor_BP
 */

UCLASS()
class PROJECTR_API UUC_SpiritScrollItemData : public URScrollItemData
{
	GENERATED_BODY()

public:
	FName SkillTableID;
};


UCLASS()
class PROJECTR_API UUC_SpiritScrollItem : public URScrollItem
{
	GENERATED_BODY()

public:
	void InvalidateData() override;
	void Selected() override;

public:
	UPROPERTY()
	UTextBlock*	TableKey;
	UPROPERTY()
	UTextBlock*	CasterEffectName;
	UPROPERTY()
	UTextBlock*	CasterPercent;
	UPROPERTY()
	UOverlay* Overlay_Selected;

private:
	const int32 MAXSPIRIT = 4;
};

UCLASS()
class PROJECTR_API UUC_SpiritSettings : public URBaseWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SpiritSettings)
	URScrollView*			Scrollview;

private:
	const int32 MAXSCROLLITEM = 16;
	const int32 MAXSCROLLTIEMDATA = 80;
	

};



UCLASS()
class PROJECTR_API UUP_SpawnerEditor : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void InvalidateData() override;

	void ProcessOption();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	URScrollView*			ScrollList = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	URScrollView*			TestSpawnScrollList = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		XTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		YTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		ZTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		LevelTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		NameFindTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		IdFindTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UCheckBox*				EnemyCheckBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UCheckBox*				HeroOffsetCheckBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UCheckBox*				PlayInfoCheckBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_SpawnerEditor|Ref")
	UCheckBox*				CoolTimeZeroCheckBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UCheckBox*				HeroLevelCheckBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		A1LevelTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		A2LevelTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		A3LevelTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		P1LevelTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UEditableTextBox*		P2LevelTextBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UCanvasPanel*			PopupPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	URScrollView*			PopupScrollList = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UCheckBox*				HeroItemCheckBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UUC_EquipItemButton*	ItemButton01 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UUC_EquipItemButton*	ItemButton02 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UUC_EquipItemButton*	ItemButton03 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UUC_EquipItemButton*	ItemButton04 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UUC_EquipItemButton*	ItemButton05 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UCanvasPanel*			HeroInfoPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	URScrollView*			Gem01ScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	URScrollView*			Gem02ScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	URScrollView*			Gem03ScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	ETestSpawnTypeEnum			CurTestSpawnType = ETestSpawnTypeEnum::VE_Npc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	ETestRightScrollTypeEnum	CurTestRightScrollType = ETestRightScrollTypeEnum::VE_Sequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_SpawnerEditor)
	UUC_SpiritSettings*		UC_SpiritSettings;

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					TestRightScrollInvlidate();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					TestSpawnInvlidate();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					PlaySequence();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					TestSpawn();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					MapLoad();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					Refresh();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					ReSave();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					SaveStageSpawn();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					ConvertCSVtoJSON();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					ConvertJSONtoXML();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	bool					CopyStageSpawners();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	bool					CopyStageSpawnersUnreal();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					FindNameList();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					IdNameList();

// 	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
// 	void					PlayInfoUIRefresh();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					EnemyCheckBoxOnOff();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					CheckBoxOnOff();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					ShowSelectItem(UUC_EquipItemButton* InButton);

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					PopupCancel();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					PopupOk();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					ItemCheckBoxOnOff();

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					OnPopupSelectScrollItem(URScrollItemData* selectItem);

	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
	void					OnClick_SpiritSettings();



	UFUNCTION(BlueprintCallable, Category = UP_SpawnerEditor)
		void					GemScrollRefresh(URScrollView* scrollView, FName itemKey);
private:
	int32					CurFindIdx = -1;
	UUC_EquipItemButton*	CurEquipItemButton = nullptr;
};



