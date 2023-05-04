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

void AParentItem::StartCooking(AParentCooker* Cooker,  float CookingTime)
{
	// Set the AttachedActor to the cooker, but only if it's not attached to anything else
	if (AttachedTo != nullptr && Data.bBurnable == true) {
		AttachedCooker = Cooker;

		// Check if the timer exists
		if (GetWorld()->GetTimerManager().TimerExists(CookingTimerHandle)) {
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
	if (AttachedCooker != nullptr) {
		// Dereference the pointer
		AttachedCooker = nullptr;

		// Pause the timer
		GetWorld()->GetTimerManager().PauseTimer(CookingTimerHandle);
	}
}

void AParentItem::EndCooking()
{
	// Change the item to the recipe
	AttachedCooker->OnCookingEnd(this);
}

