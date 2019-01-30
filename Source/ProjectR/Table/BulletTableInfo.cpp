// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BulletTableInfo.h"
#include "UtilFunctionIntegrated.h"

float FBulletTableInfo::GetFloatPropertyValue(FString PropertyName)
{
	for (TFieldIterator<UProperty> PropIt(FBulletTableInfo::StaticStruct()); PropIt; ++PropIt)
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

UClass* FBulletTableInfo::GetBulletBlueprintClass(bool forObject/* = false*/)
{
	if (false == forObject)
	{
		return UUtilFunctionLibrary::GetBulletBaseFromAssetID(BulletClass);
	}
	else
	{
		UClass* BulletUClass = BulletClass.Get();
		if (BulletUClass == nullptr)
		{
			BulletClass.ToSoftObjectPath().TryLoad();
			BulletUClass = BulletClass.Get();
		}
		return BulletUClass;
	}
	return nullptr;
}

void FBulletTableInfo::ResetLoadedAssetPtr()
{
	if (BulletClass.IsValid())
	{
		BulletClass = nullptr;
	}
}