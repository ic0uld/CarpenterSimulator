#include "Actors/CSBaseItemActor.h"
#include "Carpenter/CarpenterCharacter.h"
#include "Components/CSInteractionComponent.h"
#include "Net/UnrealNetwork.h"

ACSBaseItemActor::ACSBaseItemActor()
{
    bReplicates = true;
}

void ACSBaseItemActor::Interact_Implementation(APawn* InstigatorPawn)
{
    if (HasAuthority())
    {
        OnEquipped(InstigatorPawn);  // Server handles the equipping logic
    }
    else
    {
        ServerInteract(InstigatorPawn);  // Clients request the server to equip the item
    }
}

void ACSBaseItemActor::ServerInteract_Implementation(APawn* InstigatorPawn)
{
    OnEquipped(InstigatorPawn);  // Perform server-side logic for equipping the item
}

void ACSBaseItemActor::OnEquipped(APawn* InstigatorPawn)
{
    ACarpenterCharacter* PlayerCharacter = Cast<ACarpenterCharacter>(InstigatorPawn);
    if (PlayerCharacter)
    {
        UCSInteractionComponent* InteractionComp = PlayerCharacter->FindComponentByClass<UCSInteractionComponent>();

        if (InteractionComp && !InteractionComp->EquippedItem)
        {
            BaseMesh->SetSimulatePhysics(false);
            BaseMesh->SetEnableGravity(false);
            BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
            BaseMesh->AttachToComponent(PlayerCharacter->GetMesh(), AttachRules, TEXT("HandSocketName"));

            // Additional logic to handle attaching to first-person mesh
            if (USkeletalMeshComponent* SkeletalMesh = PlayerCharacter->Mesh1P)
            {
                this->AttachToComponent(SkeletalMesh, AttachRules, TEXT("ItemSnapLocation"));
            }
        }
    }
}

void ACSBaseItemActor::OnDropped()
{
    BaseMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    BaseMesh->SetSimulatePhysics(true);
    BaseMesh->SetEnableGravity(true);
    BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACSBaseItemActor::OnRep_ItemName() { }

void ACSBaseItemActor::OnRep_ItemMaterial() { }

void ACSBaseItemActor::OnRep_ItemMesh() { }

void ACSBaseItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACSBaseItemActor, ItemName);
    DOREPLIFETIME(ACSBaseItemActor, ItemMaterial);
    DOREPLIFETIME(ACSBaseItemActor, ChangedMesh);
}
