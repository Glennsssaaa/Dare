// Fill out your copyright notice in the Description page of Project Settings.


#include "DedicatedCamera.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ADedicatedCamera::ADedicatedCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize components
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
}

