// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
//#if WITH_KOKOMO

#include "ProjectR.h"
#include "SDamageFloater.h"
#include "PaperSprite.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"
#include "Algo/Reverse.h"
#include "RHI.h"

/**
* Construct this widget
*
* @param	InArgs	The declaration data for this widget
*/
TArray<TWeakObjectPtr<UPaperSprite>> SDamageFloater::NumericSprites;
TArray<TSharedPtr<FSlateBrush>> SDamageFloater::BrushSet;

void SDamageFloater::Construct(const FArguments& InArgs)
{
	bCanTick = true;

	//this->Advanced_SetDesiredSize(FVector2D(600.0f, 100.0f));

	ColorAndOpacity = InArgs._ColorAndOpacity;
	OwnerActor = InArgs._OwnerActor.Get();
	Damage = InArgs._Damage.Get();
	Positive = InArgs._Positive.Get();
	Magnifier = InArgs._Magnifier.Get();
	YPivot = InArgs._YPivot.Get();
	FloatingSpeed = InArgs._FloatingSpeed.Get();
	LifeSeconds = InArgs._LifeSeconds.Get();
	CurrentLife = 0.0f;
	
	if (NumericSprites.Num() == 0) MakeNumericSprites();

	NumericArray.Reserve(10);
	DigitWidth = MakeNumericArray(Damage, NumericArray);
	BasePos = FVector2D::ZeroVector;

	this->SetVisibility(EVisibility::HitTestInvisible);
}

void SDamageFloater::MakeDamageDigitImage(TArray<int32> & numericarray)
{
	//AddSlot()[SAssignNew(HorizontalBox, SHorizontalBox)];
	FVector2D slotpos = BasePos;
	int32 Numnumericarray = numericarray.Num();
	for (int i = 0; i < Numnumericarray; ++i)
	{
		//SHorizontalBox::FSlot& slot = HorizontalBox.Get()->AddSlot()
		FSlot& slot = AddSlot()
			.Size(NumericSprites[numericarray[i]]->GetSpriteSize())
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			.Position(slotpos)
			[
				SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(BrushSet[numericarray[i]].Get())
			];
		slotpos.X += NumericSprites[numericarray[i]]->GetSpriteSize().X;
	}
}

FVector2D SDamageFloater::GetScreenPos(FVector position, float magnification)
{
	FVector2D result;
	ABasePlayerController *	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(OwnerActor->GetWorld());
	if (BasePlayerController != nullptr)
	{
		BasePlayerController->ProjectWorldLocationToScreen(OwnerActor->GetActorLocation(), result, true);
		result /= magnification;
	}
	return result;
}

float SDamageFloater::MakeNumericArray(int digit, TArray<int32> & numericarray)
{
	float result = 0.0f;
	while (true)
	{
		if (digit < 10)
		{
			numericarray.Emplace(digit);
			result += NumericSprites[digit]->GetSpriteSize().X;
			break;
		}
		numericarray.Emplace(digit % 10);
		result += NumericSprites[digit % 10]->GetSpriteSize().X;
		digit /= 10;
	}
	Algo::Reverse(numericarray);
	return result;
}

float SDamageFloater::CalcNumericWidth(TArray<int32> & numericarray)
{
	float result = 0.0f;
	int32 Numnumericarray = numericarray.Num();
	for (int32 i = 0; i < Numnumericarray; ++i)
	{
		result += NumericSprites[numericarray[i]]->GetSpriteSize().X;
	}
	return result;
}

void SDamageFloater::MoveNumericImage(FVector2D newpos)
{
	TArray<int32> numericarray;
	float width = MakeNumericArray(Damage, numericarray);
	for (int i = 0; i < Children.Num(); ++i)
	{
		Children[i].PositionAttr.Set(newpos);
		newpos.X += NumericSprites[numericarray[i]]->GetSpriteSize().X;
	}
}

SDamageFloater::~SDamageFloater()
{
	//BrushSet
}

void SDamageFloater::MakeNumericSprites()
{
	FString DefaultPathName = TEXT("/Game/UMG/Image/InGameHUD/Damage/");
	for (int32 i = 0; i < 10; ++i)
	{
		FString defaultnumericspritename = TEXT("Icon_HUD_Damage_Attack_Sprite_") + FString::FormatAsNumber(i);
		FStringAssetReference stringassetref(DefaultPathName + defaultnumericspritename + FString(TEXT(".")) + defaultnumericspritename);
		UPaperSprite* papersprite = LoadObject<UPaperSprite>(NULL, *stringassetref.ToString());
		NumericSprites.Emplace(papersprite);

		TSharedPtr<FSlateBrush> brush = MakeShareable(new FSlateBrush());
		brush->SetResourceObject(NumericSprites[i].Get());
		BrushSet.Emplace(brush);
	}
}


int32 SDamageFloater::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = SCanvas::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	return LayerId;
}

void SDamageFloater::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (OwnerActor.IsValid() == false) return;
	if (BasePos == FVector2D::ZeroVector)
	{
		MakeDamageDigitImage(NumericArray);
		BasePos = GetScreenPos(OwnerActor->GetActorLocation(), AllottedGeometry.Scale);
		BasePos.X -= DigitWidth / 2.0f / AllottedGeometry.Scale;
	}

	BasePos.Y -= FloatingSpeed / AllottedGeometry.Scale * InDeltaTime;

	MoveNumericImage(BasePos);

	CurrentLife += InDeltaTime;
}

/**
* An Image's desired size is whatever the image looks best as. This is decided on a case by case basis via the Width and Height properties.
*/
FVector2D SDamageFloater::ComputeDesiredSize(float) const
{
	return FVector2D::ZeroVector;
}

void SDamageFloater::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	if (!ColorAndOpacity.IdenticalTo(InColorAndOpacity))
	{
		ColorAndOpacity = InColorAndOpacity;
		Invalidate(EInvalidateWidget::Layout);
	}
}

void SDamageFloater::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	if (ColorAndOpacity.IsBound() || ColorAndOpacity.Get() != InColorAndOpacity)
	{
		ColorAndOpacity = InColorAndOpacity;
		Invalidate(EInvalidateWidget::Layout);
	}
}

//#endif