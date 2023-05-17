// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/ParentItem.h"
#include "Equipment/ParentBin.h"

// Sets default values
AParentBin::AParentBin()
{
	BinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Oven Mesh"));
	RootComponent = BinMesh;

	BinRange = CreateDefaultSubobject<UBoxComponent>(TEXT("Delete Range"));
	BinRange->SetupAttachment(BinMesh, "");

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParentBin::BeginPlay()
{
	Super::BeginPlay();

	// Add Overlap Events
	BinRange->OnComponentBeginOverlap.AddDynamic(this, &AParentBin::OnBRBeginOverlap);
}

// Called every frame
void AParentBin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParentBin::OnBRBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if AParentItem, destroy item
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		AParentItem* ItemToBin = Cast<AParentItem>(OtherActor);
		if (ItemToBin->Data.bBinnable)
		{
			OtherActor->Destroy();
		}
	}
}

