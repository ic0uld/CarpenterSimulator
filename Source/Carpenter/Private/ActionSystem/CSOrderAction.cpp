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


void UCSOrderAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		ACSComputer* Computer = Cast<ACSComputer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACSComputer::StaticClass()));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CastingEffect, Computer->TargetPoint->GetActorLocation(), FRotator::ZeroRotator);

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), CastingSound, Computer->TargetPoint->GetActorLocation(), FRotator::ZeroRotator);

		if (Character->HasAuthority())
		{
			SpawnItem(Character);
		}
	}
}


void UCSOrderAction::SpawnItem(ACharacter* InstigatorCharacter)
{
	if (ItemClass)
	{
		ACSComputer* Computer = Cast<ACSComputer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACSComputer::StaticClass()));

		ACarpenterCharacter* Character = Cast<ACarpenterCharacter>(InstigatorCharacter);

		if (Computer && Computer->TargetPoint)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = InstigatorCharacter;
        
			FTransform SpawnTM = Computer->TargetPoint->GetActorTransform();
			ACSBaseItemActor* SpawnedItem = GetWorld()->SpawnActor<ACSBaseItemActor>(ItemClass, SpawnTM, SpawnParams);
			if (SpawnedItem)
			{
				// Set the Static Mesh based on the ItemType
				switch (Computer->ItemType)
				{
				case EItemType::Chair:
					SpawnedItem->MeshComp->SetStaticMesh(Computer->ChairMesh);
					break;
				case EItemType::Door:
					SpawnedItem->MeshComp->SetStaticMesh(Computer->DoorMesh);
					break;
				case EItemType::Cabinet:
					SpawnedItem->MeshComp->SetStaticMesh(Computer->CabinetMesh);
					break;
				case EItemType::Table:
					SpawnedItem->MeshComp->SetStaticMesh(Computer->TableMesh);
					break;
				default:
					break;
				}

				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Spawned"));
			}
		}
	}

	StopAction(InstigatorCharacter);
}