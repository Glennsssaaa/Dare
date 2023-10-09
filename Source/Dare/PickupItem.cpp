// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"

#include "ItemPlacement.h"
#include "Components/BoxComponent.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetGenerateOverlapEvents(true);

	InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(Mesh);
	InteractCollision->SetBoxExtent(FVector(100.f,100.f,100.f));
	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnOverlapBegin);
	
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
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
	if(OtherActor->ActorHasTag("Kill"))
	{
		Respawn();
	}
	AItemPlacement* placement = Cast<AItemPlacement>(OtherActor);
	if(placement!=nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped with placement"));
		if(placement->Mesh->GetStaticMesh() == Mesh->GetStaticMesh())
		{
			SetActorTransform(OtherActor->GetActorTransform());
			bIsPlaced=true;
			GetWorld()->GetTimerManager().ClearTimer(RespawnCooldown);
			OtherActor->Destroy();
			UpdateGameMode();
			UE_LOG(LogTemp, Warning, TEXT("Placed"));
			Mesh->SetMobility(EComponentMobility::Static);

		}
	}
}

void APickupItem::Respawn()
{
	SetActorLocation(RespawnPos);
	Mesh->SetPhysicsLinearVelocity(FVector(0.0f));
	GetWorld()->GetTimerManager().ClearTimer(RespawnCooldown);
}

