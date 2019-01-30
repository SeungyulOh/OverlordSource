// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ConstantVariablesTableInfo.h"

bool FConstantVariablesTableInfo::GetValue(int32& outValue)
{
	if (EConstantType::VE_INT == Type)
	{
		outValue = FCString::Atoi(*Value);
		return true;
	}

	return false;
}

bool FConstantVariablesTableInfo::GetValue(float& outValue)
{
	if (EConstantType::VE_FLOAT == Type)
	{
		outValue = FCString::Atof(*Value);
		return true;
	}

	return false;
}

bool FConstantVariablesTableInfo::GetValue(FString& outValue)
{
	if (EConstantType::VE_STRING == Type)
	{
		outValue = Value;
		return true;
	}

	return false;
}