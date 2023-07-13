// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthBridge.h"
#include "Components/StaticMeshComponent.h"

AEarthBridge::AEarthBridge()
{
	BridgeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BridgeMesh"));
	BridgeMesh->SetupAttachment(ObjectMesh);
	BridgeMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BridgeMesh2"));
	BridgeMesh2->SetupAttachment(ObjectMesh);

}

void AEarthBridge::Interact()
{
	Super::Interact();

	UE_LOG(LogTemp, Warning, TEXT("Error"));
}
