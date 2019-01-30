// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "RRichTextBlock.generated.h"

/**
 * The rich text block
 *
 * ● Fancy Text
 * ● No Children
 */
UCLASS(meta = (DisplayName = "RichText"))
class PROJECTR_API URRichTextBlock : public UWidget
{
	GENERATED_BODY()

public:
	URRichTextBlock(const FObjectInitializer& ObjectInitializer);
	
	// UWidget interface
	UFUNCTION(BlueprintCallable, Category= RichText)
	void SynchronizeProperties() override;
	// End of UWidget interface

	// UVisual interface
	void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

	UFUNCTION(BlueprintCallable, Category = RichText)
	void SetMyRichTextBlock();

	UFUNCTION(BlueprintCallable, Category = RichText)
	void SetRichText(FText InText);

	UFUNCTION(BlueprintCallable, Category = RichText)
		void SetRichColor(FColor InColor);
	
#if WITH_EDITOR
	// UWidget interface
	const FText GetPaletteCategory() override;
	// End UWidget interface
#endif

	/** True if we're wrapping text automatically based on the computed horizontal space for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	bool AutoWrapText;

protected:
	PROPERTY_BINDING_IMPLEMENTATION(FText, Text);
	/** The text to display */
	UPROPERTY(EditAnywhere, Category=Content, meta=( MultiLine="true" ))
	FText Text;

	/** A bindable delegate to allow logic to drive the text of the widget */
	UPROPERTY()
	FGetText TextDelegate;

	/** The default font for the text. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateFontInfo Font;

	/** The default color for the text. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FLinearColor Color;

	/** How the text should be aligned with the margin. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	TEnumAsByte<ETextJustify::Type> Justification;

	/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no wrapping occurs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	float WrapTextAt;

	/** The amount of blank space left around the edges of text area. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	FMargin Margin;

	/** The amount to scale each lines height by. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance, AdvancedDisplay)
	float LineHeightPercentage;

	UPROPERTY(EditAnywhere, Instanced, Category=Decorators)
	TArray<class URichTextBlockDecorator*> Decorators;

protected:
	UPROPERTY()
	FTextBlockStyle DefaultStyle;

	/** Native Slate Widget */
	TSharedPtr<SRichTextBlock> MyRichTextBlock;

	// UWidget interface
	TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};