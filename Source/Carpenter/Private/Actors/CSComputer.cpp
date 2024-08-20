// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CSComputer.h"
#include "Actors/CSBaseItemActor.h"
#include "Components/CSAcitonComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

ACSComputer::ACSComputer()
{
    
    ScreenUI = CreateDefaultSubobject<UWidgetComponent>("ScreenUI");
    ScreenUI->SetupAttachment(RootComponent);

    ActionComp = CreateDefaultSubobject<UCSAcitonComponent>("ActionComp");
}

void ACSComputer::spawnItem()
{
    if (ensureAlways(ItemClass))
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // Spawn the item on the server
        ACSBaseItemActor* SpawnedItem = GetWorld()->SpawnActor<ACSBaseItemActor>(ItemClass, GetActorLocation(), GetActorRotation(), SpawnParams);

        if (SpawnedItem)
        {
            SpawnedItem->SetReplicates(true);  // Ensure the item replicates to clients
            SpawnedItem->BaseMesh->SetStaticMesh(ChangedMesh);  // Set the item's mesh
            SpawnedItem->ItemName = ItemName;  // Set the item's name
            

            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Actor spawned and replicated successfully"));
        }
    }
}


void ACSComputer::OnRep_ItemName()
{
}

void ACSComputer::OnRep_ItemMaterial()
{
}

void ACSComputer::OnRep_ItemMesh()
{
}

void ACSComputer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSComputer, ItemName);
    DOREPLIFETIME(ACSComputer, ItemMaterial);
    DOREPLIFETIME(ACSComputer, ChangedMesh);
}

