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

	/* Sphere radius of the sweep to find desired target under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category="Targeting")
	float SweepRadius;

	/* Fallback distance when sweep finds no collision under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category="Targeting")
	float SweepDistanceFallback;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	/* Sound Effect to play (Can be Wave or Cue) */
	UPROPERTY(EditAnywhere, Category = "Attack")
	USoundBase* CastingSound;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

};