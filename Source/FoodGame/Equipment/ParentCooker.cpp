// Fill out your copyright notice in the Description page of Project Settings.


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


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParentCooker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParentCooker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

