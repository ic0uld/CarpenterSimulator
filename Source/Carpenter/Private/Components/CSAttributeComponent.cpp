// Fill out your copyright notice in the Description page of Project Settings.


#include "Carpenter/Public/Components/CSAttributeComponent.h"

#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


UCSAttributeComponent::UCSAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	Rage = 0;
	RageMax = 100;

	SetIsReplicatedByDefault(true);
}


bool UCSAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}


bool UCSAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool UCSAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}


float UCSAttributeComponent::GetHealth() const
{
	return Health;
}

float UCSAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}


bool UCSAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}
		
	}

	return ActualDelta != 0;
}


float UCSAttributeComponent::GetRage() const
{
	return Rage;
}


bool UCSAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;
	if (ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
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


bool UCSAttributeComponent::IsActorAlive(AActor* Actor)
{
	UCSAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}


void UCSAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UCSAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}


void UCSAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCSAttributeComponent, Health);
	DOREPLIFETIME(UCSAttributeComponent, HealthMax);

	DOREPLIFETIME(UCSAttributeComponent, Rage);
	DOREPLIFETIME(UCSAttributeComponent, RageMax);
}