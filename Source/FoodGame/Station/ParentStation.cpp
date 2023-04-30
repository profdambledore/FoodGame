// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/ParentStation.h"

// Sets default values
AParentStation::AParentStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Default Mesh"));
	CraftingRange = CreateDefaultSubobject<UBoxComponent>(TEXT("Crafting Range"));

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

bool AParentStation::GetSlotContextItem(FString ID)
{
	for (int i = 0; i < ContextItemSlot.AcceptedItems.Num(); i++) {
		if (ContextItemSlot.AcceptedItems[i] == ID) {
			return true;
		}
	}
	return false;
	}

FTransform AParentStation::GetSlotTransform()
{
	return ContextItemSlot.Transform;
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

