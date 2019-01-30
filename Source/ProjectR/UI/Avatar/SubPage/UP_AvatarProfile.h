// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UP_AvatarProfile.generated.h"

class UCheckBox_Radio;
class URScrollView;
class UUC_AvatarRecord;
class UUC_ScrollItem_AvatarIcon;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_AvatarProfile : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetActiveSubPage() override;

	UFUNCTION(BlueprintCallable, Category = UP_AvatarProfile)
	void SetUseMenuClick(bool InChecked);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	URScrollView*				Scroll_UseHero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UUC_ScrollItem_AvatarIcon*	AvatarIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UTextBlock*					Text_UserName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UTextBlock*					Text_GuildName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UTextBlock*					Text_AvatarLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UTextBlock*					Text_ExpMax = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UTextBlock*					Text_ExpCurrent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UProgressBar*				Bar_Exp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UCheckBox_Radio*		CheckBox_Radio_Pve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UCheckBox_Radio*		CheckBox_Radio_Coloseum = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UCheckBox_Radio*		CheckBox_Radio_Arena = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UUC_AvatarRecord*			Avatar_Record1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UUC_AvatarRecord*			Avatar_Record2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarProfile|Ref")
	UUC_AvatarRecord*			Avatar_Record3 = nullptr;
	
	TArray<UCheckBox_Radio*>	MenuArray;
	TArray<UUC_AvatarRecord*>	RecordArray;
	int32						SelectMenuNumber;

private:
	UFUNCTION()
	void Refresh();
	void SetAvatarInfo();
	UFUNCTION()
	void UpdateRecord();

	void SetRecord();
	void SetUseScroll();

	void SetUITopBarString();

private:
	UPROPERTY()
	class URGameInstance* RGameInstance = nullptr;

	int32 UseTargetNum = 0;
};
