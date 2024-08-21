// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CSBaseInteractiableActor.h"
#include "CSBaseItemActor.generated.h"

class ACarpenterCharacter;

UCLASS()
class CARPENTER_API ACSBaseItemActor : public ACSBaseInteractiableActor
{

	GENERATED_BODY()

public:
	ACSBaseItemActor();

	void Interact_Implementation(APawn* InstigatorPawn);

	void DropItem_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation();

	
	
	UPROPERTY(BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(BlueprintReadOnly)
	UMaterial* ItemMaterial;

	UPROPERTY(BlueprintReadOnly)
	UStaticMesh* ChangedMesh;

protected:

	UPROPERTY(ReplicatedUsing="OnRep_OnEquipped", BlueprintReadOnly) 
	bool bOnEquipped;

	UFUNCTION()
	void OnRep_OnEquipped();

	UPROPERTY(ReplicatedUsing="OnRep_OnDropped", BlueprintReadOnly)
	bool bOnDropped;

	UFUNCTION()
	void OnRep_OnDropped();

	ACarpenterCharacter* PlayerCharacter;
	

};