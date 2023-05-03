// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ParentStation.h"

// Sets default values
AParentStation::AParentStation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CraftingRange = CreateDefaultSubobject<UBoxComponent>(TEXT("Crafting Range"));
	CraftingRange->SetupAttachment(ItemMesh, "");

	// Get Data Table object and store it
	ConstructorHelpers::FObjectFinder<UDataTable>RecipeDTObject(TEXT("/Game/Data/DT_ChoppingRecipes"));
	if (RecipeDTObject.Succeeded()) { Recipes = RecipeDTObject.Object; }

	// Get Data Table object and store it
	ConstructorHelpers::FObjectFinder<UDataTable>ItemsDTObject(TEXT("/Game/Data/DT_ItemData"));
	if (ItemsDTObject.Succeeded()) { Items = ItemsDTObject.Object; }
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
	bool bAddItem = false;

	if (OtherActor->IsA(AParentItem::StaticClass())) {
		AParentItem* collideItem = Cast<AParentItem>(OtherActor);
		// Check if it is a context item.
		for (int i = 0; i < ContextItemSlot.AcceptedItems.Num(); i++) {
			if (collideItem->Data.ID == ContextItemSlot.AcceptedItems[i])
			{
				AddContextItem(collideItem->Data.ID);
				bAddItem = true;
				break;
			}
		}
		// Else
		if (bAddItem == false) {
			ItemsInCraftingRange.Add(collideItem);
			FindRecipe();
		}
	}
}

void AParentStation::OnCREndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bool bRemContextItem = true;
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		for (int i = 0; i < ItemsInCraftingRange.Num(); i++) {
			if (ItemsInCraftingRange[i]->GetName() == OtherActor->GetName()) {
				ItemsInCraftingRange.RemoveAt(i);
				FindRecipe();
				bRemContextItem = false;
				break;
			}
		}
		if (bRemContextItem) {
			RemoveContextItem();
		}
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

void AParentStation::AddContextItem(FString ID)
{
	ContextItemSlot.ItemInSlot = ID;
	FindRecipe();
}

void AParentStation::RemoveContextItem()
{
	ContextItemSlot.ItemInSlot = "";
	FindRecipe();
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

	FRecipe_Chop* cr;

	// Iterate through all the recipes
	for (int j = 0; j < RecipeRows.Num(); j++) {
		// If the recipe hasen't been found, continue looping
		if (!bRecipeFound) {
			cr = Recipes->FindRow<FRecipe_Chop>(RecipeRows[j], "", false);
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

	if (!bRecipeFound) {
		CurrentRecipes = {};
	}
}

void AParentStation::CraftRecipe()
{
	TArray<FString> newItems = CurrentRecipes.OutputItems;
	TArray<AParentItem*> iICR = ItemsInCraftingRange;
	int j = 0;

	// Change any items currently in the range to the new items
	for (int i = 0; i < iICR.Num(); i++) {
		if (Items->FindRow<FItemData>(FName(*newItems[j]), "", false)->Class == iICR[i]->Data.Class){
			iICR[i]->SetupItem(*Items->FindRow<FItemData>(FName(*newItems[j]), "", false));
			newItems.RemoveAt(j);
			iICR.RemoveAt(j);
		}
		else {
			i--;
			j++;
		}

	}
	
	// For any remaining items in newItems, or any items requiring different classes, create a new item class
	for (j = 0; j < newItems.Num(); j++) {
		UE_LOG(LogTemp, Warning, TEXT("NewItem"));
		FItemData* newData = Items->FindRow<FItemData>(FName(*newItems[0]), "", false);
		AParentItem* nI = GetWorld()->SpawnActor<AParentItem>(newData->Class, UKismetMathLibrary::RandomPointInBoundingBox(CraftingRange->GetComponentLocation(), CraftingRange->GetUnscaledBoxExtent()), FRotator{});
		nI->SetupItem(*newData);
	}

	// Destroy any items remaining
	for (int i = 0; i < iICR.Num(); i++) {
		iICR[i]->Destroy();
	}
}