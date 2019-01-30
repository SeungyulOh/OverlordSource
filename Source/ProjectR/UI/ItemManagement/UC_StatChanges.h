#pragma once

#include "UC_GenericPopupContent.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"

#include "UC_StatChanges.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnItemEnchantComplete);

class UUC_ItemIcon;
class ULocalizingTextBlock;
class UUC_StatChange;
class UUC_EnchantMaterialButton;

/**
* 오버로드_장비아이템_강화.docx.3
*/
UCLASS()
class PROJECTR_API UUC_StatChanges : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	void Refresh(const FItemTableInfo* ItemInfo, const FITEM* ItemDataBefore, const FITEM* ItemDataAfter);

	// stat changes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemEnchant)
	UUC_StatChange* StatChange1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemEnchant)
	UUC_StatChange* StatChange2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemEnchant)
	UUC_StatChange* StatChange3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemEnchant)
	UUC_StatChange* StatChange4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemEnchant)
	UUC_StatChange* StatChange5;

private:
	// BP widgets collected

	UPROPERTY()
	TArray<UUC_StatChange*> StatChanges;
};
