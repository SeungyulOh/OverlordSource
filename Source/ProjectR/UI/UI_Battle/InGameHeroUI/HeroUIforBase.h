#pragma once


// #include "CoreMinimal.h"
#include "UI/RBaseWidget.h"

#include "CustomStructures/HeroSlotInfo.h"

#include "HeroUIforBase.generated.h"



/**
*
*/
UCLASS()
class PROJECTR_API URHeroUIforBase : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void SetHeroSlot(FHeroSlotInfo& InHeroSlotInfo) {}
	virtual void UpdateIcon() {}
	virtual void SetRTSHeroSlot(FHeroSlotInfo& InHeroSlotInfo) {}
	virtual void RTSUpdateIcon() {}

	FHeroSlotInfo& GetHeroSlotInfo() { return HeroSlotInfo; }

	// hp material form
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RHeroUIforBase)
	UTexture*					HPTexture = nullptr;
	
	// hp material color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RHeroUIforBase)
	FLinearColor				HPBlueColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RHeroUIforBase)
	FLinearColor				HPRedColor;

protected:
	UPROPERTY()
	FHeroSlotInfo		HeroSlotInfo;

	UPROPERTY()
	UMaterialInstanceDynamic* MID = nullptr;	// for material control

	UPROPERTY()
	float				DangerValue = 0.3f;
	UPROPERTY()
	bool				bHurt = false;
};