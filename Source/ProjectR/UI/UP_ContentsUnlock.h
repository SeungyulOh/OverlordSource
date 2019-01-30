// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"

#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"


#include "UP_ContentsUnlock.generated.h"

class UUC_ContentsType;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_ContentsUnlock : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = UP_ContentsUnlock)
	void InvalidateUI();

	UFUNCTION(BlueprintCallable, Category = UP_ContentsUnlock)
	void RefreshUI();

	UFUNCTION()
	void OnRefreshUI();
	
	UFUNCTION(BlueprintCallable, Category = UP_ContentsUnlock)
	void SetInfo(EGamePlayModeTypeEnum InContents, FText Content, FText Desc, int32 SlotIndex = 0);

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UP_ContentsUnlock)
	TArray<UUC_ContentsType*>	ContentsTypeArray;

private:
	bool				bStartAni = false;
};
