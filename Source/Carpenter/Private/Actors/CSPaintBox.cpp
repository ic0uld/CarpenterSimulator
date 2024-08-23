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
	OnPickedUp();
}


void ACSPaintBox::OnRep_MyPawn()
{
	
}


void ACSPaintBox::OnRep_PaintName()
{
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
		if (MeshComp)
		{
			UMaterialInterface* NewMaterial = MeshComp->GetMaterial(0);
			if (NewMaterial)
			{
				if (MyPawn->CurrentItem)
				{
					MyPawn->CurrentItem->ChangeMaterial(NewMaterial, PaintName);
				}
			}
		
			
		}
	
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
	DOREPLIFETIME(ACSPaintBox, PaintName)
}