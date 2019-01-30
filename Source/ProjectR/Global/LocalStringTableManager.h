// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "LocalStringTableManager.generated.h"

struct FStringTableInfo;

//class URGameInstance;

/**
 * Owns string tables by language
 * UI code can use LocalizingTextBlock to display text, or call GetLocalizedText directly to look up text.
 */
UCLASS()
class PROJECTR_API ULocalStringTableManager : public UObject
{
	GENERATED_BODY()
	
public:
	static ULocalStringTableManager*	GetInstancePtr();
	static void ReleaseSingleton();

	void Initialize();
	
	void LoadTable(const ELanguageType InLanguageType);
	
	bool				GetLocalizedText(EStringTableType TableType, FName StringID, FText& StringValue);
	FStringTableInfo*	GetStringTable(EStringTableType TableType, FName StringID);
	FORCEINLINE ELanguageType		GetLanguageType() { return LanguageType; }

	TArray< FName >		GetStringKeyNames(EStringTableType TableType);

private:
	ULocalStringTableManager();
	
	static ULocalStringTableManager*	InstancePtr;

	void UnloadTableAll();

private:
// 	UPROPERTY()
// 	URGameInstance*		RGameInstance = nullptr;

	UPROPERTY()
	TMap<EStringTableType, FString> StringTableDefaultPath;

	UPROPERTY()
	TMap<ELanguageType, FString> LanguageMap;

	UPROPERTY()
	TMap<EStringTableType, UDataTable*> StringDataTableMap;
	
	UPROPERTY()
	const UEnum* 	EStringTableTypePtr = nullptr;

	UPROPERTY()
	const UEnum* 	ELanguageTypePtr = nullptr;

	UPROPERTY()
	FString DefaultStringTablePath;

	UPROPERTY()
	ELanguageType	LanguageType;
	
};

/*
Region Code
*/
// 
	// final class RegionCode {
	// 	// Region code for global networks (e.g. +800 numbers).
	// 	static final String UN001 = "001";
	// 	static final String AD = "AD";
	// 	static final String AE = "AE";
	// 	static final String AM = "AM";
	// 	static final String AO = "AO";
	// 	static final String AQ = "AQ";
	// 	static final String AR = "AR";
	// 	static final String AU = "AU";
	// 	static final String BB = "BB";
	// 	static final String BR = "BR";
	// 	static final String BS = "BS";
	// 	static final String BY = "BY";
	// 	static final String CA = "CA";
	// 	static final String CH = "CH";
	// 	static final String CL = "CL";
	// 	static final String CN = "CN";
	// 	static final String CS = "CS";
	// 	static final String CX = "CX";
	// 	static final String DE = "DE";
	// 	static final String FR = "FR";
	// 	static final String GB = "GB";
	// 	static final String HU = "HU";
	// 	static final String IT = "IT";
	// 	static final String JP = "JP";
	// 	static final String KR = "KR";
	// 	static final String MX = "MX";
	// 	static final String NZ = "NZ";
	// 	static final String PG = "PG";
	// 	static final String PL = "PL";
	// 	static final String RE = "RE";
	// 	static final String SE = "SE";
	// 	static final String SG = "SG";
	// 	static final String US = "US";
	// 	static final String YT = "YT";
	// 	static final String ZW = "ZW";
	// 	// Official code for the unknown region.
	// 	static final String ZZ = "ZZ";