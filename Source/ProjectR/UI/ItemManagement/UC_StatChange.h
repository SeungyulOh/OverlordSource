#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_StatChange.generated.h"


class UTextBlock;

/**
* component of UC_PopupContent_ItemEnchant BP
*/
UCLASS()
class PROJECTR_API UUC_StatChange : public URBaseWidget
{
	GENERATED_BODY()
public:
	//// Interface

	void Refresh(EItemStatOption ItemStatOption, int32 BeforeValue, int32 AfterValue);

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_StatChange)
	UTextBlock* Text_StatName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_StatChange)
	UTextBlock* Text_StatValueBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_StatChange)
	UTextBlock* Text_StatValueAfter;
};
