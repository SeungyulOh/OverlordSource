#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GuildRuneRequestPopup.generated.h"

class URScrollView;
class UButton;

/**
*
*/
UCLASS()
class PROJECTR_API UUC_GuildRuneRequestPopup: public URBaseWidget
{
	GENERATED_BODY()

public:
	void			NativeConstruct()	override;
	void			NativeDestruct()	override;
	void			NativeTick(const FGeometry& MyGeometry, float InDeltaTime)	override;
public:
	UFUNCTION()
	void OnClickOK();

	UFUNCTION()
	void OnClickClose();

	UFUNCTION()
	void OnSelectHeroScrollItem(URScrollItem* selectItem);

public:
	void ShowRequestRunePopup();
private:
	void Init();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneRequest)
	URScrollView*			RequestRuneScrollView = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneRequest)
	UButton*				Button_OK = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneRequest)
	UButton*				Button_Close = nullptr;

private:
	bool					bPlayCloseAni = false;

	UPROPERTY()
	FName				SelectedHeroID;
};