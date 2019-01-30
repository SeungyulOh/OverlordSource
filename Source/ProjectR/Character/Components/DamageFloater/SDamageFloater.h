// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
//#if WITH_KOKOMO

#pragma once

#include "SlateBasics.h"
#include "SImage.h"
#include "SBoxPanel.h"
#include "PaperSprite.h"

class PROJECTR_API SDamageFloater : public SCanvas
{
public:

	SLATE_BEGIN_ARGS(SDamageFloater)
		: _Damage(0)
		, _Magnifier(1.0f)
		, _ColorAndOpacity(FLinearColor::White)
		, _OwnerActor(nullptr)
	{}
		SLATE_ATTRIBUTE(int32, Damage)
		SLATE_ATTRIBUTE(bool, Positive)
		SLATE_ATTRIBUTE(float, Magnifier)
		SLATE_ATTRIBUTE(float, YPivot)
		SLATE_ATTRIBUTE(float, FloatingSpeed)
		SLATE_ATTRIBUTE(float, LifeSeconds)
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
		SLATE_ATTRIBUTE(AActor*, OwnerActor)
	SLATE_END_ARGS()

	~SDamageFloater();

	/** See the ColorAndOpacity attribute */
	void SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity);

	/** See the ColorAndOpacity attribute */
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	//~ Begin SWidget Interface
	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SWidget Interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

	float GetRemainedLifeSeconds()
	{
		return (LifeSeconds - CurrentLife > 0.0f) ? (LifeSeconds - CurrentLife) : 0.0f;
	}

protected:
	TAttribute<FSlateColor> ColorAndOpacity;
	TWeakObjectPtr<AActor> OwnerActor;
	TArray<int32> NumericArray;
	//TSharedPtr<SHorizontalBox> HorizontalBox;
	int32 Damage;
	bool Positive;
	float Magnifier;
	float DigitWidth;
	FVector2D BasePos;
	float YPivot;
	float FloatingSpeed;
	float LifeSeconds;
	float CurrentLife;
	void MakeNumericSprites();
	float MakeNumericArray(int digit, TArray<int32> & numericarray);
	float CalcNumericWidth(TArray<int32> & numericarray);
	void MoveNumericImage(FVector2D newpos);
	FVector2D GetScreenPos(FVector position, float magnification);
	void MakeDamageDigitImage(TArray<int32> & numericarray);

	static TArray<TWeakObjectPtr<UPaperSprite>> NumericSprites;
	static TArray<TSharedPtr<FSlateBrush>> BrushSet;
};

//#endif