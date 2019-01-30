#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "WidgetBluePrintTableInfo.generated.h"


/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FWidgetBluePrintTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CampaignChapterTableInfo)
	TAssetSubclassOf< URBaseWidget >		BlueprintGeneratedClass;

public:
	UClass* GetWidgetClass();
};