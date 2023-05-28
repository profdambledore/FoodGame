// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/ExitQuestion.h"

void UExitQuestion::NativeConstruct()
{
	// Add OnReleased binding to the cancel button and the blur
	if (CancelButton) {
		CancelButton->OnReleased.AddDynamic(this, &UExitQuestion::CancelButtonReleased);
		Blur->OnReleased.AddDynamic(this, &UExitQuestion::CancelButtonReleased);
	}

	// Add OnReleased binding to the exit button
	if (ExitButton) {
		ExitButton->OnReleased.AddDynamic(this, &UExitQuestion::ExitButtonReleased);
	}
}

void UExitQuestion::CancelButtonReleased()
{
	// Hide the question
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UExitQuestion::ExitButtonReleased()
{
	// Exit the game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
