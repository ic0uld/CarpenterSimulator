// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CSBaseInteractiableActor.h"

// Sets default values
ACSBaseInteractiableActor::ACSBaseInteractiableActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

}

void ACSBaseInteractiableActor::Interact_Implementation(APawn* InstigatorPawn)
{
	ICSGameplayInterface::Interact_Implementation(InstigatorPawn);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Interacted"), false);
	}
	
}

