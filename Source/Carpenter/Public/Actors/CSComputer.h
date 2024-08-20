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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components" , meta = (AllowPrivateAccess = "true"))
	UCSAcitonComponent* ActionComp;
	
public:
	ACSComputer();

	UFUNCTION(BlueprintCallable, Category= "Spawn")
	void spawnItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemName, meta=(ExposeOnSpawn))
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemMaterial, meta=(ExposeOnSpawn))
	UMaterial* ItemMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemMesh, meta=(ExposeOnSpawn))
	UStaticMesh* ChangedMesh;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* ScreenUI;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ACSBaseItemActor> ItemClass;

	UFUNCTION()
	void OnRep_ItemName();

	UFUNCTION()
	void OnRep_ItemMaterial();

	UFUNCTION()
	void OnRep_ItemMesh();
	
	
};