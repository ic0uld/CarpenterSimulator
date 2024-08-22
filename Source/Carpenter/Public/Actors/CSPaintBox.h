// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CSBaseInteractiableActor.h"
#include "CSPaintBox.generated.h"

class UMaterial;
class UCSInteractionComponent;
class USoundCue;

UCLASS()
class CARPENTER_API ACSPaintBox : public ACSBaseInteractiableActor
{
	GENERATED_BODY()
	
public:

	ACSPaintBox();
	

	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* PickupSound;

	UFUNCTION()
	void OnRep_IsActive();

	virtual void OnRespawned();



protected:

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;
	
	UPROPERTY(EditDefaultsOnly, Transient, ReplicatedUsing = OnRep_CahgedMaterial)
	UMaterialInterface*  ChangedMaterial;
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	class ACarpenterCharacter* MyPawn;

	UFUNCTION()
	void OnRep_MyPawn();

	UFUNCTION()
	void OnRep_CahgedMaterial();
	

	virtual void RespawnPickup();

	UFUNCTION(Server, Reliable)
	virtual void OnPickedUp();


	UFUNCTION(NetMulticast, Reliable)
	void ChangeMaterial();

	
	
public:

	virtual void OnUsed(APawn* InstigatorPawn) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	bool bStartActive;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	bool bAllowRespawn;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnDelayRange;

	
};
