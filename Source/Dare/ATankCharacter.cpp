// Fill out your copyright notice in the Description page of Project Settings.

#include "ATankCharacter.h"
#include "DestructableObject.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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
	DashAimArrowComponent->SetVisibility(false);
	DashDistance = 1350.f;

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
	// Do ability

	if(!bIsAbility)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Begin")));
		bCanPlayerMove = false;

		DashAimArrowComponent->SetVisibility(true);
		bIsAbility = true;
		GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, [this]()
		{
			bIsAbility = false;
		}, 5.f, false);

		AimCharge();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("End")));

		// 2. Do Dash
		Charge();
		bIsAbility = false;
		bCanPlayerMove = true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Big Virtual")));

}

void AATankCharacter::AimCharge()
{
	
}

void AATankCharacter::Charge()
{
	// Disable Player Input to prevent player movement during dash
	bCanPlayerMove = false;
	
	bIsPlayerDashing = true;
	
	// Find the predicted location of the player after the dash
	PredictedLocation = (DirectionArrowComponent->GetForwardVector() * 2000.f) + GetActorLocation();

	
	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, [this]()
	{
		FHitResult SweepHitResult;
			
		// Set actor location using interpolation and check if there is any collision in the way
		SetActorLocation(FMath::Lerp(GetActorLocation(), PredictedLocation, GetWorld()->GetDeltaSeconds() * DashSpeed), true, &SweepHitResult);

		const FVector2D ActorLocation2D = FVector2D(GetActorLocation().X, GetActorLocation().Y);
		const FVector2D PredictedLocation2D = FVector2D(PredictedLocation.X, PredictedLocation.Y);
			
		if (SweepHitResult.bBlockingHit || ActorLocation2D.Equals(PredictedLocation2D, 100.f))
		{
			// Location reached, activate dash cooldown, re-enable input and camera lag
			bCanPlayerMove = true;
			DashAimArrowComponent->SetVisibility(false);

			// Clear Dash timer to stop function running every frame
			GetWorld()->GetTimerManager().ClearTimer(DashCooldownTimerHandle);
			
			bIsPlayerDashing = false;
		
			// Only reset the cooldown
			if(DashCharges == DashChargesMax - 1)
			{
				DashCooldown = DashCooldownDefault;	
			}
		}
	}, GetWorld()->DeltaTimeSeconds / 2.75, true, 0.0f);
	
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
