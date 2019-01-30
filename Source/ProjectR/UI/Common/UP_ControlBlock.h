// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Dialogue/UP_Dialogue.h"
#include "UP_ControlBlock.generated.h"


USTRUCT(BlueprintType)
struct PROJECTR_API FControlBlockInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FControlBlockInfo)
	bool				Refresh = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FControlBlockInfo)
	FVector2D			OffsetPos = FVector2D::ZeroVector;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FControlBlockInfo)
	FVector2D			OffsetSize = FVector2D::ZeroVector;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FControlBlockInfo)
	FVector2D			Pos = FVector2D::ZeroVector;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FControlBlockInfo)
	FVector2D			Size = FVector2D::ZeroVector;

};


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_ControlBlock : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetPositionMaterial(int32 InX, int32 InY, int32 InWidth, int32 InHeight, bool bOpen = true);
	void SetPosition(int32 InX, int32 InY, int32 InWidth, int32 InHeight, bool bOpen = true);
	void SetPositionBorder(int32 InX, int32 InY, int32 InWidth, int32 InHeight);
	void SetPositionButton(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize);

	void SetPositionForce(int32 InX, int32 InY, int32 InWidth, int32 InHeight, bool InbMaterial = false);
	void SetPosWidget(UWidget* InWidget, bool InbMaterial = false);
	void SetPosWidgets(TArray<UWidget*> InWidgetList, bool InbMaterial = false);
	void SetPosWidgetsCachedList(TArray<UWidget*> InWidgetList);

	void SetPosWidgetsButton(TArray<UWidget*> InWidgetList, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize);	
	void SetButtonFull();
	void SetPosWidgetsButtonCachedList(TArray<UWidget*> InWidgetList);

	void SetButtonHide();

	void SetDlgPanelPos(float InOffsetX, float InOffsetY);

	void SetButtonRefresh(bool InRefresh);

	void Clear();
	void SetRefresh(bool InRefresh);
	void SetBorder(bool InShow);
	void SetCardRefresh(bool InRefresh);
	void SetOffsetPos(float InOffsetX, float InOffsetY);
	void SetOffsetSize(float InOffsetX, float InOffsetY);

	void SetText(FText InText);
	void ShowAnimation(float InDelay);
	void ShowHighlightEffect(float InAniTime, float InDelay);
	void ShowDlg(bool bShow);
	
	void ShowIndicator(float InDelay);
	void HideIndicator();
	bool IsIndicator() { return bIndicatorStart; }

	void FadeIn(float InFadeTime);
	void FadeOut(float InFadeTime);
	void StartAni(float InAniTime);
	void NextAni(float InAniTime);

	void SetButtonShowClear();

	FVector2D GetOffsetPos(FControlBlockInfo& InSource, FControlBlockInfo& InDest, float InRate);
	FVector2D GetOffsetSize(FControlBlockInfo& InSource, FControlBlockInfo& InDest, float InRate);
	FVector2D GetPos(FControlBlockInfo& InSource, FControlBlockInfo& InDest, float InRate);
	FVector2D GetSize(FControlBlockInfo& InSource, FControlBlockInfo& InDest, float InRate);

	template<class T>
	void SetPosWidget(UWidget* InWidget, FString InName = TEXT(""))
	{
		T* findWidget = FindChildNameWidget<T>(InWidget, InName);
		if (IsValid(findWidget))
		{
			SetPosWidget(findWidget, false);
		}
	}

	template<class T>
	void SetPosWidgetMaterial(UWidget* InWidget, FString InName = TEXT(""))
	{
		T* findWidget = FindChildNameWidget<T>(InWidget, InName);
		if (IsValid(findWidget))
		{
			SetPosWidget(findWidget, true);
		}
	}

	UFUNCTION()
	void OnClickSkipButton();
	
	void ShowSkipButton(bool bShow);
private:
	FIntRect GetRect(TArray<UWidget*> InWidgetList, bool InbMaterial);
	void StartBlockAni(float InAniTime);
	void StartButtonAni(float InAniTime);
	void NextBlockAni(float InAniTime);
	void NextButtonAni(float InAniTime);

	void SetPositionMaterialPrivate(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize, bool bOpen = true);
	void SetPositionPrivate(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize, bool bOpen = true);
	void SetPositionBorderPrivate(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize);
	void SetPositionButtonPrivate(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	UCanvasPanel*				CanvasPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	TArray<UImage*>				ImageList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	UImage*						BackImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	UImage*						ReturnImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	UPanelWidget*				DlgPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	URRichTextBlock*			DlgText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	UPanelWidget*				DlgSubPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	URRichTextBlock*			DlgSubText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	UButton*					BackButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
		UButton*					Button_BlockInputManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	UBorder*					BackBorder = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	FVector2D					ScreenSize = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ControlBlock)
	UButton*					Button_Skip = nullptr;
private:
	UFUNCTION()
	void CallbackStringAnimation(FString visibleString);

private:
	UPROPERTY()
	UDialogueStringAnimation*			ContentsStringAnimation;

private:
	float			DelayAni = 0.0f;
	bool			bStartAni = false;

	float			DelayIndicator = 0.0f;
	bool			bIndicatorStart = false;
	FText			IndicatorText;

	bool			bBorderShow = false;
	TArray< TWeakObjectPtr<UWidget> >		CachedList;	
	bool			bTempMaterial = false;

	TArray< TWeakObjectPtr<UWidget> >		CachedButtonList;

	float			HighlightDelayAni = 0.0f;
	float			HighlightAniTime = 0.0f;
	float			CurHighlightAniTime = 0.0f;
	bool			bHighlightStart = false;

	FControlBlockInfo		BlockInfo;
	FControlBlockInfo		SourceBlockInfo;

	FControlBlockInfo		BorderBlockInfo;
	FControlBlockInfo		SourceBorderBlockInfo;

	FControlBlockInfo		ButtonBlockInfo;
	FControlBlockInfo		SourceButtonBlockInfo;	

	bool			bFade = false;
	int32			FadeType = 0;
	float			CurFadeTime = 0.0f;
	float			FadeTime = 0.0f;

	bool			bBlockAni = false;
	float			CurBlockAniTime = 0.0f;
	float			BlockAniTime = 0.0f;

	bool			bButtonAni = false;
	float			CurButtonAniTime = 0.0f;
	float			ButtonAniTime = 0.0f;

	bool			bButtonShow = false;

	bool			bCardRefresh = false;
};
