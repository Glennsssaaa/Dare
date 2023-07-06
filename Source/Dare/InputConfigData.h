// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "InputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API UInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputControllerMove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputKeyboardMove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputDash;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputAim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputRelease;
};
