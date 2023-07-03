// Fill out your copyright notice in the Description page of Project Settings.

#include "ATankCharacter.h"

#include "DestructableObject.h"
#include "Components/BoxComponent.h"

// Sets default values
AATankCharacter::AATankCharacter()
{
	if(!ChargeHitBox)
	{
		ChargeHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ChargeHitBox"));
		ChargeHitBox->SetupAttachment(RootComponent);
		ChargeHitBox->OnComponentBeginOverlap.AddDynamic(this, &AATankCharacter::OnOverlapBegin);
	}
}



// Called when the game starts or when spawned
void AATankCharacter::BeginPlay()
{
	Super::BeginPlay();
	ChargeHitBox->OnComponentBeginOverlap.AddDynamic(this, &AATankCharacter::OnOverlapBegin);

}

// Called every frame
void AATankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AATankCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Step 1")));

	if (OtherActor->ActorHasTag("Destruct") && bIsPlayerDashing)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Step 2")));

		// Smoke fog function at some point
		//dynamic_cast<ADestructableObject>(OtherActor).DoSomethingReallyCoolLater;

		// Instead do this for now
		OtherActor->Destroy();
	}
}
