// Fill out your copyright notice in the Description page of Project Settings.


#include "GrowingObject.h"
#include "Components/BoxComponent.h"

AGrowingObject::AGrowingObject()
{
	bFreezePlayer=true;
}

void AGrowingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrowingObject::Interact()
{
	Super::Interact();
	if(bInteracted)
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

	if(ObjectMesh->GetRelativeScale3D().Length()>5)
	{
		GetWorldTimerManager().ClearTimer(growthTimer);
	}
	else
	{
		FVector NewScale = ObjectMesh->GetRelativeScale3D() + FVector(0.01,0.01,0.01);
		ObjectMesh->SetRelativeScale3D(NewScale);
	}
}

