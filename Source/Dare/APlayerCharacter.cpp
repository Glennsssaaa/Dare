// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayerCharacter.h"

#include "DareGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "InputConfigData.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AAPlayerCharacter::AAPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));

	PlayerMesh->SetupAttachment(GetRootComponent());
	
	DirectionArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	DirectionArrowComponent->SetupAttachment(PlayerMesh);
	
	DirectionArrowComponent->SetHiddenInGame(false);
}

// Called to bind functionality to input
void AAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	//print the controller index
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	//Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping,0);
	
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PC->SetShowMouseCursor(true);
	PEI->BindAction(InputActions->InputKeyboardMove, ETriggerEvent::Triggered, this, &AAPlayerCharacter::KeyboardMove);
    PEI->BindAction(InputActions->InputAim, ETriggerEvent::Triggered, this, &AAPlayerCharacter::Aim);
	PEI->BindAction(InputActions->InputInteract, ETriggerEvent::Started, this, &AAPlayerCharacter::Interact);
	PEI->BindAction(InputActions->InputInteract, ETriggerEvent::Completed, this, &AAPlayerCharacter::InteractEnd);
	PEI->BindAction(InputActions->InputDash, ETriggerEvent::Started, this, &AAPlayerCharacter::PlayerDash);
}


// Called when the game starts or when spawned
void AAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPlayerCharacter::AbilityOne()
{
}

// Called every frame
void AAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// lerp the player's direction to the direction variable
	if(LookValue.Y !=0 || LookValue.X!=0)
	{
		PlayerDirection = FVector(-LookValue.Y,LookValue.X,0);
		PlayerMesh->SetWorldRotation(FMath::Lerp(PlayerMesh->GetComponentRotation(), UKismetMathLibrary::MakeRotFromX(PlayerDirection), DeltaTime * RotationSpeed));
	}
	//Player Direction Mouse Controls
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, mouseHit);
	playerDirection = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mouseHit.Location);
	PlayerMesh->SetWorldRotation(FMath::Lerp(PlayerMesh->GetComponentRotation(), FRotator(0,playerDirection.Yaw,0), DeltaTime*RotationSpeed));

	// Reduce Dash timer if needed
	if(DashCooldown > 0.f)
	{
		DashCooldown -= DeltaTime;
		
		// Clamp Dash Cooldown to 0
		if(DashCooldown < 0.f)
		{
			DashCooldown = 0.f;
		}
		
		if (DashCooldown <= 0.f && DashCharges < DashChargesMax)
		{
			DashCharges++;
			if(DashCharges < DashChargesMax)
			{
				DashCooldown = DashCooldownDefault;
			}
		}
	}

	// Print Dash Cooldown to screen
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Dash Cooldown: %f"), DashCooldown));

	// Print Dash Charges to screen
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Dash Charges: %d"), DashCharges));
}

void AAPlayerCharacter::KeyboardMove(const FInputActionValue& Value)
{
	
	MoveValue=Value.Get<FVector2D>();
	//Add movement input
	if(MoveValue.Y != 0)
		AddMovementInput(GetActorForwardVector(), MoveValue.Y * MovementSpeed);

	if(MoveValue.X!=0)
	{
		AddMovementInput(GetActorRightVector(), MoveValue.X * MovementSpeed);
	}
	
}

void AAPlayerCharacter::Aim(const FInputActionValue& Value){
    LookValue=Value.Get<FVector2D>();

    if(LookValue.X==0){
        LookValue.X=1;    
    }
        PlayerDirection = FVector(LookValue.Y,LookValue.X,0);
}

void AAPlayerCharacter::Interact(const FInputActionValue& Value)
{

}

void AAPlayerCharacter::InteractEnd(const FInputActionValue& Value) {

}

void AAPlayerCharacter::PlayerDash()
{
	// Check if the player is already dashing or if the dash is on cooldown
	if(bIsPlayerDashing || DashCharges == 0) return;

	// Disable Player Input to prevent player movement during dash
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UE_LOG(LogTemp, Warning, TEXT("Dash"));
	
	bIsPlayerDashing = true;
	// Find the predicted location of the player after the dash
	PredictedLocation = (DirectionArrowComponent->GetForwardVector() * DashDistance) + GetActorLocation();

	DashCharges--;
	
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
			EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			// Clear Dash timer to stop function running every frame
			GetWorld()->GetTimerManager().ClearTimer(DashCooldownTimerHandle);
				
			bIsPlayerDashing = false;

			// Only reset the cooldowm
			if(DashCharges == DashChargesMax - 1)
			{
				DashCooldown = DashCooldownDefault;
			}
		}
	}, GetWorld()->DeltaTimeSeconds / 2.75, true, 0.0f);
	
}




	