#include "Actors/CSBaseItemActor.h"

#include "IDetailTreeNode.h"
#include "Carpenter/CarpenterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

ACSBaseItemActor::ACSBaseItemActor()
{

	bIsActive = false;
	bStartActive = true;
	bAllowRespawn = true;
	RespawnDelay = 5.0f;
	RespawnDelayRange = 5.0f;

	SetReplicates(true);
}

void ACSBaseItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACSBaseItemActor::OnRep_IsActive()
{
	if (bIsActive)
	{
		OnRespawned();
	}
	else
	{
		OnPickedUp();
	}
}

void ACSBaseItemActor::OnRep_ItemType(EItemType NewType)
{
	ItemType = NewType;
}


void ACSBaseItemActor::OnRep_MyPawn()
{
	//
}

void ACSBaseItemActor::RespawnPickup()
{
	bIsActive = true;
	OnRespawned();
}

void ACSBaseItemActor::OnPickedUp_Implementation()
{
	if (MeshComp)
	{
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetEnableGravity(false);
		SetCollision("NoCollision");
		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh1P();
		MeshComp->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MyPawn->GetMesh1P()->GetSocketBoneName("AttachLocation"));
		MyPawn->CurrentItem = this;
	
	}
}


void ACSBaseItemActor::OnRespawned()
{
	if (MeshComp)
	{
		MeshComp->SetVisibility(true);
		SetCollision("BlockAllDynamic");
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetEnableGravity(true);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		MyPawn->CurrentItem = nullptr;
	}
}

void ACSBaseItemActor::ChangeMesh_Implementation(UStaticMesh* NewMesh, EItemType NewType)
{
	if (MeshComp)
	{
		OnRep_ItemType(NewType);
		MeshComp->SetStaticMesh(NewMesh);
	}

}

void ACSBaseItemActor::SetCollision_Implementation(FName NewCollisionProfile)
{
	if (MeshComp)
	{
		MeshComp->SetCollisionProfileName(NewCollisionProfile);
	}
}

void ACSBaseItemActor::ChangeMaterial_Implementation(UMaterialInterface* NewMaterial, FName ColorName)
{
	if (MeshComp)
	{
		MeshComp->SetMaterial(0, NewMaterial);
		OnRep_PaintName(ColorName);
	}
}

void ACSBaseItemActor::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	MyPawn = Cast<ACarpenterCharacter>(InstigatorPawn);

	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	bIsActive = false;
	OnPickedUp();
	
}

void ACSBaseItemActor::OnRep_PaintName(FName NewColor)
{
	PaintName = NewColor;
}

void ACSBaseItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSBaseItemActor, MyPawn);
	DOREPLIFETIME(ACSBaseItemActor, bIsActive);
}
