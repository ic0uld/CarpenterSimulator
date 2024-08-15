// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UCSAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARPENTER_API UCSAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
public:	

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UCSAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	UCSAttributeComponent();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float RageMax;
	

	UFUNCTION(NetMulticast, Reliable) 
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable) 
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:	

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax() const;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	float GetRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRage(AActor* InstigatorActor, float Delta);

		
};
