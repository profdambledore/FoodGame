// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Item/ItemData.h"

#include "UsageListData.generated.h"

UCLASS()
class FOODGAME_API UUsageListData : public UObject
{
	GENERATED_BODY()
	
public:
	FItemUsage UsageData;
};
