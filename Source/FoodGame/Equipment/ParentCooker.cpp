// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/ParentItem.h"
#include "Equipment/ParentCooker.h"

// Sets default values
AParentCooker::AParentCooker()
{
	OvenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Oven Mesh"));
	RootComponent = OvenMesh;

	CookingRange = CreateDefaultSubobject<UBoxComponent>(TEXT("Cooking Range"));
	CookingRange->SetupAttachment(OvenMesh, "");

	// Get Data Table object and store it
	ConstructorHelpers::FObjectFinder<UDataTable>RecipeDTObject(TEXT("/Game/Data/DT_CookingRecipes"));
	if (RecipeDTObject.Succeeded()) { Recipes = RecipeDTObject.Object; }

	// Get Data Table object and store it
	ConstructorHelpers::FObjectFinder<UDataTable>ItemsDTObject(TEXT("/Game/Data/DT_ItemData"));
	if (ItemsDTObject.Succeeded()) { Items = ItemsDTObject.Object; }

	// Get material object and store it
	ConstructorHelpers::FObjectFinder<UMaterial>MaterialObject(TEXT("/Game/Data/M_BurntItem"));
	if (ItemsDTObject.Succeeded()) { BurntMaterial = MaterialObject.Object; }

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParentCooker::BeginPlay()
{
	Super::BeginPlay();

	// Add Overlap Events
	CookingRange->OnComponentBeginOverlap.AddDynamic(this, &AParentCooker::OnCRBeginOverlap);
	CookingRange->OnComponentEndOverlap.AddDynamic(this, &AParentCooker::OnCREndOverlap);
}

// Called every frame
void AParentCooker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParentCooker::OnCRBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		AParentItem* newItem = Cast<AParentItem>(OtherActor);
		ItemsInCookingRange.Add(newItem);
		newItem->StartCooking(this, FindRecipe(newItem->Data.ID).CookTime);
	}
}

void AParentCooker::OnCREndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		for (int i = 0; i < ItemsInCookingRange.Num(); i++) {
			if (ItemsInCookingRange[i]->GetName() == OtherActor->GetName()) {
				UE_LOG(LogTemp, Warning, TEXT("59"));
				ItemsInCookingRange[i]->StopCooking();
				ItemsInCookingRange.RemoveAt(i);
			}
		}
	}
}

void AParentCooker::OnCookingEnd(AParentItem* Item)
{
	// Find the recipe related with this item.  If one is not found, then burn the item
	FRecipe_Cook foundRecipe = FindRecipe(Item->Data.Name);
	if (foundRecipe.ID == "") {
		Item->ItemMesh->SetMaterial(0, BurntMaterial);
		Item->bCannotCook = true;
	}
	// Else, set the item to the one in the recipe
	else {
		Item->SetupItem(*Items->FindRow<FItemData>(FName(*foundRecipe.OutputItems), "", false));

		// Check if the new item can be cooked.  If so, restart cooking
		if (Item->Data.bBurnable == true) {
			Item->GetWorld()->GetTimerManager().ClearTimer(Item->CookingTimerHandle);
			Item->StartCooking(this, FindRecipe(Item->Data.ID).CookTime);
		}
	}
}

FRecipe_Cook AParentCooker::FindRecipe(FString ItemName)
{
	// Iterate for matching recipe
	TArray<FName> RecipeRows = Recipes->GetRowNames();

	FRecipe_Cook* cr;

	// Iterate through all the recipes
	for (int j = 0; j < RecipeRows.Num(); j++) {
		// If the recipe hasen't been found, continue looping
		cr = Recipes->FindRow<FRecipe_Cook>(RecipeRows[j], "", false);
		// Check if this recipe has the same cooking type
		if (cr->CookerType == CookerType) {
			// Check if the recipe has matching input items
			if (cr->InputItems == ItemName) {
				return *cr;
			}
		}
	}

	FRecipe_Cook outRec;  outRec.CookTime = DefaultCookTime;
	return outRec;
}

