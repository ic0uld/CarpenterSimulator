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

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void RequestSpawnActor(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* ScreenUI;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ACSBaseItemActor> ItemClass;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void ServerSpawnActor(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh);
	void ServerSpawnActor_Implementation(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh);
	bool ServerSpawnActor_Validate(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh);

	void SpawnActor(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh);

	UFUNCTION()
	void OnRep_SpawnedActor(ACSBaseItemActor* SpawnedActor);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_SpawnedActor)
	ACSBaseItemActor* SpawnedActor;
};