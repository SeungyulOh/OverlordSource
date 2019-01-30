// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ProjectR.h"
#include "Runtime/UMG/Public/Components/RichTextBlockDecorator.h"
#include "RRichTextBlock.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// URRichTextBlock

URRichTextBlock::URRichTextBlock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(TEXT("/Engine/EngineFonts/Roboto"));
	Font = FSlateFontInfo(RobotoFontObj.Object, 12, FName("Regular"));
	Color = FLinearColor::White;
	LineHeightPercentage = 1;

	//Decorators.Add(ObjectInitializer.CreateOptionalDefaultSubobject<URichTextBlockDecorator>(this, FName("DefaultDecorator")));
}

void URRichTextBlock::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock.Reset();
	}
}

TSharedRef<SWidget> URRichTextBlock::RebuildWidget()
{
	//+ OnHyperlinkClicked = FSlateHyperlinkRun::FOnClick::CreateStatic(&RichTextHelper::OnBrowserLinkClicked, AsShared());
	//+ FHyperlinkDecorator::Create(TEXT("browser"), OnHyperlinkClicked))
	//+MakeShareable(new FDefaultRichTextDecorator(Font, Color));

	DefaultStyle.SetFont(Font);
	DefaultStyle.SetColorAndOpacity(Color);

	TArray< TSharedRef< class ITextDecorator > > CreatedDecorators;

	Decorators.Empty();
	Decorators.Emplace( NewObject<URichTextBlockDecorator>(this, FName("DefaultDecorator")));
	for ( URichTextBlockDecorator* Decorator : Decorators )
	{
		if ( Decorator )
		{
			CreatedDecorators.Emplace(Decorator->CreateDecorator(Font, Color));
		}
	}

	MyRichTextBlock =
		SNew(SRichTextBlock)
		.Justification(Justification)
		.AutoWrapText(AutoWrapText)
		.WrapTextAt(WrapTextAt)
		.Margin(Margin)
		.LineHeightPercentage(LineHeightPercentage)
		.TextStyle(&DefaultStyle)
		.Decorators(CreatedDecorators);
	
	return MyRichTextBlock.ToSharedRef();
}

void URRichTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	if (MyRichTextBlock.IsValid())
	{
		TAttribute<FText> TextBinding = PROPERTY_BINDING(FText, Text);

		MyRichTextBlock->SetText(TextBinding);
		MyRichTextBlock->SetAutoWrapText(AutoWrapText);
	}
}

#if WITH_EDITOR

const FText URRichTextBlock::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

#endif

void URRichTextBlock::SetMyRichTextBlock()
{	
	SynchronizeProperties();
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->Refresh();
	}
}

void URRichTextBlock::SetRichText(FText InText)
{
	Text = InText;
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->SetText(Text);
	}
}

void URRichTextBlock::SetRichColor(FColor InColor)
{
	Color = InColor;
	DefaultStyle.SetColorAndOpacity(Color);
	if (MyRichTextBlock.IsValid())
		MyRichTextBlock->SetTextStyle(DefaultStyle);
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE