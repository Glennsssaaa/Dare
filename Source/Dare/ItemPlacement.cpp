// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPlacement.h"

// Sets default values
AItemPlacement::AItemPlacement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AItemPlacement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemPlacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

