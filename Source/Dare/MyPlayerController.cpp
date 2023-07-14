// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Print out name of actor class to screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MyPlayerController BeginPlay()"));
	
}

void AMyPlayerController::UpdateMappings(FName DisplayName, FKey NewKey)
{
	// Get the current key mappings
	// Print Hello to screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UpdateMappings()"));
	TArray<FInputActionKeyMapping> ActionMappings = PlayerInput->GetKeysForAction(DisplayName);

	// Remove all the mappings for this action
	for (FInputActionKeyMapping Mapping : ActionMappings)
	{
		PlayerInput->RemoveActionMapping(Mapping);
	}

	// Add the new mapping
	PlayerInput->AddActionMapping(FInputActionKeyMapping(DisplayName, NewKey));
}
