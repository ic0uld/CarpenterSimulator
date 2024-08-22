// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/CSAcitonComponent.h"
#include "GameFramework/Character.h"
#include "CarpenterCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class USpringArmComponent;
class UCameraComponent;
class UCSInteractionComponent;
class UCSAttributeComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ACarpenterCharacter : public ACharacter
{
	GENERATED_BODY()

///////////////////////Components///////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCSInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components" , meta = (AllowPrivateAccess = "true"))
	UCSAcitonComponent* ActionComp;





public:
	ACarpenterCharacter();

	///////////////////////Components///////////////////////

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
	UCSAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCSInteractionComponent* CarriedObjectComp;

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(BlueprintCallable)
	void PaintItem();

	/* Input mapped function for carry object component */
	void OnToggleCarryActor();


	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentItem)
	class ACSBaseItemActor* CurrentItem;

	UFUNCTION()
	void OnRep_CurrentItem(ACSBaseItemActor* NewItem);
	
	/* Use the usable actor currently in focus, if any */
	virtual void Use();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUse();

	void ServerUse_Implementation();

	bool ServerUse_Validate();
	
	void DropCurrentItem();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerDropCurrentItem();

	void ServerDropCurrentItem_Implementation();

	bool ServerDropCurrentItem_Validate();
	
	class ACSBaseInteractiableActor* GetUsableInView() const;

	/*Max distance to use/focus on actors. */
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	float MaxUseDistance;

	/* True only in first frame when focused on a new usable actor. */
	bool bHasNewFocus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
	class ACSBaseInteractiableActor* FocusedUsableActor;



protected:
	
	virtual void Tick(float DeltaSeconds) override;
	
	void MoveForward(float Val);
	
	void MoveRight(float Val);
	
	void TurnAtRate(float Rate);
	
	void LookUpAtRate(float Rate);

	

	
protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

private:
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	

};

