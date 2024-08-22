// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CSPaintBox.h"

#include "Carpenter/CarpenterCharacter.h"
#include "Components/CSInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACSPaintBox::ACSPaintBox()
{
	bIsActive = false;
	bStartActive = true;
	bAllowRespawn = true;
	RespawnDelay = 5.0f;
	RespawnDelayRange = 5.0f;

	SetReplicates(true);
}

void ACSPaintBox::BeginPlay()
{
	Super::BeginPlay();
}

void ACSPaintBox::OnRep_IsActive()
{
	if (bIsActive)
	{
		OnRespawned();
	}
	else
	{
		OnPickedUp();
	}
}

void ACSPaintBox::OnRespawned()
{
}

void ACSPaintBox::OnRep_MyPawn()
{
	
}

void ACSPaintBox::OnRep_CahgedMaterial()
{
}

void ACSPaintBox::RespawnPickup()
{
	bIsActive = true;
	OnRespawned();
}

void ACSPaintBox::OnPickedUp_Implementation()
{
	if (MeshComp)
	{
		ChangeMaterial();
	}
}



void ACSPaintBox::ChangeMaterial_Implementation()
{
	if (MyPawn)
	{
		UMaterialInterface* NewMaterial = MeshComp->GetMaterial(0);
		
		MyPawn->CurrentItem->ChangeMaterial(NewMaterial); 
	}
	
}

void ACSPaintBox::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	MyPawn = Cast<ACarpenterCharacter>(InstigatorPawn);
	

	bIsActive = false;
	OnPickedUp();
}

void ACSPaintBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSPaintBox, MyPawn);
	DOREPLIFETIME(ACSPaintBox, bIsActive);
	DOREPLIFETIME(ACSPaintBox, ChangedMaterial)
}