// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CSOrderActor.h"

#include "Actors/CSBaseItemActor.h"
#include "Carpenter/CarpenterCharacter.h"
#include "Components/CSAttributeComponent.h"
#include "Net/UnrealNetwork.h"


ACSOrderActor::ACSOrderActor()
{

	AttributeComponent = CreateDefaultSubobject<UCSAttributeComponent>("AttributeComp");

	bIsActive = false;
	bStartActive = true;
	bAllowRespawn = true;
	RespawnDelay = 5.0f;
	RespawnDelayRange = 5.0f;

	SetReplicates(true);
}

void ACSOrderActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACSOrderActor::OnRep_IsActive()
{
	
	OnPickedUp();
	
}

void ACSOrderActor::OnRespawned()
{
}

void ACSOrderActor::OnRep_MyPawn()
{
	
}

void ACSOrderActor::OnRep_PaintName()
{
}

void ACSOrderActor::OnRep_ItemType()
{
}

void ACSOrderActor::OnPickedUp_Implementation()
{
	if (MeshComp)
	{
		FinishOrder();
	}
}


void ACSOrderActor::FinishOrder_Implementation()
{
	if (MyPawn)
	{
		if (MyPawn->CurrentItem)
		{
			ACSBaseItemActor* Item = MyPawn->CurrentItem;
			if (MyPawn->CurrentItem->ItemType == ItemType)
			{
				if (MyPawn->CurrentItem->PaintName == PaintName)
				{
				  AttributeComponent->ApplyMoney(this, 50);
				}
				else
				{
					AttributeComponent->ApplyMoney(this, 25);
				}
			}
			
			Item->Destroy();
		}
	}
	
}

void ACSOrderActor::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	MyPawn = Cast<ACarpenterCharacter>(InstigatorPawn);
	

	bIsActive = false;
	OnPickedUp();
}

void ACSOrderActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSOrderActor, MyPawn);
	DOREPLIFETIME(ACSOrderActor, bIsActive);
	DOREPLIFETIME(ACSOrderActor, PaintName)
	DOREPLIFETIME(ACSOrderActor, ItemType)
}