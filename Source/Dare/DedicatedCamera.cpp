// Fill out your copyright notice in the Description page of Project Settings.


#include "DedicatedCamera.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADedicatedCamera::ADedicatedCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(!Box)
	{
		Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	}
	
	if(!SpringArm)
	{
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
		SpringArm->SetupAttachment(Box);
	}

	if(!Camera)
	{
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		Camera->SetupAttachment(SpringArm);
	}



}

// Called when the game starts or when spawned
void ADedicatedCamera::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADedicatedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!MageCharacter || !TankCharacter)
	{
		MageCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		TankCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 1);
		return;
	}
	
	// Lerp the camera to the midpoint between the two characters
	FVector NewLocation = FMath::Lerp(MageCharacter->GetActorLocation(), TankCharacter->GetActorLocation(), 0.5f);
	
	// Set the Z value of the new location to be the same as the camera's current location
	NewLocation.Z = SpringArm->GetComponentLocation().Z;
	
	// Get distance between the two characters
	const float Distance = FVector::Dist(MageCharacter->GetActorLocation(), TankCharacter->GetActorLocation());
	
	// Clamp values to minimum and maximum zoom distance
	const float ClampedDistance = FMath::Clamp(Distance, MinZoomDistance, MinZoomDistance);

	// Set new spring arm location and target arm length
	SpringArm->SetWorldLocation(NewLocation);
	SpringArm->TargetArmLength = ClampedDistance;
	
}

