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
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	UStaticMesh* CubeMeshRef = CubeMeshAsset.Object;
	CubeMesh->SetStaticMesh(CubeMeshRef);
	CubeMesh->SetupAttachment(GetRootComponent());
	CubeMesh->SetRelativeScale3D(FVector3d(0.5f,0.5f,1.0f));

	DirectionArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	DirectionArrowComponent->SetupAttachment(CubeMesh);
	DirectionArrowComponent->SetHiddenInGame(false);
}

// Called to bind functionality to input
void AAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping,0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(InputActions->InputMove,ETriggerEvent::Triggered, this, &AAPlayerCharacter::Move);

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
	if(MoveValue.Y !=0 || MoveValue.X!=0)
	{
		PlayerDirection = FVector(MoveValue.Y,MoveValue.X,0);
		CubeMesh->SetWorldRotation(FMath::Lerp(CubeMesh->GetComponentRotation(), UKismetMathLibrary::MakeRotFromX(PlayerDirection), DeltaTime * RotationSpeed));
	}
}

void AAPlayerCharacter::Move(const FInputActionValue& Value)
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


	