// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CSGameplayInterface.h"
#include "CSBaseInteractiableActor.generated.h"

class UStaticMeshComponent;
class ACarpenterCharacter;

UCLASS()
class CARPENTER_API ACSBaseInteractiableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSBaseInteractiableActor();
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

public:
	
	virtual void OnBeginFocus();
	
	virtual void OnEndFocus();

	virtual void OnUsed(APawn* InstigatorPawn);

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return MeshComp;
	}
};

