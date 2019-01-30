#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GuildModifyPopup.generated.h"

class URGameInstance;
class UUC_CurrencyDisplay;
/**
*
*/
UCLASS()
class PROJECTR_API UUC_GuildModifyPopup: public URBaseWidget
{
	GENERATED_BODY()

public:
	void			NativeConstruct()	override;
	void			NativeDestruct()	override;

public:
	UFUNCTION(BlueprintCallable, Category = UUC_GuildModifyPopup)
	void OnTextChange(const FText &changeTxt);
	UFUNCTION(BlueprintCallable, Category = UUC_GuildModifyPopup)
	void OnClickGuildDescOK();
	UFUNCTION(BlueprintCallable, Category = UUC_GuildModifyPopup)
	void OnClickGuildNoticeOK();
	UFUNCTION(BlueprintCallable, Category = UUC_GuildModifyPopup)
		void OnClickGuildNameOK();
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
		void OnClick_OK();
	UFUNCTION()
	void Init(EGuildModifyPopupType InType);
	UFUNCTION()
	void InitGuildName();
	UFUNCTION()
	void InitGuildDesc();
	UFUNCTION()
	void InitGuildNotice();

public:
	UPROPERTY()
	UWidgetSwitcher*			BodySwitcher;
	UPROPERTY()
		UTextBlock* Text_Header;
	UPROPERTY()
		UTextBlock* Text_NoticeLen;
	UPROPERTY()
		UTextBlock* Text_NoticeMax;
	UPROPERTY()
		UTextBlock* Text_NameLen;
	UPROPERTY()
		UTextBlock* Text_NameMax;
	UPROPERTY()
		UButton* Button_Cancel;
	UPROPERTY()
		UButton* Button_OK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildModifyPopup)
	UUC_CurrencyDisplay*		UC_CurrencyDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildModifyPopup)
	UMultiLineEditableText*		EditableText_ChangeNotice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GuildModifyPopup)
	UMultiLineEditableText*		EditableText_GuildName;

private:
	EGuildModifyPopupType ModifyType;
};