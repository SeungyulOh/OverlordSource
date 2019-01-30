
#include "ProjectR.h"
#include "UC_GuildModifyPopup.h"
#include "UI/Common/UC_CurrencyDisplay.h"

#include "UtilFunctionIntegrated.h"
#include "UI/ItemManagement/UP_ItemShortagePopup.h"
#include "Network/PacketFunctionLibrary.h"
#include "UI/RWidgetManager.h"
#include "GameInfo/UserInfo.h"
#include "GameInfo/GuildInfo.h"

#define DEF_GUILD_DESC 0
#define DEF_GUILD_NAME 1
#define DEF_CHANGE_NAME_COUNT 1
#define DEF_TEXT_NAME_MAX 14
#define DEF_TEXT_NOTICE_MAX 30
#define DEF_TEXT_DESC_MAX 40

void UUC_GuildModifyPopup::NativeConstruct()
{
	Super::NativeConstruct();
	Button_OK->OnClicked.AddDynamic(this, &UUC_GuildModifyPopup::OnClick_OK);
	Button_Cancel->OnClicked.AddDynamic(this, &UUC_GuildModifyPopup::OnClickClose);
	EditableText_ChangeNotice->OnTextChanged.AddDynamic(this, &UUC_GuildModifyPopup::OnTextChange);
	EditableText_GuildName->OnTextChanged.AddDynamic(this, &UUC_GuildModifyPopup::OnTextChange);
}

void UUC_GuildModifyPopup::NativeDestruct()
{
	Button_OK->OnClicked.RemoveDynamic(this, &UUC_GuildModifyPopup::OnClick_OK);
	Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_GuildModifyPopup::OnClickClose);
	EditableText_ChangeNotice->OnTextChanged.RemoveDynamic(this, &UUC_GuildModifyPopup::OnTextChange);
	EditableText_GuildName->OnTextChanged.RemoveDynamic(this, &UUC_GuildModifyPopup::OnTextChange);
	Super::NativeDestruct();
}

void UUC_GuildModifyPopup::OnTextChange(const FText &changeTxt)
{
	FString text = changeTxt.ToString();

	if (ModifyType == EGuildModifyPopupType::VE_GuildDesc) {
		Text_NoticeLen->SetText(FText::AsNumber(text.Len()));
		Text_NoticeMax->SetText(FText::AsNumber(DEF_TEXT_DESC_MAX));
	}
	else if (ModifyType == EGuildModifyPopupType::VE_GuildNotice) {
		Text_NoticeLen->SetText(FText::AsNumber(text.Len()));
		Text_NoticeMax->SetText(FText::AsNumber(DEF_TEXT_NOTICE_MAX));
	}
	else if (ModifyType == EGuildModifyPopupType::VE_GuildName) {
		Text_NameLen->SetText(FText::AsNumber(text.Len()));
		Text_NameMax->SetText(FText::AsNumber(DEF_TEXT_NAME_MAX));
	}
}

void UUC_GuildModifyPopup::OnClickGuildDescOK()
{
	if (IsValid(EditableText_ChangeNotice))
	{
		UPacketFunctionLibrary::GUILD_EDIT_DESC_RQ(EditableText_ChangeNotice->GetText().ToString());
	}
}

void UUC_GuildModifyPopup::OnClickGuildNoticeOK()
{
	if (IsValid(EditableText_ChangeNotice))
	{
		UPacketFunctionLibrary::GUILD_EDIT_NOTICE_RQ(EditableText_ChangeNotice->GetText().ToString());
	}
}

void UUC_GuildModifyPopup::OnClickGuildNameOK()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (UUIFunctionLibrary::IsEnableUsingCurrency(RGameInstance, WALLET_GEM, DEF_CHANGE_NAME_COUNT))
		UPacketFunctionLibrary::GUILD_CHANGE_NAME_RQ(EditableText_GuildName->GetText().ToString());
	else
	{
		RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		auto Widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Popup_ItemShortage);
		auto ShortagePopup = Cast<UUP_ItemShortagePopup>(Widget);
		ShortagePopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
// 		float GemValue = DEF_CHANGE_NAME_COUNT - RGameInstance->UserInfo->Avatar.gems;
// 		ShortagePopup->SetNecessaryGem(GemValue);
		ShortagePopup->LocalText_Caption->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY));
		ShortagePopup->LocalText_Desc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, "USK_up_ItemManage_OptionWarning"));
	}
}

void UUC_GuildModifyPopup::OnClickClose()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_GuildModifyPopup::OnClick_OK()
{
	switch (ModifyType)
	{
	case EGuildModifyPopupType::VE_GuildNotice:
		OnClickGuildNoticeOK();
		break;
	case EGuildModifyPopupType::VE_GuildDesc:
		OnClickGuildDescOK();
		break;
	case EGuildModifyPopupType::VE_GuildName:
		OnClickGuildNameOK();
		break;
	default:
		break;
	}
}

void UUC_GuildModifyPopup::Init(EGuildModifyPopupType InType)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	ModifyType = InType;
	switch (ModifyType)
	{
	case  EGuildModifyPopupType::VE_GuildDesc:
		InitGuildDesc();
		break;
	case EGuildModifyPopupType::VE_GuildNotice:
		InitGuildNotice();
		break;
	case EGuildModifyPopupType::VE_GuildName:
		InitGuildName();
		break;
	}	
}

void UUC_GuildModifyPopup::InitGuildName()
{
	FString CurrentName = RGAMEINSTANCE(this)->GuildInfo->MyGuildInfo.name;
	BodySwitcher->SetActiveWidgetIndex(DEF_GUILD_NAME);
	Text_Header->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Guild_Management_GuildNameButton")));
	EditableText_GuildName->HintText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("USK_uc_Guild_Search")); 
	EditableText_GuildName->SynchronizeProperties();
	EditableText_GuildName->SetText(FText::FromString(CurrentName));
 	UC_CurrencyDisplay->SetCurrency(WALLET_GEM, DEF_CHANGE_NAME_COUNT);
}

void UUC_GuildModifyPopup::InitGuildDesc()
{
	FString CurrentDesc = RGAMEINSTANCE(this)->GuildInfo->MyGuildInfo.desc;
	BodySwitcher->SetActiveWidgetIndex(DEF_GUILD_DESC);
	Text_Header->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Guild_Management_GuildIntro")));
	EditableText_ChangeNotice->HintText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_GuildFound_Desc_02"));
	EditableText_ChangeNotice->SynchronizeProperties();
	EditableText_ChangeNotice->SetText(FText::FromString(CurrentDesc));
}

void UUC_GuildModifyPopup::InitGuildNotice()
{
	FString CurrentNotice = RGAMEINSTANCE(this)->GuildInfo->MyGuildInfo.notice;
	BodySwitcher->SetActiveWidgetIndex(DEF_GUILD_DESC);
	Text_Header->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Guild_GuildInfo_GuildNotice")));
	EditableText_ChangeNotice->HintText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UP_Guild_Management_GuildNotice"));
	EditableText_ChangeNotice->SynchronizeProperties();
	EditableText_ChangeNotice->SetText(FText::FromString(CurrentNotice));
}