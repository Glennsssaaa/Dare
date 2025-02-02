// Fill out your copyright notice in the Description page of Project Settings.
#include "DestructableObject.h"
#include "TankCharacter.h"
// Sets default values
ADestructableObject::ADestructableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(!DestructibleObjectMesh)
	{
		DestructibleObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
		DestructibleObjectMesh->SetupAttachment(GetRootComponent());
	}

	if (!ObjectCollisionBox) 
	{
		ObjectCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
		ObjectCollisionBox->SetupAttachment(DestructibleObjectMesh);
	}

	Points=50;
}

// Called when the game starts or when spawned
void ADestructableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestructableObject::Destroyed()
{
	UpdateGameMode();
}



// Called every frame
void ADestructableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


