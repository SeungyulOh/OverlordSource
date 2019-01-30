// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RenderFunctionLibrary.h"
#include "GlobalIntegrated.h"

#include "Character/Components/CustomChangeMaterialComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"
#include "Table/MapPostProcessSettingInfo.h"

#include "ContentGameMode/Modes/StageGameMode.h"
#include "CustomActors/RSceneCapturer.h"

#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"

#include "ShowFlags.h"

#define USE_NEW_DARKEN 1

#if USE_NEW_DARKEN
//by danzang
//bool URenderFunctionLibrary::Darken = false;
void URenderFunctionLibrary::SetDarken( bool bDarken)
{	
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	UGameViewportClient* gameviewportclient = GameWorld->GetGameInstance()->GetGameViewportClient();
	if (gameviewportclient)
	{
		gameviewportclient->ShowNonMovableObjects(!bDarken, GameWorld);
		//gameviewportclient->ShowLandScapeComponents(!bDarken, InWorld);
		gameviewportclient->ShowTaggedActors( FName("ApplyDarken"), !bDarken, GameWorld);
	}
}

void URenderFunctionLibrary::ShowUntaggedActors(FName tagforignoring, bool show)
{	
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	UGameViewportClient* gameviewportclient = GameWorld->GetGameInstance()->GetGameViewportClient();
	if (gameviewportclient)
		gameviewportclient->ShowUntaggedActors(tagforignoring, show, GameWorld);
}

void URenderFunctionLibrary::IgnoreDarkenTarget(AActor* Target, bool bIgnore)
{
	if (!IsValid(Target)) return;
	if(bIgnore == true)
		Target->Tags.AddUnique(FName("IgnoreDarken"));
	else Target->Tags.Remove(FName("IgnoreDarken"));
}
#else
void URenderFunctionLibrary::SetDarken(float DarkenScale )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	UMaterialParameterCollection* ParameterCollection = Cast< UMaterialParameterCollection >(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL, TEXT("MaterialParameterCollection'/Game/Shader/ParamCollections/CH_Light_Params.CH_Light_Params'" ) ) );
	if (ParameterCollection != nullptr )
		UKismetMaterialLibrary::SetScalarParameterValue(GameWorld, ParameterCollection, TEXT("DarkenScale"), DarkenScale );
}


void URenderFunctionLibrary::IgnoreDarkenTarget(AActor* Target, bool bIgnore)
{
	if (!IsValid(Target))
		return;


	auto Target_RenderEntityComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Target);
	if (IsValid(Target_RenderEntityComponent))
	{
		UCustomChangeMaterialComponent* CustomMaterialComponent = Target_RenderEntityComponent->GetCustomMaterialComponent();
		if (IsValid(CustomMaterialComponent))
		{
			CustomMaterialComponent->SetCustomMaterialScalarParameterValue(TEXT("UseDarkenScale"), bIgnore ? 0.0f : 1.0f);
		}
	}
}
#endif

void URenderFunctionLibrary::SetEngineFlags(const FString cmd, bool state)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

	int32 FlagIndex = FEngineShowFlags::FindIndexByName(*cmd);
	if (FlagIndex == -1) return;
	FEngineShowFlags* EngineShowFlags = RGameInstance->GetGameViewportClient()->GetEngineShowFlags();
	EngineShowFlags->SetSingleFlag(FlagIndex, state);
}

bool URenderFunctionLibrary::GetEngineFlags(const FString cmd)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

	int32 FlagIndex = FEngineShowFlags::FindIndexByName(*cmd);
	if (FlagIndex == -1) return false;
	FEngineShowFlags* EngineShowFlags = RGameInstance->GetGameViewportClient()->GetEngineShowFlags();
	return EngineShowFlags->GetSingleFlag(FlagIndex);
}

void URenderFunctionLibrary::EnableDOF(bool InEnable )
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	for (auto& postProcessVolume : GameWorld->PostProcessVolumes)
	{
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
		if (PPVolume)
		{
			if (InEnable)
				PPVolume->Settings.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_Gaussian;
			else
				PPVolume->Settings.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_MAX;
		}
	}
}

void URenderFunctionLibrary::SetVignetteIntensity(float InIntensity)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	for (auto& postProcessVolume : GameWorld->PostProcessVolumes)
	{
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
		if (PPVolume)
		{
			PPVolume->Settings.VignetteIntensity = InIntensity;
		}
	}
}

void URenderFunctionLibrary::SetPostProcessSetting(FName MapID)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	FMapPostProcessSettingInfo* SettingInfo = UTableManager::GetInstancePtr()->GetMapPostProcessSettingRow(MapID);
	if (nullptr == SettingInfo)
		return;

	for (auto& postProcessVolume : GameWorld->PostProcessVolumes)
	{
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
		if (PPVolume)
		{
			if(SettingInfo->EnableDirectionalSetting)
			{
				PPVolume->Settings.bOverride_DirectionalLightDirectionMovable = 1;
				PPVolume->Settings.bOverride_DirectionalLightIntensityMovable = 1;
				PPVolume->Settings.bOverride_DirectionalLightColorMovable = 1;
				PPVolume->Settings.DirectionalLightDirectionMovable = SettingInfo->DirectionalLightDirectionMovable;
				PPVolume->Settings.DirectionalLightIntensityMovable = SettingInfo->DirectionalLightIntensityMovable;
				PPVolume->Settings.DirectionalLightColorMovable = SettingInfo->DirectionalLightColorMovable;
			}

			if(SettingInfo->EnableAmbientSetting)
			{
				PPVolume->Settings.bOverride_AmbientLightIntensityMovable = 1;
				PPVolume->Settings.bOverride_AmbientLightColorMovable = 1;
				PPVolume->Settings.AmbientLightIntensityMovable = SettingInfo->AmbientLightIntensityMovable;
				PPVolume->Settings.AmbientLightColorMovable = SettingInfo->AmbientLightColorMovable;
			}

			if(SettingInfo->EnableRimLightSetting)
			{
				PPVolume->Settings.bOverride_RimLightViewSpaceDirection = 1;
				PPVolume->Settings.bOverride_RimLightIntensity = 1;
				PPVolume->Settings.bOverride_RimLightColor = 1;
				PPVolume->Settings.bOverride_RimLightWidth = 1;
				PPVolume->Settings.bOverride_RimLightFallOff = 1;
				PPVolume->Settings.RimLightViewSpaceDirection = SettingInfo->RimLightViewSpaceDirection;
				PPVolume->Settings.RimLightIntensity = SettingInfo->RimLightIntensity;
				PPVolume->Settings.RimLightColor = SettingInfo->RimLightColor;
				PPVolume->Settings.RimLightWidth = SettingInfo->RimLightWidth;
				PPVolume->Settings.RimLightFallOff = SettingInfo->RimLightFallOff;
			}

			if(SettingInfo->EnableDynamicShadowSetting)
			{
				PPVolume->Settings.bOverride_DynamicShadowIntensity = 1;
				PPVolume->Settings.DynamicShadowIntensity = SettingInfo->DynamicShadowIntensity;
			}

			if(SettingInfo->EnableFogSetting)
			{
				PPVolume->Settings.bOverride_FogColor = 1;
				PPVolume->Settings.bOverride_FogStartDistance = 1;
				PPVolume->Settings.bOverride_FogDensity = 1;
				PPVolume->Settings.bOverride_FogMaxOpacity = 1;
				PPVolume->Settings.FogColor = SettingInfo->FogColor;
				PPVolume->Settings.FogStartDistance = SettingInfo->FogStartDistance;
				PPVolume->Settings.FogDensity = SettingInfo->FogDensity;
				PPVolume->Settings.FogMaxOpacity = SettingInfo->FogMaxOpacity;
			}
		}
	}
}



void URenderFunctionLibrary::SetOutLineEffect(float TimeAcc)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("%s[UWorld is Empty]"), __FUNCTION__);

	AProjectRGameMode* ProjectRGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameWorld));
	if (!ProjectRGameMode)
		return;

	float fCoeff = (2 * PI) / (RGameInstance->GameEnvironmentVar.OutLineDuration * 2 / RGameInstance->GameEnvironmentVar.FlickerCount);

	for (auto& postProcessVolume : GameWorld->PostProcessVolumes)
	{
		
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
		if (PPVolume)
		{
			if (PPVolume->Settings.WeightedBlendables.Array.IsValidIndex(0))
			{
				UMaterialInterface* OutLineMat = Cast<UMaterialInterface>(PPVolume->Settings.WeightedBlendables.Array[0].Object);
				if (OutLineMat)
				{
					UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(PPVolume->Settings.WeightedBlendables.Array[0].Object);

					if(!MID)
						MID = UMaterialInstanceDynamic::Create(OutLineMat, nullptr);

					if (MID)
					{
						float Slope = (1 - RGameInstance->GameEnvironmentVar.OutLineMinOffSet) / RGameInstance->GameEnvironmentVar.OutLineDuration;
						float fEffectRatio = FMath::Min<float>(RGameInstance->GameEnvironmentVar.OutLineMinOffSet + Slope * TimeAcc, 1.f);
						float fOutlineWidth = FMath::Abs<float>(cosf(fCoeff * TimeAcc));

						FString str = TEXT("EffectRatio : ") + FString::SanitizeFloat(fEffectRatio) + TEXT("  OutlineWidth : ") + FString::SanitizeFloat(fOutlineWidth);

						MID->SetScalarParameterValue(FName(TEXT("EffectRatio")), fEffectRatio);
						MID->SetScalarParameterValue(FName(TEXT("OutlineWidth")), fOutlineWidth);

						if (ProjectRGameMode->SceneCapturer)
						{
							UTexture* RT = ProjectRGameMode->SceneCapturer->RenderTarget;

							URCampaignHUD_ThreeControl* InGameHUD = Cast<URCampaignHUD_ThreeControl>(UUIFunctionLibrary::GetInGameHUD());
							if (InGameHUD && InGameHUD->Image_RT && ProjectRGameMode->MID && InGameHUD->isDebugRenderTarget)
							{
								ProjectRGameMode->MID->SetTextureParameterValue(FName(TEXT("RT")), RT);
								InGameHUD->Image_RT->SetBrushFromMaterial(ProjectRGameMode->MID);
							}
							MID->SetTextureParameterValue(FName(TEXT("RenderTarget")), RT);
						}
						PPVolume->Settings.WeightedBlendables.Array[0].Object = MID;
					}
				}

			}
		}
	}
}



void URenderFunctionLibrary::SetBloomEffectWithViewTarget(bool isOn, float InPower)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGameInstance->GetWorld(), 0);
	if (!PCM)
		return;

	AActor* ViewTargetActor = PCM->ViewTarget.Target;
	UCameraComponent* CamComp = ViewTargetActor->FindComponentByClass<UCameraComponent>();
	if (CamComp)
	{
		FLensBloomSettings FLens;
		FLens.ExportToPostProcessSettings(&CamComp->PostProcessSettings);
		CamComp->PostProcessSettings.bOverride_BloomIntensity = isOn;
		CamComp->PostProcessSettings.bOverride_BloomMethod= isOn;
		CamComp->PostProcessSettings.BloomIntensity = InPower;
	}
}
