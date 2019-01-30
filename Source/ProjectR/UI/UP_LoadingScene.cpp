// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_LoadingScene.h"

#include "GlobalIntegrated.h"


#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "UtilFunctionIntegrated.h"

#include "GameState/Stage/Spawner/BaseStageInfo.h"


void UUP_LoadingScene::NativeConstruct()
{
	Super::NativeConstruct();

	LoadingAni_Ref = GetAni("Loading_animation");
	if (LoadingAni_Ref)
	{
		PlayAnimation(LoadingAni_Ref);
	}
}

void UUP_LoadingScene::NativeDestruct()
{
	if (LoadingBackground)
	{
		LoadingBackground->SetBrushFromTexture(nullptr);
	}
	LoadingAni_Ref = nullptr;

	Super::NativeDestruct();
}

void UUP_LoadingScene::MatchLoadingType(EGamePlayModeTypeEnum LoadingType)
{
	TArray<FLoadingTableInfo*> TableInfo;
	GetLoadingTypeTableInfo(TableInfo, LoadingType);
	
	//auto ImageSelectedNumber = SelectLoadingImage(TableInfo);
	//if (ImageSelectedNumber < 0)
	//	return;

	//SelectLoadingText(TableInfo, ImageSelectedNumber);

	SaveLoadingInfoToLoadingSceneManager();
}

void UUP_LoadingScene::GetLoadingTypeTableInfo(TArray<FLoadingTableInfo*>& InLoadingTableInfo, EGamePlayModeTypeEnum LoadingType)
{
#if LOG_TIME
	double StartMainTime = FPlatformTime::Seconds();
#endif

	if (LoadingType == EGamePlayModeTypeEnum::VE_PlayTest)
	{
		//auto RGameInstance = RGAMEINSTANCE(this);
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		auto MapTableInfo = UTableManager::GetInstancePtr()->GetMapRow(BaseStageInfo->GetSelectedMapID());
		if (MapTableInfo == nullptr)
			return;
		
		//LoadingTableKeys = MapTableInfo->LoadingTableKeys;
		int32 ImageSelectedNumber = FMath::RandHelper(MapTableInfo->LoadingTableKeys.Num());
		auto LoadingTableInfo = UTableManager::GetInstancePtr()->GetLoadingRow(MapTableInfo->LoadingTableKeys[ImageSelectedNumber]);
		if (LoadingTableInfo == nullptr)
		{
			return;
		}
		TextureID = LoadingTableInfo->TextureID;
	}
	else
	{
		TArray<FName> LoadingTableKeys;
		LoadingTableKeys.Empty();

		TArray<FName> LoadingRowNames = UTableManager::GetInstancePtr()->GetLoadingRowsNames();
		LoadingTableKeys.Reserve(LoadingRowNames.Num());
		for (auto& LoadingRowElem : LoadingRowNames)
		{
			auto LoadingTableInfo = UTableManager::GetInstancePtr()->GetLoadingRow(LoadingRowElem);
			if (LoadingTableInfo == nullptr)
			{
				continue;
			}

			if (LoadingTableInfo->LoadingContentType == LoadingType)
			{
				LoadingTableKeys.Emplace(LoadingRowElem);
			}
		}
		int32 MaxNum = LoadingTableKeys.Num();
		if (MaxNum > 0)
		{
			int32 ImageSelectedNumber = FMath::RandHelper(MaxNum);
			auto LoadingTableInfo = UTableManager::GetInstancePtr()->GetLoadingRow(LoadingTableKeys[ImageSelectedNumber]);
			if (LoadingTableInfo == nullptr)
			{
				return;
			}
			TextureID = LoadingTableInfo->TextureID;
		}
		else
		{
			MatchLoadingType(EGamePlayModeTypeEnum::VE_Campaign);
			return;
		}
		
	}
	
	SetLoadingImage(TextureID);

#if LOG_TIME
	double EndTime = FPlatformTime::Seconds();
	double PrintTime = EndTime - StartMainTime;
	UE_LOG(LogGamePlay, Log, TEXT("%f ____UUP_LoadingScene::GetLoadingTypeTableInfo()"), PrintTime);
#endif

	/*for (const auto& LoadingTableKeyElem : LoadingTableKeys)
	{
		auto LoadingTableInfo = UTableManager::GetInstancePtr()->GetLoadingRow(LoadingTableKeyElem);
		if (LoadingTableInfo == nullptr)
		{
			continue;
		}

		InLoadingTableInfo.Emplace(LoadingTableInfo);
	}*/
}

//int32 UUP_LoadingScene::SelectLoadingImage(TArray<FLoadingTableInfo*>& InLoadingTableInfo)
//{
//	int32 MaxNum = InLoadingTableInfo.Num();
//	int32 ImageSelectedNumber = FMath::RandHelper(MaxNum);
//	if (InLoadingTableInfo.IsValidIndex(ImageSelectedNumber) == false)
//		return -1;
//
//	auto LoadingTableInfo = InLoadingTableInfo[ImageSelectedNumber];
//	if (LoadingTableInfo == nullptr)
//		return -1;
//
//	TextureID = LoadingTableInfo->TextureID;
//	SetLoadingImage(TextureID);
//
//	return ImageSelectedNumber;
//}

//void UUP_LoadingScene::SelectLoadingText(TArray<FLoadingTableInfo*>& InLoadingTableInfo, const int32 ImageSelectedNumber)
//{
//	if (InLoadingTableInfo.IsValidIndex(ImageSelectedNumber) == false)
//		return;
//
//	auto LoadingTableInfo = InLoadingTableInfo[ImageSelectedNumber];
//	if (LoadingTableInfo == nullptr)
//		return;
//
//	int32 LoadintTextsNum		= LoadingTableInfo->LoadingTexts.Num();
//	int32 TextSelectedNumber	= UKismetMathLibrary::RandomInteger(LoadintTextsNum);
//
//	if (LoadingTableInfo->LoadingTexts.IsValidIndex(TextSelectedNumber) == false)
//		return;
//
//	TextID = LoadingTableInfo->LoadingTexts[TextSelectedNumber];
//	SetLoadingText(TextID);
//}

void UUP_LoadingScene::SetLoadingImage(const FName InTextureID)
{
	if (InTextureID.IsNone())
		return;

	if (LoadingBackground)
	{
		UTexture2D* BrushTexture = UUtilFunctionLibrary::GetTexture2DFromTable(InTextureID);
		LoadingBackground->SetBrushFromTexture( BrushTexture );
	}
}

void UUP_LoadingScene::SetLoadingText(const FName InTextID)
{
	if (InTextID.IsNone())
		return;

	if (Text_Description)
		Text_Description->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Common, InTextID));
}

void UUP_LoadingScene::SaveLoadingInfoToLoadingSceneManager()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance->LoadingSceneManager != nullptr, TEXT("%s[ LoadingSceneManager Is Empty ]"), __FUNCTION__);
	RGameInstance->LoadingSceneManager->SetLoadingImageID(TextureID);
	RGameInstance->LoadingSceneManager->SetLoadingTextID(TextID);
}

//void UUP_LoadingScene::Temp()
//{
//	LoadingBackground->SetBrushFromTexture(nullptr);
//}

void UUP_LoadingScene::SetProgressBarPercent(float InValue)
{
	if (Overlay_LoadingBar)
	{
		if (Overlay_LoadingBar->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
		{
			Overlay_LoadingBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (LoadingBar)
	{
		//if(LoadingBar->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
		//	LoadingBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		LoadingBar->SetPercent(InValue);

		if (Text_CurrentSuccessValue)
		{
			//if(Text_CurrentSuccessValue->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
			//	Text_CurrentSuccessValue->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			FString ValueText = FString::SanitizeFloat(InValue * 100);
			int32 ValueInt = FMath::Clamp<int32>(InValue*100 + 1, 0, 100);
			Text_CurrentSuccessValue->SetText(FText::AsNumber(ValueInt));
		}
	}
}