// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/ParentStation.h"

// Sets default values
AParentStation::AParentStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Default Mesh"));

	// Get Data Table object and store it
	ConstructorHelpers::FObjectFinder<UDataTable>RecipieDTObject(TEXT("/Game/Data/DT_Recipes"));
	if (RecipieDTObject.Succeeded()) { Recipes = RecipieDTObject.Object; }
}

// Called when the game starts or when spawned
void AParentStation::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AParentStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FItemSlot AParentStation::GetStationSlot(FString Name)
{
	int slotIndex = -1;

	// Check each item slot in the array for the input
	for (int i = 0; i < ItemSlots.Num(); i++) {
		for (int j = 0; j < ItemSlots[i].AcceptedItems.Num(); j++) {
			if (ItemSlots[i].AcceptedItems[j] == Name) {
				return ItemSlots[i];
			}
			else if (ItemSlots[i].AcceptedItems[j] == "items_all") {
				slotIndex = i;
			}
		}
	}

	// If an example of "items_all" was found, then return that slot
	if (slotIndex != -1) {
		return ItemSlots[slotIndex];
	}
	// Else, return an empty slot
	else {
		UE_LOG(LogTemp, Warning, TEXT("Empty"));
		return FItemSlot{};
	}
	
}

FTransform AParentStation::GetSlotTransform(FString SlotName)
{
	for (int i = 0; i < ItemSlots.Num(); i++) {
		if (ItemSlots[i].Slot == SlotName) {
			return ItemSlots[i].Transform;
		}
	}
	return FTransform{};
}

FRecipe AParentStation::FindRecipe()
{
	// Matching Recipes
	TArray<FRecipe> FoundRecipes;

	// Iterate across the station recipes for one matching 
	for (int i = 0; i < StationRecipes.Num() - 1; i++) {
		//if (StationRecipes[i].ContextItem == )
	}


	return FRecipe();
}

