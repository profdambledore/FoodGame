// Fill out your copyright notice in the Description page of Project Settings.

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
	//if (Data.Mesh != nullptr) { ItemMesh->SetStaticMesh(Data.Mesh); };

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

