// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
//#if WITH_KOKOMO

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "SlateBasics.h"
#include "SImage.h"

class PROJECTR_API SDamageFloaterV2 : public SCanvas
{
	friend class FDamageFloater;

public:
	enum eFontColor
	{
		eGreenColor,
		eRedColor,
		eWhiteColor,
		eYellowColor,
		eGrayColor,
		ePinkColor,
		eColorMax
	};

	enum eEtc
	{
		ePlus,
		eBlock,
		eCritical,
		eMagicCritical,
		eMiss,
		ePierce,
		eGrayBracketLeft,
		eGrayBracketRight,
		eGrayPercent,
		eGrayPlus,
		eWhiteBracketLeft,
		eWhiteBracketRight,
		eWhitePercent,
		eWhitePlus,
		eEtcMax
	};

public:
	SLATE_BEGIN_ARGS(SDamageFloaterV2)
	{}
	SLATE_ATTRIBUTE(class ABasePlayerController*, BasePlayerController)
	SLATE_END_ARGS()

	~SDamageFloaterV2();

	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);
	void AddDamageFloater(EBattleDamageResultEnum damagetype, int32 damage, int32 addpercent, FVector pos, FLinearColor color, float inScale, bool bShowText);
	void CleanStatic();

	class FDamageFloater
	{
	public:
		FVector BasePos;
		FVector CurrentPos;
		FLinearColor Color;
		TArray<int32> DamageDigits;
		TArray<int32> PercentDigits;
		int32 Damage;
		int32 Percent;
		bool Positive;
		bool ShowText;
		float LifeSeconds;
		float CurrentLife;
		float FloatingSpeed;
		float Padding;
		float Scale;
		float SpacingScale;

		TArray<FVector2D> DigitSizes;
		TArray<FVector2D> DigitEtcSizes;
		TArray<FVector2D> PercentSizes;
		TArray<TSharedRef<SImage>> DamageImages;
		TArray<SCanvas::FSlot*> Slots;
		TArray<TSharedRef<SImage>> DamageImageEtcs;
		TArray<SCanvas::FSlot*> SlotEtcs;
		TArray<TSharedRef<SImage>> PercentImages;
		TArray<SCanvas::FSlot*> SlotPercent;

		EBattleDamageResultEnum DamageType;

		SDamageFloaterV2*		RefCanvas;

		FDamageFloater(EBattleDamageResultEnum damageType, int32 damage, int addpercent, FVector basepos, float padding, float life, float speed, float inScale, bool inShowText, FLinearColor color, SDamageFloaterV2 & canvas);
		~FDamageFloater();
		float GetWidth();
		float GetWidth(float scale);
		float GetRemainedLifeSeconds();
		void Update(float delta, float InScale, ABasePlayerController* playerController);

	private:
		void MakeNumericArray();
		float MakeNumericPanel(SDamageFloaterV2 & canvas);
	};

protected:
	float ScreenScale;
	float SpacingScale;
	class ABasePlayerController * BasePlayerController;

	TMap<int32, TArray<TWeakObjectPtr<class UPaperSprite>>>		NumericSpritesMap;
	TMap<int32, TArray<TSharedPtr<FSlateBrush>>>				BrushSetMap;

	TMap<int32, TWeakObjectPtr<class UPaperSprite>>				EtcSpritesMap;
	TMap<int32, TSharedPtr<FSlateBrush>>						EtcBrushSetMap;

	TArray<FDamageFloater*> DamageFloaters;

	FVector2D GetScreenPos(FVector position);
	void MakeNumericSprites();
};

//#endif