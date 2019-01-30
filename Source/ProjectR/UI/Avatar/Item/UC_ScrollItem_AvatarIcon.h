// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItem.h"
#include "UC_ScrollItem_AvatarIcon.generated.h"

class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_ScrollItem_AvatarIcon : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = UC_ScrollItem_AvatarIcon)
	void SetAvatarIconScrollItemData();
	UFUNCTION()
		void SetGuildGradeIcon(int32 grade);
	UFUNCTION()
	void ClickIcon();

	bool GetIsLock() { return IsLock; }

	UFUNCTION()
	virtual void SetItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	UFUNCTION()
	virtual void SetSelected(bool bSelected) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ScrollItem_AvatarIcon|Ref")
	class UWidgetSwitcher*	WidgetSwitcher_GradeIcon = nullptr;

private:
	UPROPERTY()
	URGameInstance*			RGameInstance = nullptr;

	bool IsLock = false;
};
