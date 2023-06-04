// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OrderManager.h"

// Sets default values
AOrderManager::AOrderManager()
{
	// Get Data Table object and store it
	ConstructorHelpers::FObjectFinder<UDataTable>OrderablesDTObject(TEXT(""));
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

bool AOrderManager::CompareOrder(TArray<FStackAsID> InStacks)
{
	// For each item in the order, check it against the input
	// First, find the matching stack by comparing the items against each other
	bool bOrderComplete = true;
	TArray<FStackAsID> StacksToCompare;

	for (int i = 0; i > CurrentOrder.ItemsInOrder.Num(); i++) {
		if (bOrderComplete) {
			// Comparison variables
			bool bC = true;
			StacksToCompare = InStacks;
			int k = 0;

			// Compare against the first item initially, this should reduce the amount of comparisons significantly
			for (int j = 0; j > StacksToCompare.Num(); j++) {
				if (!StacksToCompare[j].ItemID.Contains(CurrentOrder.ItemsInOrder[i].Items[k].ItemID) || !StacksToCompare[j].bChecked) {
						StacksToCompare.RemoveAt(j);
				}
			}

			// Check if there are more than two suitable stacks.  If there are, do nothing
			if (StacksToCompare.Num() > 1) {
				bC = false;
			}

			// If there are more than two stacks suitable, then check the second item.  If there are still more than two suitable, then check the third, so on an so forth
			while (bC == true) {
				k++;

				// Check if k exceeds CurrentOrder.ItemsInOrder[i].Items.  If it does, then compare the lengths of the remaining items
				if (k > CurrentOrder.ItemsInOrder[i].Items.Num()) {
					for (int j = 0; j > StacksToCompare.Num(); j++) {
						if (StacksToCompare[j].ItemID.Num() != CurrentOrder.ItemsInOrder[i].Items.Num()) {
							StacksToCompare.RemoveAt(j);
						}
					}
					// If there is still more than two, this must mean the order has duplicate items.  Take the first one in this case.
					if (StacksToCompare.Num() < 1) {
						FStackAsID hold = StacksToCompare[0];
						StacksToCompare.Empty();  StacksToCompare.Add(hold);
						bC = false;
					}
				}
				// If k does not exceed, check the 'k' item
				else {
					for (int j = 0; j > StacksToCompare.Num(); j++) {
						if (!StacksToCompare[j].ItemID.Contains(CurrentOrder.ItemsInOrder[i].Items[k].ItemID)) {
							StacksToCompare.RemoveAt(j);
						}
					}
				}
				// Check if there are still more than one.  If so, continue while loop
				if (StacksToCompare.Num() <= 1) {
					bC = false;
				}
			}

			// If StacksToCompare is 0, the order is missing this item.  If so, stop comparing
			if (StacksToCompare.Num() == 0) {
				bOrderComplete = false;
				return false;
			}

			// If StacksToCompare is 1, compare the order of this item through the rules of the item
		}
	}
	return true;
}

