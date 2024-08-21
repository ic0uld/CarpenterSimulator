#include "Actors/CSBaseItemActor.h"
#include "Carpenter/CarpenterCharacter.h"
#include "Components/CSInteractionComponent.h"
#include "Net/UnrealNetwork.h"

ACSBaseItemActor::ACSBaseItemActor()
{
    bReplicates = true;
    bOnEquipped = false;
    bOnDropped = false;

    PlayerCharacter = nullptr;
}

void ACSBaseItemActor::Interact_Implementation(APawn* InstigatorPawn)
{
    PlayerCharacter = Cast<ACarpenterCharacter>(InstigatorPawn);
  
   
        bOnEquipped = !bOnEquipped;
        OnRep_OnEquipped();
    
}

void ACSBaseItemActor::DropItem_Implementation(APawn* InstigatorPawn)
{

    PlayerCharacter = Cast<ACarpenterCharacter>(InstigatorPawn);
    
    bOnDropped = !bOnDropped;
    OnRep_OnDropped();
}

void ACSBaseItemActor::OnActorLoaded_Implementation()
{
    OnRep_OnEquipped();
    OnRep_OnDropped();
}


void ACSBaseItemActor::OnRep_OnEquipped()
{
    if (PlayerCharacter)
    {
        UCSInteractionComponent* InteractionComp = PlayerCharacter->FindComponentByClass<UCSInteractionComponent>();

        if (InteractionComp && !InteractionComp->EquippedItem)
        {
            BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            BaseMesh->SetCollisionProfileName("NoCollision");

            FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
            BaseMesh->AttachToComponent(PlayerCharacter->GetMesh(), AttachRules, TEXT("HandSocketName"));
            
            if (USkeletalMeshComponent* SkeletalMesh = PlayerCharacter->Mesh1P)
            {
                this->AttachToComponent(SkeletalMesh, AttachRules, TEXT("ItemSnapLocation"));
                InteractionComp->EquippedItem = this;
                bOnEquipped = !bOnEquipped;
            }
        }
    }
}

void ACSBaseItemActor::OnRep_OnDropped()
{
    UCSInteractionComponent* InteractionComp = PlayerCharacter->FindComponentByClass<UCSInteractionComponent>();
    if (InteractionComp)
    {
        bOnDropped = false;
        InteractionComp->EquippedItem = nullptr;
        BaseMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        BaseMesh->SetSimulatePhysics(true);
        BaseMesh->SetEnableGravity(true);
        BaseMesh->SetCollisionProfileName("BlockAllDynamic");
        BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    
}

void ACSBaseItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSBaseItemActor, bOnEquipped);
    DOREPLIFETIME(ACSBaseItemActor, bOnDropped);
}

