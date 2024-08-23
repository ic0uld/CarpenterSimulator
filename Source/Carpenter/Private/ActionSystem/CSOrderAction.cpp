// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CSOrderAction.h"

#include "Actors/CSComputer.h"
#include "Carpenter/CarpenterCharacter.h"
#include "Components/CSAttributeComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UCSOrderAction::UCSOrderAction()
{
}

void UCSOrderAction::SpawnItem_Implementation(ACharacter* InstigatorCharacter)
{
	if (ItemClass)
	{
		ACSComputer* Computer = Cast<ACSComputer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACSComputer::StaticClass()));
		
		if (Computer && Computer->TargetPoint)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = InstigatorCharacter;

			// Ensure the item is replicated
			SpawnParams.bNoFail = true;  // Ensure actor is spawned even if outside level bounds

			FTransform SpawnTM = Computer->TargetPoint->GetActorTransform();
			ACSBaseItemActor* SpawnedItem = GetWorld()->SpawnActor<ACSBaseItemActor>(ItemClass, SpawnTM, SpawnParams);
			
			switch (Computer->ItemType)
			{
			case EItemType::Chair:
				SpawnedItem->ChangeMesh(Computer->ChairMesh, EItemType::Chair);
				break;
			case EItemType::Door:
				SpawnedItem->ChangeMesh(Computer->DoorMesh, EItemType::Door);
				break;
			case EItemType::Cabinet:
				SpawnedItem->ChangeMesh(Computer->CabinetMesh, EItemType::Cabinet);
				break;
			case EItemType::Table:
				SpawnedItem->ChangeMesh(Computer->TableMesh, EItemType::Table);
				break;
			default:
				break;
			}
			
		}
	}

	StopAction(InstigatorCharacter);
}

void UCSOrderAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		ACSComputer* Computer = Cast<ACSComputer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACSComputer::StaticClass()));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CastingEffect, Computer->TargetPoint->GetActorLocation(), FRotator::ZeroRotator);

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), CastingSound, Computer->TargetPoint->GetActorLocation(), FRotator::ZeroRotator);
		if (Instigator->HasAuthority())
		{
			SpawnItem(Character);
		}
		
	}
}

/*
void UCSOrderAction::SpawnItem(ACharacter* InstigatorCharacter)
{
	
}

*/
