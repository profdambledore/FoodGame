// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PlayerCharacter.h"
#include "Character/PlayerHUD.h"

// Show the icon of the item that the player is holding
void UPlayerHUD::ToggleHeldItem(bool bActive) {
	if (HeldItemImage) {
		if (bActive) {
			HeldItemImage->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			HeldItemImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPlayerHUD::UpdateRoundTime(float NewTime)
{
	if (RoundTime) {
		// Format Text works like: NSLOCTEXT(LocTextName, TextName, TextToDisplay, ArgumentsInOrder)
		RoundTime->SetText(FText::Format(NSLOCTEXT("PlayerUINamespace", "RoundTime", "{0}"), NewTime));
	}
}

void UPlayerHUD::UpdateTicketTime(float NewTime)
{
	if (TicketTime) {
		// Format Text works like: NSLOCTEXT(LocTextName, TextName, TextToDisplay, ArgumentsInOrder)
		TicketTime->SetText(FText::Format(NSLOCTEXT("PlayerUINamespace", "TicketTime", "{0}"), NewTime));
	}
}

void UPlayerHUD::UpdateMoney(float NewAmount)
{
	if (MoneyAmount) {
		// Format Text works like: NSLOCTEXT(LocTextName, TextName, TextToDisplay, ArgumentsInOrder)
		MoneyAmount->SetText(FText::Format(NSLOCTEXT("PlayerUINamespace", "MoneyAmount", "{0}"), NewAmount));
	}
}
