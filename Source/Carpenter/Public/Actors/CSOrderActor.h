// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CSBaseInteractiableActor.h"
#include "Carpenter/Carpenter.h"
#include "CSOrderActor.generated.h"

class UMaterial;
class UCSInteractionComponent;
class USoundCue;
class UCSAttributeComponent;

UCLASS()
class CARPENTER_API ACSOrderActor : public ACSBaseInteractiableActor
{
	GENERATED_BODY()

	ACSOrderActor();

	
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* PickupSound;

	UFUNCTION()
	void OnRep_IsActive();

	virtual void OnRespawned();



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
	UCSAttributeComponent* AttributeComponent;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Transient, ReplicatedUsing = OnRep_PaintName)
	FName PaintName;
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	class ACarpenterCharacter* MyPawn;

	UFUNCTION()
	void OnRep_MyPawn();


	UFUNCTION()
	void OnRep_PaintName();

	UFUNCTION()
	void OnRep_ItemType();
	

	UFUNCTION(Server, Reliable)
	virtual void OnPickedUp();


	UFUNCTION(NetMulticast, Reliable)
	void FinishOrder();

	
	
public:

	virtual void OnUsed(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ItemType)
	EItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	bool bStartActive;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	bool bAllowRespawn;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnDelayRange;

	
	
};
