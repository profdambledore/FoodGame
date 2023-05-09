// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/ParentCooker.h"
#include "Items/ParentItem.h"

// Sets default values
AParentItem::AParentItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionProfileName(TEXT("Item"));
}

// Called when the game starts or when spawned
void AParentItem::BeginPlay()
{
	Super::BeginPlay();
	SetupItem(Data);
}

// Called every frame
void AParentItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParentItem::SetupItem(FItemData NewItem)
{
	Data = NewItem;
	ItemMesh->SetStaticMesh(NewItem.Mesh);
}

float AParentItem::GetItemWeight()
{
	return Data.ItemWeight;
}

FString AParentItem::GetItemName()
{
	return Data.ID;
}

void AParentItem::ToggleItemCollision(bool bSetCollisionOn)
{
	if (bSetCollisionOn) {
		// Enable collision
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else {
		// Disable
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	};
}

void AParentItem::DetachStack(AParentItem* ItemToDetachFrom)
{
	// Go through the Set from the last index, detaching items
	bool bMatchingItem = false;
	TArray<AParentItem*> sIA = StackedItems.Array();

	for (int i = StackedItems.Num() - 1; i > 0; i--) {
		// Check if the items dont match and the item hasn't been found
		if (sIA[i] == ItemToDetachFrom ) {
			//  If the items are the same, then set bMatchingItem to true then remove it
			bMatchingItem = true;
			sIA[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			sIA[i]->ToggleItemCollision(true);
			sIA[i]->AttachedTo = nullptr;
			StackedItems.Remove(sIA[i]);
		}
		// Else, check if the item has been found already.  If it hasn't, remove this item
		else if (!bMatchingItem)
		{
			sIA[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			sIA[i]->ToggleItemCollision(true);
			sIA[i]->AttachedTo = nullptr;
			StackedItems.Remove(sIA[i]);
		}
	}
}

void AParentItem::StartCooking(AParentCooker* Cooker,  float CookingTime)
{
	// Set the AttachedActor to the cooker, but only if it's not attached to anything else
	if (AttachedTo == nullptr && Data.bBurnable == true) {
		AttachedCooker = Cooker;

		// Check if the timer exists
		if (GetWorld()->GetTimerManager().TimerExists(CookingTimerHandle) == false) {
			UE_LOG(LogTemp, Warning, TEXT("Started Cooking"));
			// If it doesn't, create it
			GetWorld()->GetTimerManager().SetTimer(CookingTimerHandle, FTimerDelegate::CreateUObject(this, &AParentItem::EndCooking), CookingTime, false, CookingTime);
		}
		else {
			// If it does, resume it
			GetWorld()->GetTimerManager().UnPauseTimer(CookingTimerHandle);
		}
	}
}

void AParentItem::StopCooking()
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped Cooking"));
	if (AttachedCooker != nullptr) {
		// Dereference the pointer
		AttachedCooker = nullptr;

		// Pause the timer
		GetWorld()->GetTimerManager().PauseTimer(CookingTimerHandle);
	}
}

void AParentItem::EndCooking()
{
	UE_LOG(LogTemp, Warning, TEXT("End Cooking"));
	// Change the item to the recipe
	AttachedCooker->OnCookingEnd(this);
}

