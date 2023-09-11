// Fill out your copyright notice in the Description page of Project Settings.


#include "RebuildableBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ARebuildableBase::ARebuildableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Set up the mesh components
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

	//Fire VFX setup (Unused)
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

	//Check if the house is rebuilding and if it is not on fire
	if(bIsRebuilding && !bIsOnFire)
	{
		//Increment the rebuild progress
		UE_LOG(LogTemp,Warning,TEXT("AAAAA"));
		RebuildProgress += DeltaTime * 60.f;
		//If the rebuild progress is 100% or more, stop rebuilding and toggle the house destruction
		if(RebuildProgress >= 100.f)
		{
			RebuildProgress = 100.f;
			bIsRebuilding = false;
			ToggleHouseDestruction();
		}
	}

}

void ARebuildableBase::ToggleHouseDestruction()
{
	//Toggle the house destruction
	bIsDestroyed = !bIsDestroyed;

	//If the house is destroyed, hide the rebuilt mesh and show the destroyed mesh
	if(bIsDestroyed)
	{
		RebuiltMesh->SetVisibility(false);
		DestroyedMesh->SetVisibility(true);
		//Set the rebuild progress to 0
		RebuildProgress = 0.f;
		//Remove the points from the game mode score
		Points=-Points;
		UpdateGameMode();
	}
	else
	{
		//If the house is rebuilt, hide the destroyed mesh and show the rebuilt mesh
		RebuiltMesh->SetVisibility(true);
		DestroyedMesh->SetVisibility(false);
		//Find the tank player and unfreeze them
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATankCharacter::StaticClass(), FoundActors);
		ATankCharacter* TempChar = Cast<ATankCharacter>(FoundActors[0]);
		if(TempChar!=nullptr)
		{
			TempChar->bPlayerFrozen=false;
			TempChar->bToggleInteract=false;
		}
		if(Points<0)
		{
			Points=-Points;
		}
		UpdateGameMode();
	}
}

//Toggles the fire on the house
void ARebuildableBase::ToggleFire()
{
	bIsOnFire = !bIsOnFire;
}
//Toggles the clean state of the house
void ARebuildableBase::ToggleClean()
{
	bIsClean = !bIsClean;
}
