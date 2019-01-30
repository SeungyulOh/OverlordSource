#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "NPCSystemMessage.generated.h"

/**

 * Text bubble processing of the SD character displayed in the upper right of the lobby screen
 
 * It is currently in the lobby but may be displayed on other screens.
 
 */
	
UCLASS()
class PROJECTR_API UNPCSystemMessage : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetNPCMessageText(FText InText);

	UFUNCTION(BlueprintCallable, Category = NPCSystemMessage)
	void OnClickMessageButton();

	void ShowTextOverlay(bool bShow);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCSystemMessage|Ref")
	UButton*					MessageButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCSystemMessage|Ref")
	UMultiLineEditableText*		NPCMessageText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCSystemMessage|Ref")
	UOverlay*					MessageOverlay = nullptr;
};
