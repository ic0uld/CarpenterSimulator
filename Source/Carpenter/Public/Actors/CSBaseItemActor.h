// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CSBaseInteractiableActor.h"
#include "CSBaseItemActor.generated.h"

/**
 * 
 */
UCLASS()
class CARPENTER_API ACSBaseItemActor : public ACSBaseInteractiableActor
{
	GENERATED_BODY()

public:

	ACSBaseItemActor();

	void Interact_Implementation(APawn* InstigatorPawn)override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemName, meta=(ExposeOnSpawn))
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemMaterial, meta=(ExposeOnSpawn))
	UMaterial* ItemMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemMesh, meta=(ExposeOnSpawn))
	UStaticMesh* ChangedMesh;

protected:

	virtual void OnDropped();
	virtual void OnEquipped(APawn* InstigatorPawn);

	UFUNCTION()
	void OnRep_ItemName();

	UFUNCTION()
	void OnRep_ItemMaterial();

	UFUNCTION()
	void OnRep_ItemMesh();
	
};
