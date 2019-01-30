// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UC_HUD_PVP_TotalHP.h"
#include "Global/RGameInstance.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RCampaignHUD_RaidThreeControl.h"



void UUC_HUD_PVP_TotalHP::NativeConstruct()
{
	//Blueprint Constructor
	Super::NativeConstruct();
}

void UUC_HUD_PVP_TotalHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
		return;

	if (HPData[0].PrevTotalHP != HPData[0].CurrTotalHP)
	{
		HPData[0].ChangedHP = SmoothlyChangeHP(HPData[0].PrevTotalHP, HPData[0].CurrTotalHP, InDeltaTime);
	}

	if (HPData[1].PrevTotalHP != HPData[1].CurrTotalHP)
	{
		HPData[1].ChangedHP = SmoothlyChangeHP(HPData[1].PrevTotalHP, HPData[1].CurrTotalHP, InDeltaTime);
	}

	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateHPBar();
}

void UUC_HUD_PVP_TotalHP::BP_Init(bool bMyTeamLeft /*= true*/)
{
	isMyTeamLeft = bMyTeamLeft;

	PrepareWidget();

	if (isMyTeamLeft)
		PlayAni(TEXT("Right"));
	else
		PlayAni(TEXT("Left"));
	
}

void UUC_HUD_PVP_TotalHP::InitHP(int32 curr_hp, int32 max_hp, bool team_0)
{
	int32 nCurrentToTalHP = 0 < curr_hp ? curr_hp : max_hp;
	if (team_0)
	{
		HPData[0].PrevTotalHP = max_hp;
		HPData[0].CurrTotalHP = nCurrentToTalHP;
		HPData[0].TotalHPMax = max_hp;
	}
	else
	{
		HPData[1].PrevTotalHP = max_hp;
		HPData[1].CurrTotalHP = nCurrentToTalHP;
		HPData[1].TotalHPMax = max_hp;
	}
}

void UUC_HUD_PVP_TotalHP::PointUpdate(int32 curr_hp, bool team_0)
{
	if (team_0)
	{
		HPData[0].CurrTotalHP = curr_hp;
		SetTextBlockPoint(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_787"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_788"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_3"))), team_0);
	}
	else
	{
		HPData[1].CurrTotalHP = curr_hp;
		SetTextBlockPoint(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_808"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_809"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_5"))), team_0);
	}
}

void UUC_HUD_PVP_TotalHP::SetTextBlockPoint(UTextBlock* curr_HP, UTextBlock* max_HP, UTextBlock* percent, bool team_0)
{
	if (IsValid(max_HP))
		max_HP->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(percent))
		percent->SetVisibility(ESlateVisibility::Collapsed);

	UTextBlock* TextWidget = nullptr;

	TextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_4")));
	if (IsValid(TextWidget))
		TextWidget->SetVisibility(ESlateVisibility::Collapsed);
	TextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_1")));
	if (IsValid(TextWidget))
		TextWidget->SetVisibility(ESlateVisibility::Collapsed);
	TextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_0")));
	if (IsValid(TextWidget))
		TextWidget->SetVisibility(ESlateVisibility::Collapsed);
	TextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_2")));
	if (IsValid(TextWidget))
		TextWidget->SetVisibility(ESlateVisibility::Collapsed);
	TextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_6")));
	if (IsValid(TextWidget))
		TextWidget->SetVisibility(ESlateVisibility::Collapsed);
	TextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_7")));
	if (IsValid(TextWidget))
		TextWidget->SetVisibility(ESlateVisibility::Collapsed);
	TextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_9")));
	if (IsValid(TextWidget))
		TextWidget->SetVisibility(ESlateVisibility::Collapsed);
	TextWidget = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_11")));
	if (IsValid(TextWidget))
		TextWidget->SetVisibility(ESlateVisibility::Collapsed);

	UProgressBar* ProgressWidget = nullptr;

	ProgressWidget = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_673")));
	if(ProgressWidget)
		ProgressWidget->SetPercent(1.0f);
	ProgressWidget = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_672")));
	if (ProgressWidget)
		ProgressWidget->SetPercent(1.0f);
	ProgressWidget = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_674")));
	if (ProgressWidget)
		ProgressWidget->SetPercent(1.0f);
	ProgressWidget = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_675")));
	if (ProgressWidget)
		ProgressWidget->SetPercent(1.0f);

	if (IsValid(curr_HP))
	{
		FText hp_text;
		if (team_0)
		{
			hp_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(HPData[0].CurrTotalHP));
			curr_HP->SetText(hp_text);
		}
		else
		{
			hp_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(HPData[1].CurrTotalHP));
			curr_HP->SetText(hp_text);
		}
	}
}

void UUC_HUD_PVP_TotalHP::PrepareWidget()
{
	SetAvatarInfo(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_750"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_751"))), isMyTeamLeft);
	(Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_673"))))->SetPercent(1.f);
	(Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_672"))))->SetPercent(1.f);
	SetTextBlockHP(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_787"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_788"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_3"))), isMyTeamLeft);

	SetAvatarInfo(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_752"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_753"))), !isMyTeamLeft);
	(Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_674"))))->SetPercent(1.f);
	(Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_675"))))->SetPercent(1.f);
	SetTextBlockHP(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_808"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_809"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_5"))), !isMyTeamLeft);
}

void UUC_HUD_PVP_TotalHP::UpdateHPBar()
{
	SetProgressBarHP(Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_673"))), Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_672"))), isMyTeamLeft);
	SetTextBlockHP(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_787"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_788"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_3"))), isMyTeamLeft);

	SetProgressBarHP(Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_674"))), Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_675"))), !isMyTeamLeft);
	SetTextBlockHP(Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_808"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_809"))), Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_5"))), !isMyTeamLeft);
}

void UUC_HUD_PVP_TotalHP::InitAvatarInfo(const FString& nick, const FString& guild, bool team_0)
{
	if (team_0)
	{
		NickName[0] = nick;
		GuildName[0] = guild;
	}
	else
	{
		NickName[1] = nick;
		GuildName[1] = guild;
	}
}

void UUC_HUD_PVP_TotalHP::SetAvatarInfo(UTextBlock* nick_name, UTextBlock* guild_name, bool team_0)
{
	//if (nick_name && guild_name)
	if (nick_name)
	{
		FText info_text;
		if (team_0)
		{
			info_text = FText::FromString(NickName[0]);
			nick_name->SetText(info_text);

// 			info_text = FText::FromString(GuildName[0]);
// 			guild_name->SetText(info_text);
		}
		else
		{
			info_text = FText::FromString(NickName[1]);
			nick_name->SetText(info_text);

// 			info_text = FText::FromString(GuildName[1]);
// 			guild_name->SetText(info_text);
		}
	}
}

void UUC_HUD_PVP_TotalHP::SetTextBlockHP(UTextBlock* curr_HP, UTextBlock* max_HP, UTextBlock* percent, bool team_0)
{
	if (IsValid(curr_HP) && IsValid(max_HP) && IsValid(percent))
	{
		FText hp_text;
		if (team_0)
		{
			hp_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(HPData[0].CurrTotalHP));
			curr_HP->SetText(hp_text);

			hp_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(HPData[0].TotalHPMax));
			max_HP->SetText(hp_text);

			int32 perc = int32(float(HPData[0].CurrTotalHP * 100) / float(HPData[0].TotalHPMax));
			hp_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(perc));
			percent->SetText(hp_text);
		}
		else
		{
			hp_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(HPData[1].CurrTotalHP));
			curr_HP->SetText(hp_text);

			hp_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(HPData[1].TotalHPMax));
			max_HP->SetText(hp_text);

			int32 perc = int32(float(HPData[1].CurrTotalHP * 100) / float(HPData[1].TotalHPMax));
			hp_text = FText::Format(FText::FromString("{0}"), FText::AsNumber(perc));
			percent->SetText(hp_text);
		}
	}
}

void UUC_HUD_PVP_TotalHP::SetProgressBarHP(UProgressBar* back_bar, UProgressBar* real_bar, bool team_0)
{
	if (back_bar && real_bar)
	{
		float percent = 0.0f;
		if (team_0)
		{
			if (0 < HPData[0].TotalHPMax)
			{
				percent = float(HPData[0].PrevTotalHP) / float(HPData[0].TotalHPMax);
				back_bar->SetPercent(percent);

				percent = float(HPData[0].CurrTotalHP) / float(HPData[0].TotalHPMax);
				real_bar->SetPercent(percent);
			}
			else
			{
				back_bar->SetPercent(0.0f);
				real_bar->SetPercent(0.0f);
			}
		}
		else
		{
			if (0 < HPData[1].TotalHPMax)
			{
				percent = float(HPData[1].PrevTotalHP) / float(HPData[1].TotalHPMax);
				back_bar->SetPercent(percent);

				percent = float(HPData[1].CurrTotalHP) / float(HPData[1].TotalHPMax);
				real_bar->SetPercent(percent);
			}
			else
			{
				back_bar->SetPercent(0.0f);
				real_bar->SetPercent(0.0f);
			}
		}
	}
}

void UUC_HUD_PVP_TotalHP::ChangeHP(int32 curr_hp, int32 max_hp, bool team_0)
{
	if (team_0)
	{
		HPData[0].ChangedHP = true;
		HPData[0].CurrTotalHP = curr_hp;
		HPData[0].TotalHPMax = max_hp;
	}
	else
	{
		HPData[1].ChangedHP = true;
		HPData[1].CurrTotalHP = curr_hp;
		HPData[1].TotalHPMax = max_hp;
	}
}

bool UUC_HUD_PVP_TotalHP::SmoothlyChangeHP(int32& prev_hp, int32 curr_hp, float delta_time)
{
	float speed = float(prev_hp - curr_hp) / HP_CHANGE_TIME;

	if(0.0f < speed)
	{
		if (HP_CHANGE_SPEED_MIN > speed)
		{
			speed = HP_CHANGE_SPEED_MIN;
		}

		prev_hp = prev_hp - int32(speed * delta_time);
		if (prev_hp <= curr_hp)
		{
			prev_hp = curr_hp;
			return false;
		}
		return true;
	}
	else
	{
		prev_hp = curr_hp;
	}
	return false;
}

void UUC_CS_Topbar::NativeConstruct()
{
	Super::NativeConstruct();

	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	for (TPair<int16, FRTS_PLAYER> Player : RGameInstance->RealTimeModManager->RTSPlayerList)
	{
		if (RGameInstance->HttpClient->GetKID() == Player.Value.kId)
		{
			if (IsValid(MyAvatarName))
				MyAvatarName->SetText(FText::FromString(Player.Value.nick));
		}
		else
		{
			if (IsValid(EnemyAvatarName))
				EnemyAvatarName->SetText(FText::FromString(Player.Value.nick));
		}
	}
	
	UHUD_RaidThreeControl* parentwidget = Cast<UHUD_RaidThreeControl>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Raid_Three));
	if (IsValid(parentwidget))
	{
		parentwidget->OnColosseum_PointChanged.Unbind();
		parentwidget->OnColosseum_PointChanged.BindUObject(this , &UUC_CS_Topbar::Callback_CSPointChanged);
	}
}

void UUC_CS_Topbar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsVisible())
		return;

	if (MyElapsedTime < Duration)
	{
		MyElapsedTime += InDeltaTime;
		if (MyElapsedTime >= Duration)
			MyElapsedTime = Duration;

		float result = FMath::Lerp<float , float>(MyInitialPercent , MyTargetProgressbar->Percent , MyElapsedTime / Duration);
		MyCurrentProgressbar->SetPercent(result);
	}

	if (EnemyElapsedTime < Duration)
	{
		EnemyElapsedTime += InDeltaTime;
		if (EnemyElapsedTime >= Duration) 
			EnemyElapsedTime = Duration;

		float result = FMath::Lerp<float, float>(EnemyInitialPercent, EnemyTargetProgressbar->Percent, EnemyElapsedTime / Duration);
		EnemyCurrentProgressbar->SetPercent(result);
	}
}

void UUC_CS_Topbar::Callback_CSPointChanged(int32 MyPoint, int32 EnemyPoint)
{
	if (IsValid(MyPointText))
		MyPointText->SetText(FText::AsNumber(MyPoint));

	if (IsValid(EnemyPointText))
		EnemyPointText->SetText(FText::AsNumber(EnemyPoint));

	float RequiredMedalCount = RGAMEINSTANCE(this)->RealTimeModManager->RTSInfo.modId == FString::FromInt(1) ? 6 : 21;

	float MyPercent = (float)MyPoint / RequiredMedalCount;
	float EnemyPercent = (float)EnemyPoint / RequiredMedalCount;

	if (IsValid(MyTargetProgressbar))
	{
		MyElapsedTime = 0.f;
		MyInitialPercent = MyTargetProgressbar->Percent;
		MyTargetProgressbar->SetPercent(MyPercent);
	}
		

	if (IsValid(EnemyTargetProgressbar))
	{
		EnemyElapsedTime = 0.f;
		EnemyInitialPercent = EnemyTargetProgressbar->Percent;
		EnemyTargetProgressbar->SetPercent(EnemyPercent);
	}
		


}
