// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/BoxComponent.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Object Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetGenerateOverlapEvents(true);
	//Object Collision
	InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(Mesh);
	InteractCollision->SetBoxExtent(FVector(100.f,100.f,100.f));
	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	//Set the respawn position to the objects current location at the start of the game
	RespawnPos = GetActorLocation();
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Checks if the object has overlapped with a killzone
	if(OtherActor->ActorHasTag("Kill"))
	{
		//If it has, reset the objects location to the respawn position
		SetActorLocation(RespawnPos);
	}
}

