// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Item/ItemData.h"

#include "UsageListItem.generated.h"

UCLASS()
class FOODGAME_API UUsageListItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// As OnListItemObjectSet is BlueprintImplementableEvent, it cannot be implemented in C++ (as stated by https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/GameplayArchitecture/Interfaces/)
	// Call this function in Blueprint
	UFUNCTION(BlueprintCallable)
		void OnListItem(UObject* ListItemObject);

public:
	// Components
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* Description = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* KeyIcon = nullptr;


	// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UUsageListData* Data = nullptr;
};
