// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "TowerFloorMapPopup.generated.h"

class URScrollView;


UCLASS()
class PROJECTR_API UTowerFloorScrollData : public URScrollItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollData)
	int32		nFloor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollData)
	bool		bLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollData)
	FName		FloorTitleLocalKey = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollData)
	FName		FloorDescLocalKey = NAME_None;
};

UCLASS()
class PROJECTR_API UTowerFloorScrollItem: public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UFUNCTION()
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;


	UFUNCTION()
	void OnClickFloorSelectButton();

	void SetFloorScrollItem();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollItem)
	UTextBlock*					FloorText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollItem)
	ULocalizingTextBlock*		FloorTitle = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollItem)
	ULocalizingTextBlock*		FloorDescription = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollItem)
	UButton*					SelectButton = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorScrollItem)
	UOverlay*					LockImage = nullptr;
};

/**
*
*/
UCLASS()
class PROJECTR_API UTowerFloorMapPopup : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	UFUNCTION()
	void OnClickPlayIntroMovie();

	UFUNCTION()
	void OnClickGotoWorld();

	UFUNCTION()
	void OnClickClose();

	UFUNCTION()
	void OnChangedFloor(int32 InFloor);

	void SetCurrentFloorInfo();
	void SetFloorScrollData(int32 InFloor);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorMapPopup)
	URScrollView*				TowerFloorScrollView = nullptr;

	UPROPERTY()
	TArray<UTowerFloorScrollData*> CachedItemDataList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorMapPopup)
	UTextBlock*					CurrentFloor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorMapPopup)
	UButton*					PlayIntroMovieButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorMapPopup)
	UButton*					GotoWorldButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorMapPopup)
	UButton*					CloseButton = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UTowerFloorMapPopup)
	UButton*					BackButton = nullptr;

private:
	void InitScrollList();

private:
	const int32 MAXSCROLLITEM = 7;
	const int32 MAXSCROLLITEMDATA = 25;

};