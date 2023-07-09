// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RebuildableBase.generated.h"

UCLASS()
class DARE_API ARebuildableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARebuildableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Destroyed static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* DestroyedMesh;

	// Rebuilt static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* RebuiltMesh;

	// Rebuilding Collision Area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UBoxComponent* RebuildCollision;

	// House Collision Box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UBoxComponent* HouseCollision;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
