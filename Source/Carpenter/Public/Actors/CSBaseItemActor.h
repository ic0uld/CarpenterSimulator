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

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerInteract(APawn* InstigatorPawn);

	UPROPERTY(ReplicatedUsing = OnRep_ItemName)
	FName ItemName;

	UPROPERTY(ReplicatedUsing = OnRep_ItemMaterial)
	UMaterial* ItemMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_ItemMesh)
	UStaticMesh* ChangedMesh;

protected:



	void OnEquipped(APawn* InstigatorPawn);
	void OnDropped();

	UFUNCTION()
	void OnRep_ItemName();

	UFUNCTION()
	void OnRep_ItemMaterial();

	UFUNCTION()
	void OnRep_ItemMesh();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};