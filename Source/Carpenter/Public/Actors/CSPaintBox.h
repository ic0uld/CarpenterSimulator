// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CSBaseInteractiableActor.h"
#include "CSPaintBox.generated.h"

class UMaterial;

UCLASS()
class CARPENTER_API ACSPaintBox : public ACSBaseInteractiableActor
{
	GENERATED_BODY()
	
public:

	ACSPaintBox();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemMaterial)
	UMaterial* ItemMaterial;

	void Interact_Implementation(APawn* InstigatorPawn);


protected:
	
	UFUNCTION()
	void OnRep_ItemMaterial();
	
	
};
