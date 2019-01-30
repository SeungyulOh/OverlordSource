#pragma once

#include "UI/RBaseWidget.h"

#include "QuestPassingAlertPopup.generated.h"


UCLASS()
class PROJECTR_API UQuestPassingAlertPopup : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

public:
	UFUNCTION()
	void OnClickOK();

	UFUNCTION()
	void OnClickCancel();


	void SetPopupText(int32 InQuestID);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestPassingAlert)
	URRichTextBlock*			ImportantMissionTargetText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestPassingAlert)
	ULocalizingTextBlock*		ImportantCurrentText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuestPassingAlert)
	ULocalizingTextBlock*		ImportantMaxText = nullptr;

	UPROPERTY()
	UButton*					Button_Cancel = nullptr;
	
	UPROPERTY()
	UButton*					Button_Close = nullptr;

	UPROPERTY()
	UButton*					Button_OK = nullptr;

};