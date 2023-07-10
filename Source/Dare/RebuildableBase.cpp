// Fill out your copyright notice in the Description page of Project Settings.


#include "RebuildableBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARebuildableBase::ARebuildableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(!DestroyedMesh)
	{
		DestroyedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestroyedMesh"));
		DestroyedMesh->SetupAttachment(RootComponent);
	}

	if(!RebuiltMesh)
	{
		RebuiltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RebuiltMesh"));
		RebuiltMesh->SetupAttachment(DestroyedMesh);
	}

	if(!RebuildCollision)
	{
		RebuildCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RebuildCollision"));
		RebuildCollision->SetupAttachment(DestroyedMesh);
	}

	if(!HouseCollision)
	{
		HouseCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HouseCollision"));
		HouseCollision->SetupAttachment(DestroyedMesh);
	}

	if(!FireVfx)
	{
	//	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(FireVfx, DestroyedMesh, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		
	}

}

// Called when the game starts or when spawned
void ARebuildableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARebuildableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARebuildableBase::ToggleHouseDestruction()
{
	bIsDestroyed = !bIsDestroyed;

	if(bIsDestroyed)
	{
		RebuiltMesh->SetVisibility(false);
		DestroyedMesh->SetVisibility(true);
	}
	else
	{
		RebuiltMesh->SetVisibility(true);
		DestroyedMesh->SetVisibility(false);

	}
}

void ARebuildableBase::ToggleFire()
{
	bIsOnFire = !bIsOnFire;

	
}

void ARebuildableBase::ToggleClean()
{
	bIsClean = !bIsClean;
}
