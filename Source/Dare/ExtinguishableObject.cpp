// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtinguishableObject.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AExtinguishableObject::AExtinguishableObject()
{

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetWorldScale3D(FVector(3	,3,3));
	BaseMesh->SetVisibility(false);	
}
void AExtinguishableObject::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(FireVfx, BaseMesh, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, false);
	NiagaraComp->Activate();
	NiagaraComp->SetWorldScale3D(FVector(15,15,15));
}
void AExtinguishableObject::DrawOnObject()
{
	if(NiagaraComp->GetRelativeScale3D().Length() < 1)
	{
		NiagaraComp->DestroyComponent();
		BaseMesh->DestroyComponent();
		UE_LOG(LogTemp,Warning,TEXT("Extinguished"))
	}
	else
	{
		NiagaraComp->SetWorldScale3D(NiagaraComp->GetRelativeScale3D()-FVector(0.2,0.2,0.2));
		UE_LOG(LogTemp,Warning,TEXT("Cleaning"));
	}
}
