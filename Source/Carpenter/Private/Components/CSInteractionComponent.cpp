// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CSInteractionComponent.h"
#include "Components/SceneComponent.h"

UCSInteractionComponent::UCSInteractionComponent()
{
	MaxPickupDistance = 600;
	RotateSpeed = 10.0f;

	SetIsReplicatedByDefault(true);
}

AActor* UCSInteractionComponent::GetActorInView()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	AController* Controller = PawnOwner->Controller;
	if (Controller == nullptr)
	{
		return nullptr;
	}

	FVector CamLoc;
	FRotator CamRot;
	Controller->GetPlayerViewPoint(CamLoc, CamRot);

	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxPickupDistance);

	FCollisionQueryParams TraceParams(TEXT("TraceActor"), true, PawnOwner);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = false;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	/* Check to see if we hit a staticmesh component that has physics simulation enabled */
	UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Hit.GetComponent());
	if (MeshComp && MeshComp->IsSimulatingPhysics())
	{
		return Hit.GetActor();
	}

	return nullptr;
}

void UCSInteractionComponent::OnDropMulticast_Implementation()
{
	AActor* CarriedActor = GetCarriedActor();
	if (CarriedActor)
	{
		/* Find the static mesh (if any) to re-enable physics simulation */
		UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(CarriedActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (MeshComp)
		{
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MeshComp->SetSimulatePhysics(true);
		}

		CarriedActor->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void UCSInteractionComponent::Throw()
{
	if (!GetIsCarryingActor())
		return;

	if (!GetOwner()->HasAuthority())
	{
		ServerThrow();
		return;
	}

	/* Grab a reference to the MeshComp before dropping the object */
	UStaticMeshComponent* MeshComp = GetCarriedMeshComp();
	if (MeshComp)
	{
		/* Detach and re-enable collision */
		OnDropMulticast();

		APawn* OwningPawn = Cast<APawn>(GetOwner());
		if (OwningPawn)
		{
			/* Re-map uint8 to 360 degrees */
			const float PawnViewPitch = (OwningPawn->RemoteViewPitch / 255.f)*360.f;

			FRotator NewRotation = GetComponentRotation();
			NewRotation.Pitch = PawnViewPitch;

			/* Apply physics impulse, ignores mass */
			MeshComp->AddImpulse(NewRotation.Vector() * 1000, NAME_None, true);
		}
	}
}

bool UCSInteractionComponent::GetIsCarryingActor()
{
	return GetChildComponent(0) != nullptr;
}

void UCSInteractionComponent::Rotate(float DirectionYaw, float DirectionRoll)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerRotate(DirectionYaw, DirectionRoll);
	}

	OnRotateMulticast(DirectionYaw, DirectionRoll);
}

AActor* UCSInteractionComponent::GetCarriedActor()
{
	USceneComponent* ChildComp = GetChildComponent(0);
	if (ChildComp)
	{
		return ChildComp->GetOwner();
	}

	return nullptr;
}

UStaticMeshComponent* UCSInteractionComponent::GetCarriedMeshComp()
{
	USceneComponent* ChildComp = GetChildComponent(0);
	if (ChildComp)
	{
		AActor* OwningActor = ChildComp->GetOwner();
		if (OwningActor)
		{
			return Cast<UStaticMeshComponent>(OwningActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		}
	}

	return nullptr;
}

void UCSInteractionComponent::TickComponent(float DeltaSeconds, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if (APawn* OwningPawn = Cast<APawn>(GetOwner()))
	{
		if (OwningPawn->IsLocallyControlled())
		{
			Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);
		}
		else
		{
			if (bUsePawnControlRotation)
			{
				{
					const float PawnViewPitch = (OwningPawn->RemoteViewPitch / 255.f)*360.f;
					if (PawnViewPitch != GetComponentRotation().Pitch)
					{
						FRotator NewRotation = GetComponentRotation();
						NewRotation.Pitch = PawnViewPitch;
						SetWorldRotation(NewRotation);
					}
				}
			}

			UpdateDesiredArmLocation(bDoCollisionTest, bEnableCameraLag, bEnableCameraRotationLag, DeltaSeconds);
		}
	}
}

void UCSInteractionComponent::RotateActorAroundPoint(AActor* RotateActor, FVector RotationPoint, FRotator AddRotation)
{
	FVector Loc = RotateActor->GetActorLocation() - RotationPoint;
	FVector RotatedLoc = AddRotation.RotateVector(Loc);

	FVector NewLoc = RotationPoint + RotatedLoc;
	
	/* Compose the rotators, use Quats to avoid gimbal lock */
	FQuat AQuat = FQuat(RotateActor->GetActorRotation());
	FQuat BQuat = FQuat(AddRotation);

	FRotator NewRot = FRotator(BQuat*AQuat);

	RotateActor->SetActorLocationAndRotation(NewLoc, NewRot);
}

void UCSInteractionComponent::Pickup()
{
	/* Drop if we are already carrying an Actor */
	if (GetIsCarryingActor())
	{
		Drop();
		return;
	} 

	if (!GetOwner()->HasAuthority())
	{
		ServerPickup();
		return;
	}

	AActor* FocusActor = GetActorInView();
	OnPickupMulticast(FocusActor);
}

void UCSInteractionComponent::OnPickupMulticast_Implementation(AActor* FocusActor)
{
	if (FocusActor && FocusActor->IsRootComponentMovable())
	{
		/* Find the static mesh (if any) to disable physics simulation while carried
		Filter by objects that are physically simulated and can therefor be picked up */
		UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(FocusActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (MeshComp && MeshComp->IsSimulatingPhysics())
		{
			MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			MeshComp->SetSimulatePhysics(false);
		}

		FocusActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void UCSInteractionComponent::Drop()
{
	if (!GetOwner()->HasAuthority())
	{
		ServerDrop();
	}

	OnDropMulticast();
}

void UCSInteractionComponent::ServerDrop_Implementation()
{
	Drop();
}


bool UCSInteractionComponent::ServerDrop_Validate()
{
	return true;
}


void UCSInteractionComponent::ServerThrow_Implementation()
{
	Throw();
}


bool UCSInteractionComponent::ServerThrow_Validate()
{
	return true;
}


void UCSInteractionComponent::ServerPickup_Implementation()
{
	Pickup();
}


bool UCSInteractionComponent::ServerPickup_Validate()
{
	return true;
}


void UCSInteractionComponent::ServerRotate_Implementation(float DirectionYaw, float DirectionRoll)
{
	Rotate(DirectionYaw, DirectionRoll);
}


bool UCSInteractionComponent::ServerRotate_Validate(float DirectionYaw, float DirectionRoll)
{
	return true;
}

void UCSInteractionComponent::OnRotateMulticast_Implementation(float DirectionYaw, float DirectionRoll)
{
	AActor* CarriedActor = GetCarriedActor();
	if (CarriedActor)
	{
		/* Retrieve the object center */
		FVector RootOrigin = GetCarriedActor()->GetRootComponent()->Bounds.Origin;
		FRotator DeltaRot = FRotator(0, DirectionYaw * RotateSpeed, DirectionRoll * RotateSpeed);

		RotateActorAroundPoint(CarriedActor, RootOrigin, DeltaRot);
	}
}
