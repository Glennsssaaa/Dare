// Fill out your copyright notice in the Description page of Project Settings.

#include "ATankCharacter.h"
#include "DestructableObject.h"
#include "EnhancedInputComponent.h"
#include "InputConfigData.h"
#include "RebuildableBase.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AATankCharacter::AATankCharacter()
{
	if(!ChargeHitBox)
	{
		ChargeHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ChargeHitBox"));
		ChargeHitBox->SetupAttachment(PlayerMesh);
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
	DashAimArrowComponent->SetVisibility(false);
	ChargeHitBox->OnComponentBeginOverlap.AddDynamic(this, &AATankCharacter::OnOverlapBegin);
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

void AATankCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	//print the controller index
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	//Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping,0);

	//Enhanced Input Setup
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PC->SetShowMouseCursor(true);
	

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

void AATankCharacter::AbilityTwo()
{
	Super::AbilityTwo();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Rebuild Input")));

	// If building destroyed, rebuild it
	if(Rebuildable->GetIsDestroyed())
	{
		Rebuild();
	}
}

void AATankCharacter::Rebuild()
{
	// Check if overlapping with house rebuild collision
	// TArray<UPrimitiveComponent*>& OutOverlappingComponents
	//  ChargeHitBox->GetOverlappingComponents(TArray<UPrimitiveComponent*>& OutOverlappingComponents);
	if(bIsInRebuildZone && Rebuildable)
	{
		Rebuildable->SetIsRebuilding(true);	
	}


	
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
		DashAimArrowComponent->SetVisibility(false);
		
		FHitResult SweepHitResult;
		
		// Set actor location using interpolation and check if there is any collision in the way
		SetActorLocation(FMath::Lerp(GetActorLocation(), PredictedLocation, GetWorld()->GetDeltaSeconds() * DashSpeed), true, &SweepHitResult);

		const FVector2D ActorLocation2D = FVector2D(GetActorLocation().X, GetActorLocation().Y);
		const FVector2D PredictedLocation2D = FVector2D(PredictedLocation.X, PredictedLocation.Y);
			
		if (SweepHitResult.bBlockingHit || ActorLocation2D.Equals(PredictedLocation2D, 100.f))
		{
			// Location reached, activate dash cooldown, re-enable input and camera lag
			bCanPlayerMove = true;

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
	if (OtherActor->ActorHasTag("Destruct") && bIsPlayerDashing)
	{
		// Smoke fog function at some point
		//dynamic_cast<ADestructableObject>(OtherActor).DoSomethingReallyCoolLater;

		// Instead do this for now
		OtherActor->Destroy();
	}

	if(OtherComp->ComponentHasTag("Rebuild"))
	{

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Big Rebuild")));
		// ARebuildableBase* OverlappedCharacter = Cast<ARebuildableBase>(OtherActor);
		// OverlappedCharacter->ToggleHouseDestruction();
		Rebuildable = Cast<ARebuildableBase>(OtherActor);
		bIsInRebuildZone = true;
		
		
	}

	if(OtherComp->ComponentHasTag("HouseCollision"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Big Destory")));
		ARebuildableBase* OverlappedCharacter = Cast<ARebuildableBase>(OtherActor);

		if(OverlappedCharacter->GetIsDestroyed() == false)
		{
			OverlappedCharacter->ToggleHouseDestruction();
		}
	}
	
}

void AATankCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	// Player left build zone, stop rebuilding
	if(OtherComp->ComponentHasTag("Rebuild"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Left rebuild area")));
		Rebuildable->SetIsRebuilding(false);
	//	Rebuildable = nullptr;
		bIsInRebuildZone = false;
	}
}
