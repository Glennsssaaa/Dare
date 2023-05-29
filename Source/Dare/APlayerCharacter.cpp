// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayerCharacter.h"

#include "DareGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "InputConfigData.h"
#include "Components/ArrowComponent.h"
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
	
	PEI->BindAction(InputActions->InputKeyboardMove, ETriggerEvent::Triggered, this, &AAPlayerCharacter::KeyboardMove);
	PEI->BindAction(InputActions->InputInteract, ETriggerEvent::Started, this, &AAPlayerCharacter::Interact);

}


// Called when the game starts or when spawned
void AAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// lerp the player's direction to the direction variable
	if(MoveValue.Y !=0 || MoveValue.X!=0 && !Interacting)
	{
		PlayerDirection = FVector(MoveValue.Y,MoveValue.X,0);
		PlayerMesh->SetWorldRotation(FMath::Lerp(PlayerMesh->GetComponentRotation(), UKismetMathLibrary::MakeRotFromX(PlayerDirection), DeltaTime * RotationSpeed));
	}

}

void AAPlayerCharacter::KeyboardMove(const FInputActionValue& Value)
{
	if(!Interacting){
        MoveValue=Value.Get<FVector2D>();
        //Add movement input
        if(MoveValue.Y != 0)
            AddMovementInput(GetActorForwardVector(), MoveValue.Y * MovementSpeed);
    
        if(MoveValue.X!=0)
        {
            AddMovementInput(GetActorRightVector(), MoveValue.X * MovementSpeed);
        }
    }
}

void AAPlayerCharacter::Interact(const FInputActionValue& Value)
{
	//print interact
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	APlayerController* PC = Cast<APlayerController>(GetController());
	if(InteractableActor!=NULL){
		//InteractableActor->Destroy();
	}
}


	