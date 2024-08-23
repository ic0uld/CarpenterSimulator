// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CSBaseInteractiableActor.h"
#include "Carpenter/Carpenter.h"
#include "CSBaseItemActor.generated.h"

class ACarpenterCharacter;
class USoundCue;

UCLASS()
class CARPENTER_API ACSBaseItemActor : public ACSBaseInteractiableActor
{

	GENERATED_BODY()

public:
	ACSBaseItemActor();

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ItemType, meta = (ExposeOnSpawn))
	EItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* PickupSound;

	UFUNCTION()
	void OnRep_IsActive();

	UFUNCTION()
	void OnRep_ItemType(EItemType NewType);

	virtual void OnRespawned();
	
	UFUNCTION(NetMulticast, Reliable)
	void ChangeMaterial(UMaterialInterface* NewMaterial, FName ColorName);

	UFUNCTION(NetMulticast, Reliable)
	void ChangeMesh(UStaticMesh* NewMesh,EItemType NewType);



protected:

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	class ACarpenterCharacter* MyPawn;

	UFUNCTION()
	void OnRep_MyPawn();

	virtual void RespawnPickup();

	UFUNCTION(Server, Reliable)
	virtual void OnPickedUp();

	UFUNCTION(NetMulticast, Reliable)
	void SetCollision(FName NewCollisionProfile);


	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_PaintName,  Category = "Paint")
	FName PaintName;

	UFUNCTION()
	void OnRep_PaintName(FName NewColor);
	
};


