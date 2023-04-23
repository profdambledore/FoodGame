// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UsageListData.h"
#include "Character/InspectWidget.h"

void UInspectWidget::NativeConstruct()
{

}

void UInspectWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

}

void UInspectWidget::SetInspectingItem(FString Name, TArray<FItemUsage> Usage, bool bCanPickup)
{
	// Set the name text
	ItemName->SetText(FText::FromString(Name));

	// Create the list of usage points
	UsageList->ClearListItems();
	
	for (int i = 0; i < Usage.Num() - 1; i++) {
		// Check if the usage point is PickUp.  If it is, check to see if we can pick up the interactable
		if (Usage[i].Rule == EUsageRule::Pickup && bCanPickup || Usage[i].Rule != EUsageRule::Pickup) {
			// If the rule suffices, creat the object
			newULI = NewObject<UUsageListData>();
			newULI->UsageData = Usage[i];
			UsageList->AddItem(newULI);
		}
	}
}
