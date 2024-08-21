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

	
	UCSAttributeComponent();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Money;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float MaxMoney;
	
	UFUNCTION(NetMulticast, Unreliable) 
	void MulticastMoneyChanged(AActor* InstigatorActor, float NewMoney, float Delta);





public:	

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnRageChanged;


	UFUNCTION(BlueprintCallable)
	float GetMoney() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyMoney(AActor* InstigatorActor, float Delta);

};
