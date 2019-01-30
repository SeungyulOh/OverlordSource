#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Guild_LevelUp.generated.h"

class URGameInstance;
class UUC_CurrencyDisplay;
/**
*
*/
UCLASS()
class PROJECTR_API UUC_Guild_LevelUp: public URBaseWidget
{
	GENERATED_BODY()

public:
	void			NativeConstruct()	override;
	void			NativeDestruct()	override;
	void			NativeTick(const FGeometry& MyGeometry, float InDeltaTime)	override;
public:
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_LevelUp)
	void OnClickLevelUp();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_LevelUp)
	void OnClickClose();
	UFUNCTION(BlueprintCallable, Category = UUC_Guild_LevelUp)
		void SetGuildLevelUpInfo(ULocalizingTextBlock* CurrentLevel, ULocalizingTextBlock* NextLevel,
			ULocalizingTextBlock* CurrentPoint, ULocalizingTextBlock* MaxPoint,
			ULocalizingTextBlock* CurrentGold, ULocalizingTextBlock* MaxGold,
			UProgressBar* ExpProgressBar, UTextBlock* ExpText);
	UFUNCTION(BlueprintImplementableEvent, Category = UUC_Guild_LevelUp)
	void BP_SetGuildLevelUpInfo();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_LevelUp)
	UWidgetSwitcher*			TileSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_LevelUp)
	UWidgetSwitcher*			BodySwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Guild_LevelUp)
	UUC_CurrencyDisplay*		CurrenyChangeGuildName;

private:
	EGuildModifyPopupType ModifyType;
};