// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	ObjectMesh->SetupAttachment(GetRootComponent());

	CollisionMesh=CreateDefaultSubobject<UBoxComponent>(TEXT("ObjectCollision"));
	CollisionMesh->SetupAttachment(ObjectMesh);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::OnOverlapBegin);
	CollisionMesh->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ObjectMesh->SetRenderCustomDepth(true);

}

void AInteractableObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ObjectMesh->SetRenderCustomDepth(false);
}

void AInteractableObject::Interact()
{
	UE_LOG(LogTemp,Warning,TEXT("Interact"));
	bInteracted=true;
	UpdateGameMode();
}

