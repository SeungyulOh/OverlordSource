#pragma once

#include "UI/RBaseWidget.h"
#include "UC_ExpBar.generated.h"

/**
*/
UCLASS()
class PROJECTR_API UUC_ExpBar : public URBaseWidget
{
	GENERATED_BODY()
public:

	//// Interface

	TArray<int32> ExpsByLevel;
	int32 CurrentValue;

	void InvalidateData() override;

	void GetLevelAndResidue(int32& OutLevel, int32& OutResidue);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UExpBar)
	UProgressBar* ProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UExpBar)
	UTextBlock* Text_Value;

private:
};
