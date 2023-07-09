// Fill out your copyright notice in the Description page of Project Settings.


#include "RebuildableBase.h"

#include "Components/BoxComponent.h"

// Sets default values
ARebuildableBase::ARebuildableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(!DestroyedMesh)
	{
		DestroyedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestroyedMesh"));
		RootComponent = DestroyedMesh;
	}

	if(!RebuiltMesh)
	{
		RebuiltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RebuiltMesh"));
		RebuiltMesh->SetupAttachment(GetRootComponent());
	}

	if(!RebuildCollision)
	{
		RebuildCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RebuildCollision"));
		RebuildCollision->SetupAttachment(GetRootComponent());
	}

	if(!HouseCollision)
	{
		HouseCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HouseCollision"));
		HouseCollision->SetupAttachment(GetRootComponent());
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

