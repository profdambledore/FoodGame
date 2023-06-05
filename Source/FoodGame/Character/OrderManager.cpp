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
	UE_LOG(LogTemp, Warning, TEXT("Start"));

	// Get a copy of the input and requested arrays so we can remove indexes from it
	//TArray<FStackAsID>
	 InputStacks = InStacks;
	//TArray<FStackAsID> RequestStacks;

	// Convert the current order from FOrderable to FStackAsID
	for (int i = 0; i < CurrentOrders[OrderIndex].ItemsInOrder.Num(); i++) {
		TArray<FString> sID;
		for (int j = 0; j < CurrentOrders[OrderIndex].ItemsInOrder[i].Items.Num(); j++) {
			sID.Add(CurrentOrders[OrderIndex].ItemsInOrder[i].Items[j].ItemID);
		}
		RequestStacks.Add(FStackAsID(sID));
	}

	// Before comparing, make an array the length of the input stacks
	// Set all of their points to 0 to begin
	//TArray<FStackAccuracy> StackPoints;
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
			UE_LOG(LogTemp, Warning, TEXT("Below Len Thresh"));
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
			for (int j = 0; j < InputStacks[StackPoints[i].Index].ItemID.Num(); j++) {
				if (RequestStacks[0].ItemID.Contains(InputStacks[StackPoints[i].Index].ItemID[j])) {
					// RequestStacks[0].ItemID.Contains(InputStacks[StackPoints[i].Index].ItemID[j])
					StackPoints[i].AccuracyPoints++;
					UE_LOG(LogTemp, Warning, TEXT("Acc++"));
				}
			}
		}

		// Remove any indexes outside the accuracy bounds
		for (int i = 0; i < StackPoints.Num(); i++) {
			if (StackPoints[i].AccuracyPoints > RequestStacks[0].ItemID.Num() + 1 || StackPoints[i].AccuracyPoints < RequestStacks[0].ItemID.Num() - 1) {
				StackPoints.RemoveAt(i);
				UE_LOG(LogTemp, Warning, TEXT("Below Acc Thresh"));
			}
		}

		// If any remain now, then take the first index available.
		if (StackPoints.Num() != 0) {
			UE_LOG(LogTemp, Warning, TEXT("Accurate"));
			// Remove input and requested, add to the reward map
			// First, check if the map contains the index.  If there is, modify the struct in the map
			//FOrderReward foundReward;

			if (!Rewards.Find(OrderIndex)) {
				Rewards.Add(OrderIndex, FOrderReward{});
				UE_LOG(LogTemp, Warning, TEXT("Couldn't find one, adding to map"));
			}
			foundReward = Rewards.FindRef(OrderIndex);
			
			// Find what items are additional and what are missing
			TArray<FString> input = InputStacks[StackPoints[0].Index].ItemID; TArray<FString> request = RequestStacks[0].ItemID;
			int j = 0; int k = request.Num();
			for (int i = 0; i < k; i++) {
				if (input.Contains(request[j])) {
					UE_LOG(LogTemp, Warning, TEXT("Contains"));
					input.RemoveSingle(request[j]);
					request.RemoveAt(j);
				}
				else {
					j++;
				}
			}

			// Add a new item to the FoundReward
			foundReward.ItemRewardData.Add(FRewardData{ StackPoints[0].PresentationPoints, StackPoints[0].AccuracyPoints, 0, request });
			foundReward.AdditionalItems.Append(input);

			// Modify the map
			Rewards.Add(OrderIndex, foundReward);

			// Then remove the requested and inputted items
			RequestStacks.RemoveAt(0);
			CurrentOrders[OrderIndex].ItemsInOrder[0].bComplete = true;
			InputStacks.RemoveAt(StackPoints[0].Index);

			// If RequestStacks is now empty, complete the order
			if (RequestStacks.Num() == 0)
			{
				CompleteOrder();
			}
		}
	}
}

void AOrderManager::CompleteOrder()
{
}

