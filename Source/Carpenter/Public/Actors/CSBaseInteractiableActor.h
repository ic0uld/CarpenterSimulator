// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CSGameplayInterface.h"
#include "CSBaseInteractiableActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class CARPENTER_API ACSBaseInteractiableActor : public AActor, public ICSGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ACSBaseInteractiableActor();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	UStaticMeshComponent* BaseMesh;

};
