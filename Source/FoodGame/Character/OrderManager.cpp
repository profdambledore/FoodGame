// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OrderManager.h"

// Sets default values
AOrderManager::AOrderManager()
{
	// Get Data Table object and store it
	ConstructorHelpers::FObjectFinder<UDataTable>OrderablesDTObject(TEXT("/Game/Data/DT_Orderables"));
	if (OrderablesDTObject.Succeeded()) { OrderablesDataTable = OrderablesDTObject.Object; }

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOrderManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrderManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrderManager::CompareOrder(TArray<FStackAsID> InStacks)
{
	// Get a copy of the input and requested arrays so we can remove indexes from it
	TArray<FStackAsID> InputStacks = InStacks;
	TArray<FStackAsID> RequestStacks;

	// Convert the current order from FOrderable to FStackAsID
	for (int i = 0; i < CurrentOrder.ItemsInOrder.Num(); i++) {
		TArray<FString> sID;
		for (int j = 0; j < CurrentOrder.ItemsInOrder[i].Items.Num(); j++) {
			sID.Add(CurrentOrder.ItemsInOrder[i].Items[j].ItemID);
		}
		RequestStacks.Add(FStackAsID(sID));
	}

	// Before comparing, make an array the length of the input stacks
	// Set all of their points to 0 to begin
	TArray<FStackAccuracy> StackPoints;
	for (int i = 0; i < InputStacks.Num(); i++) {
		StackPoints.Add(FStackAccuracy(i));
	}

	// Next, start comparing each item's length against the ones ordered
	for (int i = 0; i < InputStacks.Num(); i++) {
		StackPoints[i].LengthPoints = RequestStacks[0].ItemID.Num() / InputStacks[i].ItemID.Num();
		// Inverse points if they exceed 1.0f
		if (StackPoints[i].LengthPoints > 1.0f) {
			StackPoints[i].LengthPoints = StackPoints[i].LengthPoints - (StackPoints[i].LengthPoints - 1.0f);
		}
	}

	// Remove any indexes below the length threshold
	for (int i = 0; i < InputStacks.Num(); i++) {
		if (StackPoints[i].LengthPoints < LengthThreshold) {
			StackPoints.RemoveAt(i);
		}
	}

	// Check if there is no suitable index.  If so, this item requested is missing
	if (StackPoints.Num() == 0) {
		// Don't remove anything from the requested array
	}
	// Check if there are more than one suitable index.  If so, compute test two
	else {
		// Compare each input against the current requested by checking if input contains a requested item.  If it does, give it a point
		for (int i = 0; i < StackPoints.Num(); i++) {
			for (int j = 0; j < RequestStacks[0].ItemID.Num(); j++) {
				if (RequestStacks[0].ItemID.Contains(InputStacks[StackPoints[i].Index].ItemID[j])) {
					StackPoints[i].AccuracyPoints++;
				}
			}
		}

		// Remove any indexes outside the accuracy bounds
		for (int i = 0; i < StackPoints.Num(); i++) {
			if (StackPoints[i].AccuracyPoints > RequestStacks[0].ItemID.Num() + 1 || StackPoints[i].AccuracyPoints < RequestStacks[0].ItemID.Num() - 1) {
				StackPoints.RemoveAt(i);
			}
		}

		// If any remain now, then take the first index available.
		if (StackPoints.Num() != 0) {
			// Remove input and requested, reward player


			// If RequestStacks is now empty, add time
		}
		else {
			// Don't remove anything from the requested array, but remove the input

		}
	}
}

