// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarpenterCharacter.h"

#include "CarpenterProjectile.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Components/CSAttributeComponent.h"
#include "Components/CSInteractionComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ACarpenterCharacter

ACarpenterCharacter::ACarpenterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());

	//Interact implement
	CarriedObjectComp = CreateDefaultSubobject<UCSInteractionComponent>(TEXT("CarriedObjectComp"));
	CarriedObjectComp->SetupAttachment(GetRootComponent());

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(SpringArmComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create an Action Management Comp
	ActionComp = CreateDefaultSubobject<UCSAcitonComponent>("ActionComp");

	MaxUseDistance = 500;
	//DropWeaponMaxDistance = 100;
	bHasNewFocus = true;

}

//////////////////////////////////////////////////////////////////////////
// Input

void ACarpenterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACarpenterCharacter::Use);

	//Drop Item
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &ACarpenterCharacter::DropCurrentItem);


	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);
	

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ACarpenterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACarpenterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACarpenterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACarpenterCharacter::LookUpAtRate);
}


void ACarpenterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Controller && Controller->IsLocalController())
	{
		ACSBaseInteractiableActor* Usable = GetUsableInView();

		// End Focus
		if (FocusedUsableActor != Usable)
		{
			if (FocusedUsableActor)
			{
				FocusedUsableActor->OnEndFocus();
			}

			bHasNewFocus = true;
		}

		// Assign new Focus
		FocusedUsableActor = Usable;

		// Start Focus.
		if (Usable)
		{
			if (bHasNewFocus)
			{
				Usable->OnBeginFocus();
				FocusedUsableActor = Cast<ACSBaseItemActor>(Usable);
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "Find Focus Actor to interact.");
				bHasNewFocus = false;
				if (FocusedUsableActor)
				{
					FocusedUsableActor->SetOwner(this);
				}
				
			}
		}
	}
}





//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ACarpenterCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ACarpenterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACarpenterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACarpenterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACarpenterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACarpenterCharacter::Interact()
{
	if (InteractionComponent)
	{
		//InteractionComponent->PrimaryInteract();
	}
}

void ACarpenterCharacter::ServerEquipButtonPressed_Implementation()
{
}

void ACarpenterCharacter::DropItem()
{
	if (InteractionComponent)
	{
		//InteractionComponent->DropItem();
	}
}

void ACarpenterCharacter::PaintItem()
{
	if (InteractionComponent)
	{
		//InteractionComponent->PaintItem();
	}
}

void ACarpenterCharacter::OnToggleCarryActor()
{
	CarriedObjectComp->Pickup();
}

void ACarpenterCharacter::OnRep_CurrentItem(ACSBaseItemActor* NewItem)
{
	CurrentItem = NewItem;
}

void ACarpenterCharacter::Use()
{
	if (HasAuthority())
	{
		ACSBaseInteractiableActor* Usable = GetUsableInView();
		if (Usable)
		{
			Usable->OnUsed(this);
		}
	}
	else
	{
		ServerUse();
	}
}

void ACarpenterCharacter::ServerUse_Implementation()
{
	Use();
}

bool ACarpenterCharacter::ServerUse_Validate()
{
	return true;
}

ACSBaseInteractiableActor* ACarpenterCharacter::GetUsableInView() const
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == nullptr)
		return nullptr;

	Controller->GetPlayerViewPoint(CamLoc, CamRot);

	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape Shape;
	Shape.SetSphere(15.0f);
	
	FHitResult Hit(ForceInit);
	
	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(
		Hit,TraceStart,TraceEnd,FQuat::Identity,ObjectQueryParams, Shape);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f, 0, 1.0f);

	if (bBlockingHit)
	{
		if (Hit.GetActor())
		{
		
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 50.0f, 12, FColor::Red, false, 0.0f);
			
		}
		
	}
	 
	return Cast<ACSBaseInteractiableActor>(Hit.GetActor());
}

void ACarpenterCharacter::DropCurrentItem()
{
	if (!HasAuthority())
	{
		ServerDropCurrentItem();
		return;
	}

	if (CurrentItem)
	{
		CurrentItem->OnRespawned();
	}
}


void ACarpenterCharacter::ServerDropCurrentItem_Implementation()
{
	DropCurrentItem();
}


bool ACarpenterCharacter::ServerDropCurrentItem_Validate()
{
	return true;
}


bool ACarpenterCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		
		return true;
	}
	
	return false;
}

void ACarpenterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACarpenterCharacter, CurrentItem);

}


