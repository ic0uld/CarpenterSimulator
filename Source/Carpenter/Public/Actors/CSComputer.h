// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSBaseItemActor.h"
#include "Actors/CSBaseInteractiableActor.h"
#include "Components/CSAcitonComponent.h"
#include "CSComputer.generated.h"

class UWidgetComponent;
class ATargetPoint;

UCLASS()
class CARPENTER_API ACSComputer : public ACSBaseInteractiableActor
{
	GENERATED_BODY()
	
public:
	ACSComputer();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void ServerSetChangedMesh(UStaticMesh* NewMesh);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void ServerSetItemName(const FName& NewItemName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	UMaterial* ItemMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ChangedMesh)
	UStaticMesh* ChangedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATargetPoint* TargetPoint;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* ScreenUI;

	UFUNCTION()
	void OnRep_ChangedMesh();


	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ACSBaseItemActor> ItemClass;
	
	
};