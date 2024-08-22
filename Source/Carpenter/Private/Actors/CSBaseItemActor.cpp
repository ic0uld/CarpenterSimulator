#include "Actors/CSBaseItemActor.h"
#include "Carpenter/CarpenterCharacter.h"
#include "Components/CSInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

ACSBaseItemActor::ACSBaseItemActor()
{
	/* Ignore Pawn - this is to prevent objects shooting through the level or pawns glitching on top of small items. */
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

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

	//if (bStartActive)
	{
		RespawnPickup();
	}
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

void ACSBaseItemActor::RespawnPickup()
{
	bIsActive = true;
	OnRespawned();
}

void ACSBaseItemActor::OnPickedUp()
{
	if (MeshComp)
	{
		MeshComp->SetVisibility(false);
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACSBaseItemActor::OnRespawned()
{
	if (MeshComp)
	{
		MeshComp->SetVisibility(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACSBaseItemActor::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	bIsActive = false;
	OnPickedUp();

	if (bAllowRespawn)
	{
		FTimerHandle RespawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ACSBaseItemActor::RespawnPickup, RespawnDelay + FMath::RandHelper(RespawnDelayRange), false);
	}
	else
	{
		Destroy();
	}
}

void ACSBaseItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACSBaseItemActor, bIsActive);
}