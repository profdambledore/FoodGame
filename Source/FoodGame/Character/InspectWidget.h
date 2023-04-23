// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/ListView.h"

#include "Item/ItemData.h"

#include "InspectWidget.generated.h"

/**
 * 
 */
UCLASS()
class FOODGAME_API UInspectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	virtual void SynchronizeProperties() override;

	UFUNCTION(BlueprintCallable)
		void SetInspectingItem(FString Name, TArray<FItemUsage> Usage, bool bCanPickup);
	
public:
	// Components
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* ItemName = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UListView* UsageList = nullptr;

	// New Usage List Item pointer property, makes it so we only declare this variable once
	class UUsageListData* newULI = nullptr;
};
