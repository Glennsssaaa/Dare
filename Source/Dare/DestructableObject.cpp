// Fill out your copyright notice in the Description page of Project Settings.
#include "DestructableObject.h"
#include "ATankCharacter.h"
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
}

// Called when the game starts or when spawned
void ADestructableObject::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ADestructableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


