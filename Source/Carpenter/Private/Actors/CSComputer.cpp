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

}

void ACSComputer::ServerSetItemName_Implementation(const FName& NewItemName)
{
}

void ACSComputer::OnRep_ChangedMesh()
{
  
}


void ACSComputer::ServerSetChangedMesh_Implementation(UStaticMesh* NewMesh)
{
    if (ChangedMesh != NewMesh)
    {
        ChangedMesh = NewMesh;
        OnRep_ChangedMesh();
    }
}


void ACSComputer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSComputer, ChangedMesh);
}

