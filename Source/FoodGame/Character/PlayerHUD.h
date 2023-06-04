// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Components/ProgressBar.h"
#include "Components/ListView.h"
#include "Math/Color.h"

#include "PlayerHUD.generated.h"

UCLASS()
class FOODGAME_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void ToggleHeldItem(bool bActive);

	UFUNCTION(BlueprintCallable)
		void UpdateRoundTime(float NewTime);

	UFUNCTION(BlueprintCallable)
		void UpdateTicketTime(float NewTime);

	UFUNCTION(BlueprintCallable)
		void UpdateMoney(float NewAmount);

public:
	// Components
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* AimPosition = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* HeldItemImage = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* RoundTime = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* TicketTime = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* MoneyAmount = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UProgressBar* RoundBar = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UListView* MissingItems = nullptr;

	// References
	class APlayerCharacter* Owner = nullptr;

	// Colours
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colours")
		FLinearColor RoundBarMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colours")
		FLinearColor RoundBarMax;

protected:

	
};
