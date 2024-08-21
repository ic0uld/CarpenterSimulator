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

void ACSComputer::OnItemTypeSelected(EItemType SelectedItemType)
{
        if (this->HasAuthority())
        {
          
            ServerSetItemType(SelectedItemType);
        }
        else
        {
         
            ServerSetItemType(SelectedItemType);
        }
}

void ACSComputer::ServerSetItemType_Implementation(EItemType NewItemType)
{
    ItemType = NewItemType;
}

void ACSComputer::OnRep_ItemType()
{
}


void ACSComputer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSComputer, ItemType);
}

