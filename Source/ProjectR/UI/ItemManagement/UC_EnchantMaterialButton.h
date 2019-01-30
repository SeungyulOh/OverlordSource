#pragma once

#include "UI/RBaseWidget.h"
#include "UC_EnchantMaterialButton.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnchantMaterialButtonToggled, bool /* IsOn */);

class UTextBlock;
class UImage;
class UPanelWidget;
class UUC_MaterialIcon;

/**
* component of UC_PopupContent_ItemEnchant BP
*/
UCLASS()
class PROJECTR_API UUC_EnchantMaterialButton : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface
	FName MaterialIconKey = NAME_None;
	int32 MaterialHaveAmount = 0;
	int32 MaterialNeedAmount = 0;

	// override disabled
	bool IsDisabled = false;

	void InvalidateData() override;

	void SetSelect(bool IsOn);
	bool GetIsSelected();

	bool GetIsEnabled();

	FOnEnchantMaterialButtonToggled OnButtonToggled;

	////

	UFUNCTION()
	void OnButtonMaterialClicked();

	//// BP Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EnchantMaterialButton)
	UUC_MaterialIcon* MaterialIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EnchantMaterialButton)
	UButton* Button_Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EnchantMaterialButton)
	UTextBlock* Text_OwnedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EnchantMaterialButton)
	UTextBlock* Text_NeededAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EnchantMaterialButton)
	UImage* Image_Disabled;

	//// BP Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_EnchantMaterialButton)
	FSlateColor Color_InsufficientMaterial;

private:
	bool IsSelected = false;

	FSlateColor SavedMaterialColor;
};
