// Fill out your copyright notice in the Description page of Project Settings.


#include "Carpenter/Public/Components/CSAttributeComponent.h"

#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


UCSAttributeComponent::UCSAttributeComponent()
{

	Money = 0;
	Money = 999999;

	SetIsReplicatedByDefault(true);
}


float UCSAttributeComponent::GetMoney() const
{
	return Money;
}


bool UCSAttributeComponent::ApplyMoney(AActor* InstigatorActor, float Delta)
{
	float OldMoney = Money;

	Money = FMath::Clamp(Money + Delta, 0.0f, MaxMoney);

	float ActualDelta = Money - OldMoney;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Money, ActualDelta);
	}

	return ActualDelta != 0;
}



UCSAttributeComponent* UCSAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCSAttributeComponent>(FromActor->GetComponentByClass(UCSAttributeComponent::StaticClass()));
	}

	return nullptr;
}



void UCSAttributeComponent::MulticastMoneyChanged_Implementation(AActor* InstigatorActor, float NewMoney, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewMoney, Delta);
}


void UCSAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCSAttributeComponent, Money);
	DOREPLIFETIME(UCSAttributeComponent, MaxMoney);

	
}