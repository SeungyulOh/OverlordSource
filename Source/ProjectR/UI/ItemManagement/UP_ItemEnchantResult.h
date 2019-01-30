// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "Network/PacketDataStructures.h"
#include "UP_ItemEnchantResult.generated.h"

class UButton;
class UUC_Gacha_Item;
class ABaseSequenceActor;
/**
 * BP: UP_ItemEnchant_Result
 * WBP_Enchant_Result
 */
UCLASS()
class PROJECTR_API UUP_ItemEnchantResult : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ShowSuccessResult(FITEM_ICON_INFO ResultItemIconInfo);
	void ShowFailResult(FITEM_ICON_INFO ResultItemIconInfo);
	void ShowDestructionResult(FITEM_ICON_INFO ResultItemIconInfo, const TArray<FITEM>& FailedReturnMaterials);

	//void AttachItem(FName ItemID);
		
	
	
	UFUNCTION(BlueprintCallable, Category = UP_ItemEnchantResult)
	void OnClickOK();

	UFUNCTION(BlueprintCallable, Category = UP_ItemEnchantResult)
	void ReturnEnchant();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantResult)
	UButton*							Button_OpenAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_Gacha_Item)
	FString								ConfirmBtnAniName = TEXT("Initialize_1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantResult)
	FName								EnchantSuccessSequenceID = FName(TEXT("Enchant_Succes_Sequence"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantResult)
	FName								EnchantFailSequenceID = FName(TEXT("Enchant_Fail_Sequence"));

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantResult)
// 	FName								EnchantFailSequenceID = FName(TEXT("Enchant_Fail_Sequence"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantResult)
	FName								ItemCardName = FName(TEXT("GachaItem_Card"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantResult)
	bool								IsPlaySequence = false;

	// success squence or faile squence..
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantResult)
	bool								IsSuccessSequence = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_ItemEnchantResult)
	AActor* SpawnItem = nullptr;

	UPROPERTY()
	EENCHANT_RESULT						EnchantResult;

private:

	UFUNCTION()
	void				CallbackShowResult();

	UFUNCTION()
	void				EndSquence(FName SequenceKey);

	UFUNCTION()
	void				OnCreateWidget();
	

	void				SetItemIconInUserWidget(FITEM_ICON_INFO ResultItemIconInfo);
	TWeakObjectPtr< UUC_Gacha_Item>		FindItemCard(FITEM_ICON_INFO ResultItemIconInfo);
	FName				GetSequenceActorTagByItemEquipType(FName ItemID);
	void				StartSequence( bool bSuccess = true );
	FName				GetSequenceName(bool bSuccess);

	void				VisibleSucessInfo();
	void				VisibleFailedInfo();
	void				VisibleDestructionInfo();

	AActor*				GetItemCard(FName ActorName);
	void				ReturnToPreviousEnvironment();
	

private:
	UPROPERTY()
	TArray<FITEM>			MaterialsAfterDestructions;
	UPROPERTY()
	FITEM_ICON_INFO			CurrentItemIconInfo;
	UPROPERTY()
	bool					IsSetItmeIcon = false;

	UPROPERTY()
	TWeakObjectPtr<	AActor >				LobbyCameraActor;
	TWeakObjectPtr<ABaseSequenceActor>		CurSequenceActor;
	FName					CurSequenceName;

	UPROPERTY()
	FTimerHandle						FailHandle;
};
