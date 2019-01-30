#pragma once

#include "UI/RBaseWidget.h"
#include "UP_BottomTip.generated.h"


UCLASS()
class PROJECTR_API UUP_BottomTip: public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_BottomTip")
	FName					TipLocalStringKey = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_BottomTip")
	EStringTableType		TipStringTableType = EStringTableType::VE_UI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_BottomTip|Ref")
	ULocalizingTextBlock*	TipText = nullptr;
};