// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UC_AvatarSkill.generated.h"

class UUP_AvatarAbility;
class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_AvatarSkill : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void OnWidgetRebuilt() override;

	void Refresh(bool bNextLevel = false);

	void ToggleSelected();
	void SetSkillImage(FName InImageKey, FSlateColor InBGTint);

	FORCEINLINE int32	GetIndex()								{ return Index; }
	FORCEINLINE void	SetIndex(int32 InIndex)					{ Index = InIndex; }
	FORCEINLINE void	SetOwner(UUP_AvatarAbility* InOwner)	{ OwnerWidget = InOwner; }
	FORCEINLINE FName	GetTableKey() { return TableKey; }
	FORCEINLINE UUP_AvatarAbility*	GetOwner()					{ return OwnerWidget; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkill|Ref")
	UImage*					Image_BG = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkill|Ref")
	UImage*					Image_SkillIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkill|Ref")
	UOverlay*				Overlay_Select = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkill|Ref")
	UTextBlock*				Text_CurrentValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkill|Ref")
	UTextBlock*				Text_MaxValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkill|Ref")
	UButton*				Button_Click = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_AvatarSkill)
	FName					ImageKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_AvatarSkill)
	FName					TableKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkill|Temp")
	FName					UIStringKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_AvatarSkill)
	FSlateColor				BGTint;

private:
	void SetSkill(bool bNextLevel = false);

	UFUNCTION()
	void ClickIcon();

private:
	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;

	UPROPERTY()
	UUP_AvatarAbility*	OwnerWidget = nullptr;

	int32			Index = -1;
	bool			IsClick = false;
};
