// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CharacterTableInfo.h"
#include "UtilFunctionIntegrated.h"

float FCharacterTableInfo::GetFloatPropertyValue(FString PropertyName)
{
	for (TFieldIterator<UProperty> PropIt(FCharacterTableInfo::StaticStruct()); PropIt; ++PropIt)
	{
		UProperty* Property = *PropIt;

		FString CppName = Property->GetNameCPP();
		if (CppName == PropertyName)
		{
			FString typeString = Property->GetCPPType();
			if (typeString == "float")
			{
				UFloatProperty* FloatProperty = Cast< UFloatProperty >(Property);
				float value = FloatProperty->GetPropertyValue_InContainer(this);
				return value;
			}

			if (typeString == "int32")
			{
				UIntProperty* FloatProperty = Cast< UIntProperty >(Property);
				int32 value = FloatProperty->GetPropertyValue_InContainer(this);
				return (float)value;
			}
		}
	}

	return 0.0f;
}

UClass* FCharacterTableInfo::GetHeroBlueprintClass( )
{	
	return UUtilFunctionLibrary::GetCharacterClassFromAssetID( HeroClass );
}

UClass* FCharacterTableInfo::GetLobbyHeroBlueprintClass()
{
	return UUtilFunctionLibrary::GetCharacterClassFromAssetID(LobbyHeroClass);
}

UClass* FCharacterTableInfo::GetDefaultWeaponBlueprintClass(bool IsRWeapon)
{
	UClass* WeaponUClass = nullptr;	
	if (IsRWeapon)
	{
		WeaponUClass = UUtilFunctionLibrary::GetWeaponBaseFromAssetID( RWeaponType);
	}
	else
	{
		WeaponUClass = UUtilFunctionLibrary::GetWeaponBaseFromAssetID( LWeaponType);
	}
	
	return WeaponUClass;
}

#define RESET_LOADED_ASSET_PTR(X)\
if(X.IsValid())\
	X = nullptr;
void FCharacterTableInfo::ResetLoadedAssetPtr()
{
	RESET_LOADED_ASSET_PTR(HeroClass);
	RESET_LOADED_ASSET_PTR(LobbyHeroClass);
	RESET_LOADED_ASSET_PTR(RWeaponType);
	RESET_LOADED_ASSET_PTR(LWeaponType);
			
	RESET_LOADED_ASSET_PTR(DecoMesh);
	RESET_LOADED_ASSET_PTR(DecoParticle);
	RESET_LOADED_ASSET_PTR(MeshMaterial);
	RESET_LOADED_ASSET_PTR(MeshMaterial2);
}

void FCharacterGrind::OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems)
{
	this->HeroGrade = FCString::Atoi(*(InRowName.ToString()));
	UE_LOG(LogUI, Log, TEXT("OnPostDataImport %d"), FCString::Atoi(*(InRowName.ToString())));
}

void FCharacterCostUpgrade::DataSetting()
{
	CharacterCondition.Reset();
	CharacterValue.Reset();
	CharacterGrade.Reset();
	CharacterNum.Reset();
	CharacterCondition.Emplace(CharacterCondition1);
	CharacterCondition.Emplace(CharacterCondition2);
	CharacterCondition.Emplace(CharacterCondition3);
	CharacterCondition.Emplace(CharacterCondition4);
	CharacterValue.Emplace(CharacterValue1);
	CharacterValue.Emplace(CharacterValue2);
	CharacterValue.Emplace(CharacterValue3);
	CharacterValue.Emplace(CharacterValue4);
	CharacterGrade.Emplace(CharacterGrade1);
	CharacterGrade.Emplace(CharacterGrade2);
	CharacterGrade.Emplace(CharacterGrade3);
	CharacterGrade.Emplace(CharacterGrade4);
	CharacterNum.Emplace(CharacterNum1);
	CharacterNum.Emplace(CharacterNum2);
	CharacterNum.Emplace(CharacterNum3);
	CharacterNum.Emplace(CharacterNum4);
}
