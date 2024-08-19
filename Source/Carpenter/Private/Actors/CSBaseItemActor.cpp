// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CSBaseItemActor.h"

#include "Carpenter/CarpenterCharacter.h"
#include "Components/CSInteractionComponent.h"
#include "Net/UnrealNetwork.h"

ACSBaseItemActor::ACSBaseItemActor()
{

}

void ACSBaseItemActor::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (ensure(InstigatorPawn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("InteracrItem"));

		OnEquipped(InstigatorPawn);
	}
}

void ACSBaseItemActor::OnDropped()
{
	
	
	BaseMesh->SetSimulatePhysics(true);
	BaseMesh->SetEnableGravity(true);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BaseMesh->MarkRenderStateDirty();
	
}

void ACSBaseItemActor::OnEquipped(APawn* InstigatorPawn)
{

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("OnEquipped"));

	ACarpenterCharacter* PlayerCharacter = CastChecked<ACarpenterCharacter>(InstigatorPawn);

	UCSInteractionComponent* InteractionComp = PlayerCharacter->FindComponentByClass<UCSInteractionComponent>();

	if (PlayerCharacter && InteractionComp)
	{
		if (!InteractionComp->EquippedItem)
		{
			InteractionComp->ServerEquippedItem();
			
			BaseMesh->SetSimulatePhysics(false);
			BaseMesh->SetEnableGravity(false);
			BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	
	

}

void ACSBaseItemActor::OnRep_ItemName()
{
}

void ACSBaseItemActor::OnRep_ItemMaterial()
{
}

void ACSBaseItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSBaseItemActor, ItemName);
	DOREPLIFETIME(ACSBaseItemActor, ItemMaterial);
}