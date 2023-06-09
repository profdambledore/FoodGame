// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"
#include "Items/ItemDataLibrary.h"

#include "OrderManager.generated.h"

UCLASS()
class FOODGAME_API AOrderManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrderManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void CompareOrder(TArray<FStackAsID> InStacks);

	UFUNCTION(BlueprintCallable)
		void CompleteOrder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Money
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TotalMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RoundMoney;

	// Data Tables
	// Pointer to the orderable data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
		UDataTable* OrderablesDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
		TArray<FOrderable> AvailableOrderables;

	// Order
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FOrder> CurrentOrders;

	int OrderIndex;

	// Rewards
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int, FOrderReward> Rewards;

	//TEEST
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStackAsID> InputStacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStackAsID> RequestStacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FStackAccuracy> StackPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOrderReward foundReward;

	float LengthThreshold = 0.7f;
	float AccuracyThreshold = 1.0f;

};
