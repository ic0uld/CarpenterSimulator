// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CSPaintBox.h"

#include "Net/UnrealNetwork.h"

ACSPaintBox::ACSPaintBox()
{
	
}

void ACSPaintBox::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	
}

void ACSPaintBox::OnRep_ItemMaterial()
{
	
}

void ACSPaintBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSPaintBox, ItemMaterial);
}

