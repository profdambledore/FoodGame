// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Plate.h"
#include "Character/PlayerCharacter.h"
#include "Equipment/ParentSink.h"

// Sets default values
AParentSink::AParentSink()
{
	SinkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sink Mesh"));
	RootComponent = SinkMesh;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AParentSink::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParentSink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AParentSink::GetCanAddPlate()
{
	// Check if the player can add the plate to the sink
	if (PlatesInSink.Num() < MaxPlatesInSink) {
		return true;
	}
	else {
		return false;
	}
}

void AParentSink::RemovePlateFromSink(APlayerCharacter* PlayerToAttachTo)
{
}

void AParentSink::CleanPlate()
{
}

