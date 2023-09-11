//Class for objects that can be extinguished by the mage's water spell
#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "ObjectBase.h"
#include "ExtinguishableObject.generated.h"

/**
 * 
 */
UCLASS()
class DARE_API AExtinguishableObject : public AObjectBase
{
	GENERATED_BODY()
protected:
	AExtinguishableObject();
	
public:
	void BeginPlay() override;

	//Called while the mage's water spell is hitting this object
	UFUNCTION(BlueprintCallable)
	void DrawOnObject();

	//Fire VFX
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraSystem* FireVfx;
	//Poof VFX
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraSystem* PoofVfx;
	//Base mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BaseMesh;
	//Niagara component
	UNiagaraComponent* NiagaraComp;
};
