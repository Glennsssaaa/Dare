// Fill out your copyright notice in the Description page of Project Settings.


#include "GrowingObject.h"
#include "Components/BoxComponent.h"
#include "MageCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AGrowingObject::AGrowingObject()
{
	bFreezePlayer=true;
	bInteracted=false;
}

void AGrowingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrowingObject::Interact()
{
	//Super::Interact();
	ObjectMesh->SetRenderCustomDepth(false);
	if(bInteracted || bFinished)
	{
		bInteracted=false;
		GetWorldTimerManager().ClearTimer(growthTimer);
	}
	else
	{
		GetWorldTimerManager().SetTimer(growthTimer, this, &AGrowingObject::Grow, 0.01f, true);
		bInteracted=true;
	}
}

void AGrowingObject::Grow()
{

}

