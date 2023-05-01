// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ParentItem.h"
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

	// Add Overlap Events
	CraftingRange->OnComponentBeginOverlap.AddDynamic(this, &AParentStation::OnCRBeginOverlap);
	CraftingRange->OnComponentEndOverlap.AddDynamic(this, &AParentStation::OnCREndOverlap);
}

// Called every frame
void AParentStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParentStation::OnCRBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		ItemsInCraftingRange.Add(Cast<AParentItem>(OtherActor));
		FindRecipe();
	}
}

void AParentStation::OnCREndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		for (int i = 0; i < ItemsInCraftingRange.Num(); i++) {
			if (ItemsInCraftingRange[i]->GetName() == OtherActor->GetName()) {
				ItemsInCraftingRange.RemoveAt(i);
				FindRecipe();
			}
		};
	}
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

void AParentStation::AddContextItem(FString ID)
{
	ContextItemSlot.ItemInSlot = ID;
}

void AParentStation::RemoveContextItem()
{
	ContextItemSlot.ItemInSlot = "";
}

void AParentStation::FindRecipe()
{
	// Get all ID's in the crafting range
	TArray<FString> InputItemIDs;

	for (int i = 0; i < ItemsInCraftingRange.Num(); i++) {
		InputItemIDs.Add(ItemsInCraftingRange[i]->GetItemName());
	}

	// Iterate for matching recipe
	TArray<FName> RecipeRows = Recipes->GetRowNames();
	bool bRecipeFound = false;

	TArray<FString> iids;
	TArray<FString> frid;

	FRecipe* cr;

	// Iterate through all the recipes
	for (int j = 0; j < RecipeRows.Num(); j++) {
		// If the recipe hasen't been found, continue looping
		if (!bRecipeFound) {
			cr = Recipes->FindRow<FRecipe>(RecipeRows[j], "", false);
			// Check if the recipe matches the station type
			if (cr->Station == StationType) {
				// Check if this recipe has the same context item
				if (cr->ContextItem == ContextItemSlot.ItemInSlot) {
					// Check if the recipe has the same amount of items used in crafting
					if (cr->InputItems.Num() == InputItemIDs.Num()) {
						iids = InputItemIDs; frid = cr->InputItems;
						for (int k = 0; k < iids.Num(); k++) {
							if (frid.Contains(iids[k])) {
								frid.RemoveAt(k);  iids.RemoveAt(k);
							}
						}
						if (iids.Num() == 0 && frid.Num() == 0)
						{
							// This is the item
							bRecipeFound = true;
							CurrentRecipes = *cr;
						}
					}
				}
			}
		}
	}

	if (!bRecipeFound) {
		CurrentRecipes = {};
	}
}

void AParentStation::CraftRecipe()
{
}
