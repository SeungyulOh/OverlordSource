// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Control/RBaseButton.h"
#include "UC_StageButton.generated.h"


class UImage;

UENUM(BlueprintType)
enum class EStageDisplayInfoTypeEnum : uint8
{
	VE_Stage01				UMETA(DisplayName = "Stage01"),
	VE_Stage02				UMETA(DisplayName = "Stage02"),
	VE_Stage03				UMETA(DisplayName = "Stage03"),
	VE_Stage04				UMETA(DisplayName = "Stage04"),
	VE_Stage05				UMETA(DisplayName = "Stage05"),
	VE_Stage06				UMETA(DisplayName = "Stage06"),
};


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_StageButton : public URBaseButton
{
	GENERATED_BODY()
	
public:
	void		NativeConstruct() override;
	void		NativeDestruct() override;
	void		InvalidateData() override;

	void		SetStageState( int32 InStageIdx, EStageStateType InState );
	void		SetSelected(bool InSelected);
	void		HideGradePanel(bool bHide);
	
	UFUNCTION()
	void GotoCashShop();

	FORCEINLINE EStageStateType GetStageState() { return StageState; }
	FORCEINLINE int32 GetStageIdx() { return StageIdx; }
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_StageButton)
	EStageDisplayInfoTypeEnum			StageDisplayInfo = EStageDisplayInfoTypeEnum::VE_Stage01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UTextBlock*					Text_StageNumber = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UTextBlock*					Text_StageNumber_Elite = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UTextBlock*					Text_Stamina = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UTextBlock*					Text_Stamina_Elite = nullptr;
		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UImage*						Image_Lock = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UImage*						Image_Lock_Elite = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UImage*						ImageHead_Selected_Elite = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UImage*						ImageBody_Selected_Elite = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UImage*						ImageHead_Selected = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UImage*						ImageBody_Selected = nullptr;

	UPROPERTY()
		USizeBox* Quest_Image_Normal;
	UPROPERTY()
		USizeBox* Quest_Image_Hard;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UButton*					ButtonInput = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UCanvasPanel*				PanelGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_StageButton|Ref")
	UCanvasPanel*				PanelGrade_Elite;

	UPROPERTY()
	UVerticalBox*			VerticalBox_Play;
	UPROPERTY()
	UVerticalBox*			VerticalBox_Play_Elite;

	UPROPERTY()
	UWidgetSwitcher*		WidgetSwitcher_Elite;

	UPROPERTY()
	class UUP_Campaign_Tower* UP_Campaign_Tower;
public:
	UFUNCTION()
	void OnButtonClick();

	void SetEliteMode();

private:
	void SetLock();
	void SetGrade();
	void SetNotPlayed();
	void SetStateImage();

private:
	UPROPERTY()
	int32						StageIdx;

	UPROPERTY()
	EStageStateType				StageState = EStageStateType::VE_None;



	UPROPERTY()
	TArray<UWidgetSwitcher*>	WidgetSwitcher_GradeStar;

	bool bSelected = false;
	bool bEliteMode = false;
};
