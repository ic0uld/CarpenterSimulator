// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSAction.h"
#include "Actors/CSComputer.h"
#include "CSOrderAction.generated.h"

class ACSBaseItemActor;
class UParticleSystem;

UCLASS()
class CARPENTER_API UCSOrderAction : public UCSAction
{
	GENERATED_BODY()
	
public:
	UCSOrderAction();
	
protected:

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<ACSBaseItemActor> ItemClass;
	
	UPROPERTY(EditAnywhere, Category = "Spawn")
	UParticleSystem* CastingEffect;
	
	UPROPERTY(EditAnywhere, Category = "Spawn")
	USoundBase* CastingSound;

	UFUNCTION()
	void SpawnItem(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

};