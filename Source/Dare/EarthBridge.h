// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "EarthBridge.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AEarthBridge : public AInteractableObject
{
	GENERATED_BODY()

public:
	virtual void Interact() override;
	AEarthBridge();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BridgeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BridgeMesh2;

	bool bInteracting;
};
