// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_ControlBlock.h"

#include "GlobalIntegrated.h"
//
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "Lobby/TowerLobbyLevelScriptActor.h"
#include "CustomActors/RHeroGacha.h"
#include "Engine/UserInterfaceSettings.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"

void UUP_ControlBlock::NativeConstruct()
{
	Super::NativeConstruct();

	for (UImage* img : ImageList)
	{
		img->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(BackImage))
	{
		BackImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(BackButton))
	{
		//BackButton->SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f, 0.5f));
		BackButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(DlgPanel))
	{
		DlgPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(DlgSubPanel))
	{
		DlgSubPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	//URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	//
	//{
	//	if (!IsValid(ContentsStringAnimation))
	//	{
	//		ContentsStringAnimation = NewObject< UDialogueStringAnimation>((UObject*)GetTransientPackage(), UDialogueStringAnimation::StaticClass());
	//		if (IsValid(ContentsStringAnimation))
	//		{
	//			ContentsStringAnimation->Init(RGameInstance);
	//			ContentsStringAnimation->ContentsAnimEvent.AddUniqueDynamic(this, &UUP_ControlBlock::CallbackStringAnimation);
	//		}
	//	}
	//}

	PlayAni(TEXT("NewAnimation_1"), false, 0);
	PlayAni(TEXT("NewAnimation_3"), false, 0);

	if (Button_Skip)
	{
		Button_Skip->OnClicked.RemoveDynamic(this, &UUP_ControlBlock::OnClickSkipButton);
		Button_Skip->OnClicked.AddDynamic(this, &UUP_ControlBlock::OnClickSkipButton);
	}

	USlateBlueprintLibrary::ScreenToViewport(GetWorld(), UWidgetLayoutLibrary::GetViewportSize(this), ScreenSize);
}

void UUP_ControlBlock::NativeDestruct()
{
	if (Button_Skip)
	{
		Button_Skip->OnClicked.RemoveDynamic(this, &UUP_ControlBlock::OnClickSkipButton);
	}

	Super::NativeDestruct();
}

void UUP_ControlBlock::CallbackStringAnimation(FString visibleString)
{
	//if (IsValid(DlgText))
	//{
	//	FText contents = FText::FromString(visibleString);
	//	DlgText->SetText(contents);
	//}
}

void UUP_ControlBlock::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bIndicatorStart)
	{
		DelayIndicator -= InDeltaTime;
		if (DelayIndicator <= 0.0f)
		{
			UUtilFunctionLibrary::ShowIndicator(IndicatorText, 30.0f, true);
			bIndicatorStart = false;

		}
	}

	if (bStartAni)
	{
		DelayAni -= InDeltaTime;
		if (DelayAni <= 0.0f)
		{
			//if (bButtonShow)
			//{
			//	if (IsValid(DlgSubPanel))
			//	{
			//		DlgSubPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			//	}
			//}
			//else
			{
				PlayAni(TEXT("Splash_Appear"));
			}
			
			bStartAni = false;
		}
	}

	if (bFade)
	{
		CurFadeTime += InDeltaTime;
		if (CurFadeTime >= FadeTime)
		{
			CurFadeTime = FadeTime;
			bFade = false;
		}

		float fValue = CurFadeTime / FadeTime;
		float curValue = 0.0f;
		if(FadeType == 0)
			curValue = FMath::Lerp<float>(0.0f, 1.0f, fValue);
		else
			curValue = FMath::Lerp<float>(1.0f, 0.0f, fValue);

		for (UImage* img : ImageList)
		{
			if (IsValid(img))
			{
				img->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, curValue));
			}				
		}
	}

	if (bHighlightStart)
	{
		HighlightDelayAni -= InDeltaTime;
		if (HighlightDelayAni <= 0.0f)
		{
			CurHighlightAniTime += InDeltaTime;
			if (CurHighlightAniTime >= HighlightAniTime)
			{
				bHighlightStart = false;
			}

			if (IsValid(BackImage))
			{
				UMaterialInstanceDynamic* material = BackImage->GetDynamicMaterial();
				if (IsValid(material))
				{
					FName transitionName = "Transition";

					float curTime = CurHighlightAniTime / HighlightAniTime;
					material->SetScalarParameterValue(transitionName, curTime);
				}
			}
		}
	}

	if (CachedList.Num() > 0)
	{
		if (CachedList[0].IsValid())
		{
			const FGeometry& geometry = CachedList[0]->GetCachedGeometry();
			if (!geometry.Size.IsNearlyZero())
			{
				TArray<UWidget*> tempList;
				for (TWeakObjectPtr<UWidget> widget : CachedList)
				{
					if (widget.IsValid())
					{
						tempList.Emplace(widget.Get());
					}
				}

				if (!BlockInfo.Refresh)
				{
					CachedList.Empty();
				}

				FIntRect rect = GetRect(tempList, bTempMaterial);
				FIntRect rectBorder = GetRect(tempList, false);

				BlockInfo.Pos.X = (float)rect.Min.X;
				BlockInfo.Pos.Y = (float)rect.Min.Y;
				BlockInfo.Size.X = (float)(rect.Max.X - rect.Min.X);
				BlockInfo.Size.Y = (float)(rect.Max.Y - rect.Min.Y);

				BorderBlockInfo.Pos.X = (float)rectBorder.Min.X;
				BorderBlockInfo.Pos.Y = (float)rectBorder.Min.Y;
				BorderBlockInfo.Size.X = (float)(rectBorder.Max.X - rectBorder.Min.X);
				BorderBlockInfo.Size.Y = (float)(rectBorder.Max.Y - rectBorder.Min.Y);

				if (bTempMaterial)
					SetPositionMaterialPrivate((int32)BlockInfo.Pos.X, (int32)BlockInfo.Pos.Y, (int32)BlockInfo.Size.X, (int32)BlockInfo.Size.Y, BlockInfo.OffsetPos.X, BlockInfo.OffsetPos.Y, BlockInfo.OffsetSize.X, BlockInfo.OffsetSize.Y, true);
				else
					SetPositionPrivate((int32)BlockInfo.Pos.X, (int32)BlockInfo.Pos.Y, (int32)BlockInfo.Size.X, (int32)BlockInfo.Size.Y, BlockInfo.OffsetPos.X, BlockInfo.OffsetPos.Y, BlockInfo.OffsetSize.X, BlockInfo.OffsetSize.Y, true);

				SetPositionBorderPrivate((int32)BorderBlockInfo.Pos.X, (int32)BorderBlockInfo.Pos.Y, (int32)BorderBlockInfo.Size.X, (int32)BorderBlockInfo.Size.Y, BlockInfo.OffsetPos.X, BlockInfo.OffsetPos.Y, BlockInfo.OffsetSize.X, BlockInfo.OffsetSize.Y);

				//SetPosWidgets(tempList, bTempMaterial);
			}
		}
	}

	if (bBlockAni)
	{
		CurBlockAniTime += InDeltaTime;
		if (CurBlockAniTime >= BlockAniTime)
		{
			CurBlockAniTime = BlockAniTime;
			bBlockAni = false;
		}

		float fValue = CurBlockAniTime / BlockAniTime;

		FVector2D pos = GetPos(SourceBlockInfo, BlockInfo, fValue);
		FVector2D size = GetSize(SourceBlockInfo, BlockInfo, fValue);
		FVector2D offsetpos = GetOffsetPos(SourceBlockInfo, BlockInfo, fValue);
		FVector2D offsetsize = GetOffsetSize(SourceBlockInfo, BlockInfo, fValue);

		FVector2D borderpos = GetPos(SourceBorderBlockInfo, BorderBlockInfo, fValue);
		FVector2D bordersize = GetSize(SourceBorderBlockInfo, BorderBlockInfo, fValue);
		FVector2D borderoffsetpos = GetOffsetPos(SourceBorderBlockInfo, BorderBlockInfo, fValue);
		FVector2D borderoffsetsize = GetOffsetSize(SourceBorderBlockInfo, BorderBlockInfo, fValue);

		if (bTempMaterial)
			SetPositionMaterialPrivate((int32)pos.X, (int32)pos.Y, (int32)size.X, (int32)size.Y, offsetpos.X, offsetpos.Y, offsetsize.X, offsetsize.Y, true);
		else
			SetPositionPrivate((int32)pos.X, (int32)pos.Y, (int32)size.X, (int32)size.Y, offsetpos.X, offsetpos.Y, offsetsize.X, offsetsize.Y, true);

		SetPositionBorderPrivate((int32)borderpos.X, (int32)borderpos.Y, (int32)bordersize.X, (int32)bordersize.Y, borderoffsetpos.X, borderoffsetpos.Y, borderoffsetsize.X, borderoffsetsize.Y);
	}

	if (CachedButtonList.Num() > 0)
	{
		if (CachedButtonList[0].IsValid())
		{
			const FGeometry& geometry = CachedButtonList[0]->GetCachedGeometry();
			if (!geometry.Size.IsNearlyZero())
			{
				TArray<UWidget*> tempList;
				for (TWeakObjectPtr<UWidget> widget : CachedButtonList)
				{
					if (widget.IsValid())
					{
						tempList.Emplace(widget.Get());
					}
				}				

				if (!ButtonBlockInfo.Refresh)
				{
					CachedButtonList.Empty();
				}

				FIntRect rect = GetRect(tempList, false);

				ButtonBlockInfo.Pos.X = (float)rect.Min.X;
				ButtonBlockInfo.Pos.Y = (float)rect.Min.Y;
				ButtonBlockInfo.Size.X = (float)(rect.Max.X - rect.Min.X);
				ButtonBlockInfo.Size.Y = (float)(rect.Max.Y - rect.Min.Y);

				SetPositionButtonPrivate((int32)ButtonBlockInfo.Pos.X, (int32)ButtonBlockInfo.Pos.Y, (int32)ButtonBlockInfo.Size.X, (int32)ButtonBlockInfo.Size.Y, ButtonBlockInfo.OffsetPos.X, ButtonBlockInfo.OffsetPos.Y, ButtonBlockInfo.OffsetSize.X, ButtonBlockInfo.OffsetSize.Y);

				//SetPosWidgetsButton(tempList, ButtonBlockInfo.OffsetPos.X, ButtonBlockInfo.OffsetPos.Y, ButtonBlockInfo.OffsetSize.X, ButtonBlockInfo.OffsetSize.Y);
			}
		}
	}

	if (bButtonAni)
	{
		CurButtonAniTime += InDeltaTime;
		if (CurButtonAniTime >= ButtonAniTime)
		{
			CurButtonAniTime = ButtonAniTime;
			bButtonAni = false;
		}

		float fValue = CurButtonAniTime / ButtonAniTime;

		FVector2D pos = GetPos(SourceButtonBlockInfo, ButtonBlockInfo, fValue);
		FVector2D size = GetSize(SourceButtonBlockInfo, ButtonBlockInfo, fValue);
		FVector2D offsetpos = GetOffsetPos(SourceButtonBlockInfo, ButtonBlockInfo, fValue);
		FVector2D offsetsize = GetOffsetSize(SourceButtonBlockInfo, ButtonBlockInfo, fValue);

		SetPositionButtonPrivate((int32)pos.X, (int32)pos.Y, (int32)size.X, (int32)size.Y, offsetpos.X, offsetpos.Y, offsetsize.X, offsetsize.Y);
	}

	if (bCardRefresh)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		UWorld* GameWorld = RGameInstance->GetWorld();

		ATowerLobbyLevelScriptActor* TowerLobbyLSA = Cast<ATowerLobbyLevelScriptActor>(GameWorld->GetLevelScriptActor());

		ARHeroGachaCard* pCard = nullptr;

		if(TowerLobbyLSA->HeroGachaActor)
		{
			if (TowerLobbyLSA->HeroGachaActor->HeroCardArray.IsValidIndex(0))
				pCard = TowerLobbyLSA->HeroGachaActor->HeroCardArray[0];

			ALobbyPlayerController * PlayerController = UUtilFunctionLibrary::GetLobbyPlayerController(GameWorld);
			FVector2D TempLoca(0, 0);

			if (PlayerController && pCard && pCard->IsValidLowLevel())
				PlayerController->ProjectWorldLocationToScreen(pCard->GetTargetLocation(), TempLoca);

			const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));

			TempLoca /= ViewportScale;

			int32 x, y, width, height;
			width = 200;
			height = 300;

			x = TempLoca.X - (width / 2);
			y = TempLoca.Y - (height / 2);

			SetPositionButtonPrivate(x, y, width, height, 0, 0, 0, 0);
			SetPositionPrivate(x, y, width, height, 0, 0, 0, 0, true);
		}
	}
}

void UUP_ControlBlock::Clear()
{
	CachedList.Empty();
	CachedButtonList.Empty();

	SetDlgPanelPos(0.0f, 0.0f);
}

void UUP_ControlBlock::SetDlgPanelPos(float InOffsetX, float InOffsetY)
{
	if (IsValid(DlgPanel))
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DlgPanel->Slot);
		if (IsValid(CanvasSlot))
		{
			CanvasSlot->SetPosition(FVector2D(InOffsetX, InOffsetY));
		}
	}

	if (IsValid(DlgSubPanel))
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DlgSubPanel->Slot);
		if (IsValid(CanvasSlot))
		{
			CanvasSlot->SetPosition(FVector2D(InOffsetX, InOffsetY));
		}
	}
}

void UUP_ControlBlock::SetPositionForce(int32 InX, int32 InY, int32 InWidth, int32 InHeight, bool InbMaterial)
{
	bTempMaterial = InbMaterial;

	CachedList.Empty();

	if (InbMaterial)
	{
		SetPositionMaterial(InX, InY, InWidth, InHeight);
	}
	else
	{
		SetPosition(InX, InY, InWidth, InHeight);
	}
}

void UUP_ControlBlock::SetPosWidget(UWidget* InWidget, bool InbMaterial)
{
	if (IsValid(InWidget))
	{
		const FGeometry& geometry = InWidget->GetCachedGeometry();

		FVector2D outPos = geometry.AbsolutePosition;
		FVector2D outPos1;
		FVector2D localPos = FVector2D(0.0f, 0.0f);

		USlateBlueprintLibrary::LocalToViewport(GetWorld(), geometry, localPos, outPos, outPos1);

		if(InbMaterial)
			SetPositionMaterial((int32)outPos.X, (int32)outPos.Y, (int32)geometry.GetDrawSize().X, (int32)geometry.GetDrawSize().Y, true);
		else
			SetPosition((int32)outPos1.X, (int32)outPos1.Y, (int32)geometry.Size.X, (int32)geometry.Size.Y, true);
	}
}

void UUP_ControlBlock::SetPosWidgetsButtonCachedList(TArray<UWidget*> InWidgetList)
{
	CachedButtonList.Empty();
	for (UWidget* widet : InWidgetList)
	{
		if (IsValid(widet))
			CachedButtonList.Emplace(widet);
	}
}

void UUP_ControlBlock::SetPosWidgetsButton(TArray<UWidget*> InWidgetList, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize)
{
	bButtonShow = true;

	ButtonBlockInfo.OffsetPos.X = InOffsetX;
	ButtonBlockInfo.OffsetPos.Y = InOffsetY;
	ButtonBlockInfo.OffsetSize.X = InOffsetXSize;
	ButtonBlockInfo.OffsetSize.Y = InOffsetYSize;

	if (InWidgetList.Num() > 0)
	{
 		if (IsValid(InWidgetList[0]))
		{
			const FGeometry& geometry = InWidgetList[0]->GetCachedGeometry();

			if (geometry.Size.IsNearlyZero())
			{
				CachedButtonList.Empty();
				for (UWidget* widet : InWidgetList)
				{
					if (IsValid(widet))
						CachedButtonList.Emplace(widet);
				}
			}
			else
			{
				FIntRect rect;
				rect = GetRect(InWidgetList, false);

				SetPositionButton(rect.Min.X, rect.Min.Y, (rect.Max.X - rect.Min.X), (rect.Max.Y - rect.Min.Y), InOffsetX, InOffsetY, InOffsetXSize, InOffsetYSize);
			}
		}
	}
}

void UUP_ControlBlock::SetPosWidgetsCachedList(TArray<UWidget*> InWidgetList)
{
	CachedList.Empty();
	for (UWidget* widet : InWidgetList)
	{
		if (IsValid(widet))
			CachedList.Emplace(widet);
	}
}

FIntRect UUP_ControlBlock::GetRect(TArray<UWidget*> InWidgetList, bool InbMaterial)
{
	FIntRect rect;

	rect.Min.X = 0;
	rect.Min.Y = 0;
	rect.Max.X = 0;
	rect.Max.Y = 0;

	if (InWidgetList.Num() > 0)
	{
		if (IsValid(InWidgetList[0]))
		{
			const FGeometry& geometry = InWidgetList[0]->GetCachedGeometry();

			FVector2D outPos = geometry.AbsolutePosition;
			FVector2D outPos1;
			FVector2D localPos = FVector2D(0.0f, 0.0f);

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InWidgetList[0]);
			if (IsValid(CanvasSlot))
			{
				localPos = CanvasSlot->GetPosition();
			}

			USlateBlueprintLibrary::LocalToViewport(GetWorld(), geometry, localPos, outPos, outPos1);

			if (InbMaterial)
			{
				rect.Min.X = (int32)outPos.X;
				rect.Min.Y = (int32)outPos.Y;
				rect.Max.X = (int32)(outPos.X + geometry.GetDrawSize().X);
				rect.Max.Y = (int32)(outPos.Y + geometry.GetDrawSize().Y);
			}
			else
			{
				rect.Min.X = (int32)outPos1.X;
				rect.Min.Y = (int32)outPos1.Y;
				rect.Max.X = (int32)(outPos1.X + geometry.Size.X);
				rect.Max.Y = (int32)(outPos1.Y + geometry.Size.Y);
			}

			for (int32 i = 1; i < InWidgetList.Num(); ++i)
			{
				if (IsValid(InWidgetList[i]))
				{
					const FGeometry& geometry1 = InWidgetList[i]->GetCachedGeometry();
					FIntRect rect1;
					FVector2D outPos2 = geometry1.AbsolutePosition;
					FVector2D outPos3;
					FVector2D localPos1 = FVector2D(0.0f, 0.0f);

					UCanvasPanelSlot* CanvasSlot1 = Cast<UCanvasPanelSlot>(InWidgetList[i]);
					if (IsValid(CanvasSlot1))
					{
						localPos1 = CanvasSlot1->GetPosition();
					}

					USlateBlueprintLibrary::LocalToViewport(GetWorld(), geometry1, localPos1, outPos2, outPos3);

					if (InbMaterial)
					{
						rect1.Min.X = (int32)outPos2.X;
						rect1.Min.Y = (int32)outPos2.Y;
						rect1.Max.X = (int32)(outPos2.X + geometry1.GetDrawSize().X);
						rect1.Max.Y = (int32)(outPos2.Y + geometry1.GetDrawSize().Y);
					}
					else
					{
						rect1.Min.X = (int32)outPos3.X;
						rect1.Min.Y = (int32)outPos3.Y;
						rect1.Max.X = (int32)(outPos3.X + geometry1.Size.X);
						rect1.Max.Y = (int32)(outPos3.Y + geometry1.Size.Y);
					}

					if (rect.Min.X > rect1.Min.X)
						rect.Min.X = rect1.Min.X;
					if (rect.Min.Y > rect1.Min.Y)
						rect.Min.Y = rect1.Min.Y;
					if (rect.Max.X < rect1.Max.X)
						rect.Max.X = rect1.Max.X;
					if (rect.Max.Y < rect1.Max.Y)
						rect.Max.Y = rect1.Max.Y;
				}
			}
		}
	}

	return rect;
}

void UUP_ControlBlock::SetPosWidgets(TArray<UWidget*> InWidgetList, bool InbMaterial)
{
	bTempMaterial = InbMaterial;	

	if (InWidgetList.Num() > 0)
	{
		if (IsValid(InWidgetList[0]))
		{
			const FGeometry& geometry = InWidgetList[0]->GetCachedGeometry();

			if (geometry.Size.IsNearlyZero())
			{
				CachedList.Empty();
				for (UWidget* widet : InWidgetList)
				{
					if(IsValid(widet))
						CachedList.Emplace(widet);
				}
			}
			else
			{
				FIntRect rect;
				FIntRect rectBorder;

				rect = GetRect(InWidgetList, InbMaterial);
				rectBorder = GetRect(InWidgetList, false);

				if (InbMaterial)
					SetPositionMaterial(rect.Min.X, rect.Min.Y, (rect.Max.X - rect.Min.X), (rect.Max.Y - rect.Min.Y), true);
				else
					SetPosition(rect.Min.X, rect.Min.Y, (rect.Max.X - rect.Min.X), (rect.Max.Y - rect.Min.Y), true);

				SetPositionBorder(rectBorder.Min.X, rectBorder.Min.Y, (rectBorder.Max.X - rectBorder.Min.X), (rectBorder.Max.Y - rectBorder.Min.Y));
			}
		}
	}	
}

void UUP_ControlBlock::SetPositionMaterial(int32 InX, int32 InY, int32 InWidth, int32 InHeight, bool bOpen)
{
	BlockInfo.Pos.X = (float)InX;
	BlockInfo.Pos.Y = (float)InY;
	BlockInfo.Size.X = (float)InWidth;
	BlockInfo.Size.Y = (float)InHeight;

	SetPositionMaterialPrivate(InX, InY, InWidth, InHeight, BlockInfo.OffsetPos.X, BlockInfo.OffsetPos.Y, BlockInfo.OffsetSize.X, BlockInfo.OffsetSize.Y, bOpen);
}

void UUP_ControlBlock::SetButtonHide()
{
	if (IsValid(BackButton))
	{
		BackButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_ControlBlock::SetPositionButton(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize)
{
	bButtonShow = true;

	ButtonBlockInfo.Pos.X = (float)InX;
	ButtonBlockInfo.Pos.Y = (float)InY;
	ButtonBlockInfo.Size.X = (float)InWidth;
	ButtonBlockInfo.Size.Y = (float)InHeight;

	SetPositionButtonPrivate(InX, InY, InWidth, InHeight, InOffsetX, InOffsetY, InOffsetXSize, InOffsetYSize);
}

void UUP_ControlBlock::SetButtonFull()
{
	CachedButtonList.Empty();

	if (IsValid(BackButton))
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
		BackButton->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(BackButton->Slot);
		if (IsValid(CanvasSlot))
		{
			CanvasSlot->SetPosition(FVector2D(0.0f, 0.0f));
			CanvasSlot->SetSize(FVector2D(ScreenSize.X, ScreenSize.Y));
		}
	}
}

void UUP_ControlBlock::SetPositionBorder(int32 InX, int32 InY, int32 InWidth, int32 InHeight)
{
	BorderBlockInfo.Pos.X = (float)InX;
	BorderBlockInfo.Pos.Y = (float)InY;
	BorderBlockInfo.Size.X = (float)InWidth;
	BorderBlockInfo.Size.Y = (float)InHeight;

	SetPositionBorderPrivate(InX, InY, InWidth, InHeight, BlockInfo.OffsetPos.X, BlockInfo.OffsetPos.Y, BlockInfo.OffsetSize.X, BlockInfo.OffsetSize.Y);
}

void UUP_ControlBlock::SetPosition(int32 InX, int32 InY, int32 InWidth, int32 InHeight, bool bOpen)
{
	BlockInfo.Pos.X = (float)InX;
	BlockInfo.Pos.Y = (float)InY;
	BlockInfo.Size.X = (float)InWidth;
	BlockInfo.Size.Y = (float)InHeight;

	SetPositionPrivate(InX, InY, InWidth, InHeight, BlockInfo.OffsetPos.X, BlockInfo.OffsetPos.Y, BlockInfo.OffsetSize.X, BlockInfo.OffsetSize.Y, bOpen);
}

void UUP_ControlBlock::SetButtonRefresh(bool InRefresh)
{
	ButtonBlockInfo.Refresh = InRefresh;
}

void UUP_ControlBlock::SetBorder(bool InShow)
{
	bBorderShow = InShow;

	if (!bBorderShow)
	{
		if (IsValid(BackBorder))
		{
			BackBorder->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUP_ControlBlock::SetCardRefresh(bool InRefresh)
{
	bCardRefresh = InRefresh;
}

void UUP_ControlBlock::SetRefresh(bool InRefresh)
{
	BlockInfo.Refresh = InRefresh;
}

void UUP_ControlBlock::SetOffsetPos(float InOffsetX, float InOffsetY)
{
	BlockInfo.OffsetPos = FVector2D(InOffsetX, InOffsetY);
	BorderBlockInfo.OffsetPos = BlockInfo.OffsetPos;
}

void UUP_ControlBlock::SetOffsetSize(float InOffsetX, float InOffsetY)
{
	BlockInfo.OffsetSize = FVector2D(InOffsetX, InOffsetY);
	BorderBlockInfo.OffsetSize = BlockInfo.OffsetSize;
}

void UUP_ControlBlock::SetText(FText InText)
{
	//if (bButtonShow)
	//{
	//	if (IsValid(DlgPanel))
	//	{
	//		DlgPanel->SetVisibility(ESlateVisibility::Collapsed);
	//	}

	//	if (IsValid(DlgSubPanel))
	//	{
	//		DlgSubPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//	}
	//}
	//else
	//{
	//	if (bStartAni)
	//	{
	//		if (IsValid(DlgPanel))
	//		{
	//			DlgPanel->SetVisibility(ESlateVisibility::Collapsed);
	//		}
	//	}

	//	if (IsValid(DlgSubPanel))
	//	{
	//		DlgSubPanel->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//}

	if (IsValid(ReturnImage))
	{
		if (bButtonShow)
		{
			ReturnImage->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			ReturnImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (bIndicatorStart)
	{
		IndicatorText = InText;
	}

	if (IsValid(DlgText))
	{
		FText txt = FText::Format(InText, FText::FromString(RGAMEINSTANCE(this)->UserInfo->Avatar.nick));
		DlgText->SetRichText(txt);
		DlgText->SynchronizeProperties();
	}

	if (IsValid(DlgSubText))
	{
		DlgSubText->SetRichText(InText);
		DlgSubText->SynchronizeProperties();
	}

	//if (IsValid(ContentsStringAnimation))
	//{
	//	ContentsStringAnimation->StartStringAnimation(InText.ToString());
	//}
}

void UUP_ControlBlock::ShowAnimation(float InDelay)
{
	DelayAni = InDelay;
	bStartAni = true;

	if (IsValid(DlgPanel))
	{
		DlgPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(DlgSubPanel))
	{
		DlgSubPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	//ShowHighlightEffect(0.5f, InDelay);
}

void UUP_ControlBlock::ShowIndicator(float InDelay)
{
	DelayIndicator = InDelay;
	bIndicatorStart = true;

	if (IsValid(DlgPanel))
	{
		DlgPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(DlgSubPanel))
	{
		DlgSubPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_ControlBlock::HideIndicator()
{
	bIndicatorStart = false;
	UUtilFunctionLibrary::HideIndicator();
}

void UUP_ControlBlock::ShowHighlightEffect(float InAniTime, float InDelay)
{
	bHighlightStart = true;
	HighlightAniTime = InAniTime;
	HighlightDelayAni = InDelay;
	CurHighlightAniTime = 0.0f;
}

void UUP_ControlBlock::ShowDlg(bool bShow)
{
	if (IsValid(DlgPanel))
	{
		if (bShow)
		{
			if (bStartAni)
			{
				DlgPanel->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				DlgPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}			
		}			
		else
		{
			DlgPanel->SetVisibility(ESlateVisibility::Collapsed);
		}			
	}
}

void UUP_ControlBlock::FadeIn(float InFadeTime)
{
	bFade = true;
	FadeType = 0;
	CurFadeTime = 0.0f;
	FadeTime = InFadeTime;
}

void UUP_ControlBlock::FadeOut(float InFadeTime)
{
	bFade = true;
	FadeType = 1;
	CurFadeTime = 0.0f;
	FadeTime = InFadeTime;
}

void UUP_ControlBlock::StartAni(float InAniTime)
{
	StartBlockAni(InAniTime);
	StartButtonAni(InAniTime);
}

void UUP_ControlBlock::NextAni(float InAniTime)
{
	NextBlockAni(InAniTime);
	NextButtonAni(InAniTime);
}

void UUP_ControlBlock::SetButtonShowClear()
{
	bButtonShow = false;
}

void UUP_ControlBlock::StartBlockAni(float InAniTime)
{
	bBlockAni = true;
	CurBlockAniTime = 0.0f;
	BlockAniTime = InAniTime;

	SourceBlockInfo.Pos = FVector2D::ZeroVector;
	SourceBlockInfo.Size = ScreenSize;
	SourceBlockInfo.OffsetPos = FVector2D::ZeroVector;
	SourceBlockInfo.OffsetSize = FVector2D::ZeroVector;

	SourceBorderBlockInfo.Pos = FVector2D::ZeroVector;
	SourceBorderBlockInfo.Size = ScreenSize;
	SourceBorderBlockInfo.OffsetPos = FVector2D::ZeroVector;
	SourceBorderBlockInfo.OffsetSize = FVector2D::ZeroVector;
}

void UUP_ControlBlock::StartButtonAni(float InAniTime)
{
	bButtonAni = true;
	CurButtonAniTime = 0.0f;
	ButtonAniTime = InAniTime;

	SourceButtonBlockInfo.Pos = FVector2D::ZeroVector;
	SourceButtonBlockInfo.Size = ScreenSize;
	SourceButtonBlockInfo.OffsetPos = FVector2D::ZeroVector;
	SourceButtonBlockInfo.OffsetSize = FVector2D::ZeroVector;
}

void UUP_ControlBlock::NextBlockAni(float InAniTime)
{
	bBlockAni = true;
	CurBlockAniTime = 0.0f;
	BlockAniTime = InAniTime;

	SourceBlockInfo.Pos = BlockInfo.Pos;
	SourceBlockInfo.Size = BlockInfo.Size;
	SourceBlockInfo.OffsetPos = BlockInfo.OffsetPos;
	SourceBlockInfo.OffsetSize = BlockInfo.OffsetSize;

	SourceBorderBlockInfo.Pos = BorderBlockInfo.Pos;
	SourceBorderBlockInfo.Size = BorderBlockInfo.Size;
	SourceBorderBlockInfo.OffsetPos = BorderBlockInfo.OffsetPos;
	SourceBorderBlockInfo.OffsetSize = BorderBlockInfo.OffsetSize;
}

void UUP_ControlBlock::NextButtonAni(float InAniTime)
{
	bButtonAni = true;
	CurButtonAniTime = 0.0f;
	ButtonAniTime = InAniTime;

	SourceButtonBlockInfo.Pos = ButtonBlockInfo.Pos;
	SourceButtonBlockInfo.Size = ButtonBlockInfo.Size;
	SourceButtonBlockInfo.OffsetPos = ButtonBlockInfo.OffsetPos;
	SourceButtonBlockInfo.OffsetSize = ButtonBlockInfo.OffsetSize;
}

void UUP_ControlBlock::SetPositionMaterialPrivate(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize, bool bOpen)
{
	float fScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	float posX = (float)InX - InOffsetXSize * fScale + InOffsetX * fScale;
	float posY = (float)InY - InOffsetYSize * fScale + InOffsetY * fScale;
	float width = (float)InWidth + InOffsetXSize * fScale * 2.0f;
	float height = (float)InHeight + InOffsetYSize * fScale * 2.0f;

	FVector2D size = UWidgetLayoutLibrary::GetViewportSize(this);

	if (IsValid(BackImage))
	{
		BackImage->SetVisibility(ESlateVisibility::Visible);

		UMaterialInstanceDynamic* material = BackImage->GetDynamicMaterial();
		if (IsValid(material))
		{
			FName colorName = "BGColorAndOpacity";
			FName rectName = "HighlightRect";
			FName screenName = "ScreenRectangle";

			FLinearColor screencolor;
			screencolor.R = 0.0f;
			screencolor.G = 0.0f;
			screencolor.B = size.X;
			screencolor.A = size.Y;

			material->SetVectorParameterValue(screenName, screencolor);

			FLinearColor color;
			if (bOpen)
			{
				color.R = (float)posX;
				color.G = (float)posY;
				color.B = (float)width;
				color.A = (float)height;
			}
			else
			{
				color.R = (float)0.0f;
				color.G = (float)0.0f;
				color.B = (float)0.0f;
				color.A = (float)0.0f;
			}

			material->SetVectorParameterValue(rectName, color);
		}
	}
}

void UUP_ControlBlock::SetPositionPrivate(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize, bool bOpen)
{
	float posX = (float)InX - InOffsetXSize + InOffsetX;
	float posY = (float)InY - InOffsetYSize + InOffsetY;
	float width = (float)InWidth + InOffsetXSize * 2.0f;
	float height = (float)InHeight + InOffsetYSize * 2.0f;

	for (UImage* img : ImageList)
	{
		if (IsValid(img))
			img->SetVisibility(ESlateVisibility::Visible);
	}

	if (ImageList.Num() == 9)
	{
		//////////////////////////////////////////////////////////////////////////////////////
		// Row 1 Start
		if (IsValid(ImageList[0]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[0]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetSize(FVector2D(posX, posY));
			}
		}

		if (IsValid(ImageList[1]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[1]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetPosition(FVector2D(posX, 0.0f));
				CanvasSlot->SetSize(FVector2D(width, posY));
			}
		}

		if (IsValid(ImageList[2]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[2]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetPosition(FVector2D(posX + width, 0.0f));
				CanvasSlot->SetSize(FVector2D(ScreenSize.X - (posX + width), posY));
			}
		}
		// Row 1 End
		//////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////
		// Row 2 Start
		if (IsValid(ImageList[3]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[3]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetPosition(FVector2D(0.0f, posY));
				CanvasSlot->SetSize(FVector2D(posX, height));
			}
		}

		//------------------------------------------------------------------------------------
		if (IsValid(ImageList[4]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[4]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetPosition(FVector2D(posX, posY));
				CanvasSlot->SetSize(FVector2D(width, height));
			}

			if (bOpen)
				ImageList[4]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
			else
				ImageList[4]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		//------------------------------------------------------------------------------------

		if (IsValid(ImageList[5]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[5]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetPosition(FVector2D(posX + width, posY));
				CanvasSlot->SetSize(FVector2D(ScreenSize.X - (posX + width), height));
			}
		}
		// Row 2 End
		//////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////
		// Row 3 Start
		if (IsValid(ImageList[6]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[6]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetPosition(FVector2D(0, posY + height));
				CanvasSlot->SetSize(FVector2D(posX, ScreenSize.Y - (posY + height)));
			}
		}

		if (IsValid(ImageList[7]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[7]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetPosition(FVector2D(posX, posY + height));
				CanvasSlot->SetSize(FVector2D(width, ScreenSize.Y - (posY + height)));
			}
		}

		if (IsValid(ImageList[8]))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ImageList[8]->Slot);
			if (IsValid(CanvasSlot))
			{
				CanvasSlot->SetPosition(FVector2D(posX + width, posY + height));
				CanvasSlot->SetSize(FVector2D(ScreenSize.X - (posX + width), ScreenSize.Y - (posY + height)));
			}
		}
		// Row 3 End
		//////////////////////////////////////////////////////////////////////////////////////
	}
}

void UUP_ControlBlock::SetPositionBorderPrivate(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize)
{
	if (bBorderShow)
	{
		int32 posX = (float)InX - InOffsetXSize + InOffsetX;
		int32 posY = (float)InY - InOffsetYSize + InOffsetY;
		int32 width = (float)InWidth + InOffsetXSize * 2.0f;
		int32 height = (float)InHeight + InOffsetYSize * 2.0f;

		if (IsValid(BackBorder))
		{
			if (width == 0 || height == 0)
			{
				BackBorder->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				if (BackBorder->GetVisibility() == ESlateVisibility::Collapsed)
					BackBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(BackBorder->Slot);
				if (IsValid(CanvasSlot))
				{
					CanvasSlot->SetPosition(FVector2D(posX, posY));
					CanvasSlot->SetSize(FVector2D(width, height));
				}
			}
		}
	}
}

void UUP_ControlBlock::SetPositionButtonPrivate(int32 InX, int32 InY, int32 InWidth, int32 InHeight, float InOffsetX, float InOffsetY, float InOffsetXSize, float InOffsetYSize)
{
	float x = (float)InX - InOffsetXSize + InOffsetX;
	float y = (float)InY - InOffsetYSize + InOffsetY;
	float width = (float)InWidth + InOffsetXSize * 2.0f;
	float height = (float)InHeight + InOffsetYSize * 2.0f;

	if (IsValid(BackButton))
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
		BackButton->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(BackButton->Slot);
		if (IsValid(CanvasSlot))
		{
			CanvasSlot->SetPosition(FVector2D(x, y));
			CanvasSlot->SetSize(FVector2D(width, height));
		}
	}
}

FVector2D UUP_ControlBlock::GetOffsetPos(FControlBlockInfo& InSource, FControlBlockInfo& InDest, float InRate)
{
	FVector2D rt = FVector2D::ZeroVector;

	if (InRate <= 0.0f)
		InRate = 0.0f;

	if (InRate >= 1.0f)
		InRate = 1.0f;

	rt = InSource.OffsetPos + (InDest.OffsetPos - InSource.OffsetPos) * InRate;

	return rt;
}

FVector2D UUP_ControlBlock::GetOffsetSize(FControlBlockInfo& InSource, FControlBlockInfo& InDest, float InRate)
{
	FVector2D rt = FVector2D::ZeroVector;

	if (InRate <= 0.0f)
		InRate = 0.0f;

	if (InRate >= 1.0f)
		InRate = 1.0f;

	rt = InSource.OffsetSize + (InDest.OffsetSize - InSource.OffsetSize) * InRate;

	return rt;
}

FVector2D UUP_ControlBlock::GetPos(FControlBlockInfo& InSource, FControlBlockInfo& InDest, float InRate)
{
	FVector2D rt = FVector2D::ZeroVector;

	if (InRate <= 0.0f)
		InRate = 0.0f;

	if (InRate >= 1.0f)
		InRate = 1.0f;

	rt = InSource.Pos + (InDest.Pos - InSource.Pos) * InRate;

	return rt;
}

FVector2D UUP_ControlBlock::GetSize(FControlBlockInfo& InSource, FControlBlockInfo& InDest, float InRate)
{
	FVector2D rt = FVector2D::ZeroVector;

	if (InRate <= 0.0f)
		InRate = 0.0f;

	if (InRate >= 1.0f)
		InRate = 1.0f;

	rt = InSource.Size + (InDest.Size - InSource.Size) * InRate;

	return rt;
}

void UUP_ControlBlock::OnClickSkipButton()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if ( RGameInstance->ForceTutorialManager)
	{
		RGameInstance->ForceTutorialManager->SkipUnlockTutorial();
	}
}

void UUP_ControlBlock::ShowSkipButton(bool bShow)
{
	if (Button_Skip)
		Button_Skip->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}