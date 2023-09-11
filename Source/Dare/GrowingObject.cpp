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
	//When interacted with, set the objects outline to false
	ObjectMesh->SetRenderCustomDepth(false);
	//If the object is being interacted with, or has finished growing, stop the timer
	if(bInteracted || bFinished)
	{
		bInteracted=false;
		GetWorldTimerManager().ClearTimer(growthTimer);
	}
	else
	{
		//Otherwise, start the timer and call the grow function every 0.01 seconds
		GetWorldTimerManager().SetTimer(growthTimer, this, &AGrowingObject::Grow, 0.01f, true);
		bInteracted=true;
	}
}

void AGrowingObject::Grow()
{
	//Checks if the objects scale is greater than 1, if so, stop the timer and set the objects outline to false
	if(ObjectMesh->GetRelativeScale3D().Length()>1)
	{
		GetWorldTimerManager().ClearTimer(growthTimer);
		bFinished=true;
		//Cast to mage character and set the players frozen state to false
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMageCharacter::StaticClass(), FoundActors);
		AMageCharacter* TempChar = Cast<AMageCharacter>(FoundActors[0]);
		if(TempChar!=nullptr)
		{
			TempChar->bPlayerFrozen=false;
			TempChar->bToggleEarth=false;
		}
		hasGrown=true;
	}
	else
	{
		//Otherwise, increase the objects scale by 0.003 
		FVector NewScale = ObjectMesh->GetRelativeScale3D() + FVector(0.003,0.003,0.003);
		ObjectMesh->SetRelativeScale3D(NewScale);
	}
}

