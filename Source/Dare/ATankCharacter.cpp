// Fill out your copyright notice in the Description page of Project Settings.

#include "ATankCharacter.h"
#include "DestructableObject.h"
#include "Components/ArrowComponent.h"
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

	if(!DashAimArrowComponent)
	{
		DashAimArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("DashAimArrowComponent"));
		if(PlayerMesh)
		{
			DashAimArrowComponent->SetupAttachment(PlayerMesh);
		}
		DashAimArrowComponent->SetHiddenInGame(false);
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

void AATankCharacter::TankCharge()
{
	// Show arrow that indicates direction player will charge

	// Dash / Charge when key is released 
}



void AATankCharacter::AbilityOne()
{
	Super::AbilityOne();

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Big Virtual")));

}

void AATankCharacter::AimCharge()
{
	// 1. Player can't move while aiming charge
	bCanPlayerMove = false;

	// 2. Player can use mouse or analogue stick to aim the dash

	
	// 3. Pressing Ability input again makes player charge in the direction
	
}

void AATankCharacter::Charge()
{
	PlayerDash();
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
