// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"

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
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &APickupItem::OnOverlapEnd);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnKillOverlap);
	
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
	if(placement!=nullptr && !bIsPlaced && !bIsHeld)
	{
		if(placement->Mesh->GetStaticMesh() == Mesh->GetStaticMesh())
		{
			if(placement!=nullptr)
			{
				placement->Mesh->SetVisibility(false);
				UE_LOG(LogTemp,Warning,TEXT("Placement"))
			}
			SetActorTransform(placement->GetActorTransform());
			bIsPlaced=true;
			Mesh->SetSimulatePhysics(false);
			GetWorld()->GetTimerManager().ClearTimer(RespawnCooldown);
			//Mesh->SetMobility(EComponentMobility::Static);
			if(!bScoreUpdated)
			{
				bScoreUpdated=true;
				UpdateGameMode();
			}
		}
	}
}

void APickupItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	placement = Cast<AItemPlacement>(OtherActor);
}

void APickupItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(placement!=nullptr)
	{
		placement=nullptr;
	}
}

void APickupItem::OnKillOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Kill"))
	{
		Respawn();
	}
}

void APickupItem::Respawn()
{
	SetActorLocation(RespawnPos);
	Mesh->SetPhysicsLinearVelocity(FVector(0.0f));
	GetWorld()->GetTimerManager().ClearTimer(RespawnCooldown);
}

