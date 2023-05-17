// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PlayerCharacter.h"
#include "Items/ParentContainer.h"

AParentContainer::AParentContainer()
{
	ContainerRange = CreateDefaultSubobject<UBoxComponent>(TEXT("Crafting Range"));
	ContainerRange->SetupAttachment(ItemMesh, "");
};

// Called when the game starts or when spawned
void AParentContainer::BeginPlay()
{
	Super::BeginPlay();

	//ContainerRange->OnComponentBeginOverlap.AddDynamic(this, &AParentContainer::OnCRBeginOverlap);

}

void AParentContainer::SetupContainer(FItemData Item, int StartAmount)
{
	ContainedItem = Item;
	Amount = StartAmount;
}

void AParentContainer::OnCRBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

bool AParentContainer::AddItemToContainer(AParentItem* ItemToAdd)
{
	// Check if the container has an item stored already.  If it doesn't check if the new item ID can be stored
	if (ContainedItem.ID == "" || Amount == 0) {
		if (AcceptedItems.Contains(ItemToAdd->Data.ID)) {
			// If the ID can be stored, set it to be this containers item
			SetupContainer(ItemToAdd->Data, 1);
			return true;
		}
	}
	// Check if the item is the same ID as the item stored
	else if (ItemToAdd->Data.ID == ContainedItem.ID) {
		Amount++;
		return true;
	}
	return false;
}

bool AParentContainer::RemoveItemFromContainer(class APlayerCharacter* CharacterToGiveItem)
{
	if (Amount > 0) {
		// Spawn the item in, setup its data and attach it to the player
		AParentItem* nI = GetWorld()->SpawnActor<AParentItem>(FVector(), FRotator{});
		nI->SetupItem(ContainedItem);
		CharacterToGiveItem->CollectItem(nI);
		return true;
	}
	return false;
}
