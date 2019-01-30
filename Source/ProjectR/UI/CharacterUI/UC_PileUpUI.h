#pragma once

#include "UI/RBaseWidget.h"
#include "UC_PileUpUI.generated.h"

UCLASS()
class PROJECTR_API UUC_PileUpUI : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:
	void SetStackProgress(int32 InCurrentCount, int32 InMaxCount);
	void PlayStackFullAni(bool bPlay);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_PileUpUI)
	class UProgressBar*				PileUpProgressBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_PileUpUI)
	class UImage*					OutLine;

};
