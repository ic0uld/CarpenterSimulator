// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CSComputer.h"
#include "Actors/CSBaseItemActor.h"
#include "Components/CSAcitonComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

ACSComputer::ACSComputer()
{
    bReplicates = true;
    
    ScreenUI = CreateDefaultSubobject<UWidgetComponent>("ScreenUI");
    ScreenUI->SetupAttachment(RootComponent);

    ActionComp = CreateDefaultSubobject<UCSAcitonComponent>("ActionComp");
}

void ACSComputer::RequestSpawnActor(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh)
{
    if (HasAuthority())
    {
        SpawnActor(Location, Rotation, ItemName, StaticMesh);
    }
    else
    {
        ServerSpawnActor(Location, Rotation, ItemName, StaticMesh);
    }
}

void ACSComputer::ServerSpawnActor_Implementation(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh)
{
    SpawnActor(Location, Rotation, ItemName, StaticMesh);
}

bool ACSComputer::ServerSpawnActor_Validate(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh)
{
    return true;
}

void ACSComputer::SpawnActor(const FVector& Location, const FRotator& Rotation, const FName& ItemName, UStaticMesh* StaticMesh)
{

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    SpawnedActor = GetWorld()->SpawnActor<ACSBaseItemActor>(ItemClass, Location, Rotation, SpawnParams);

    if (SpawnedActor)
    {
            SpawnedActor->SetReplicates(true);
            SpawnedActor->BaseMesh->SetStaticMesh(StaticMesh);
            SpawnedActor->ItemName = ItemName;

            OnRep_SpawnedActor(SpawnedActor);

            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Actor spawned and replicated successfully"));
    }
}

void ACSComputer::OnRep_SpawnedActor(ACSBaseItemActor* SpawnedActorX)
{
    if (SpawnedActorX)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Actor replicated to client"));
    }
}

void ACSComputer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSComputer, SpawnedActor);
}
