// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "Object.h"
// #include "CafeSDKPluginManager.generated.h"
// 
// //class URGameInstance;
// /**
//  * 
//  */
// UCLASS()
// class PROJECTR_API UCafeSDKPluginManager : public UObject
// {
// 	GENERATED_BODY()
// 	
// public:
// 	void Initialize(/*URGameInstance* InGameInstance*/);
// 
// 
// 	UFUNCTION(BlueprintCallable, Category = CafeSDKPluginManager)
// 	bool IsUsedCafeSDK() { return UseCafeSDK;  }
// 	
// 	UFUNCTION(BlueprintCallable, Category = CafeSDKPluginManager)
// 	void InitDomestic();
// 
// 	UFUNCTION(BlueprintCallable, Category = CafeSDKPluginManager)
// 	void InitGlobal();
// 	
// 	UFUNCTION(BlueprintCallable, Category = CafeSDKPluginManager)
// 	void InitTheme();
// 
// 	UFUNCTION(BlueprintCallable, Category = CafeSDKPluginManager)
// 	void InitChannel();
// 
// 	
// 
// 	/*
// 	Callback event functions
// 	*/
// 	void InitCallbackFunctions();
// 	void OnScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Colors);
// 
// 
// 
// private:
// // 	UPROPERTY()
// // 	URGameInstance*				RGameInstance = nullptr;
// 		
// 	bool						UseCafeSDK = false;
// };