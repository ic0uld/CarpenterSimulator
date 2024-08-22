// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CSBaseInteractiableActor.h"

// Sets default values
ACSBaseInteractiableActor::ACSBaseInteractiableActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;

}

void ACSBaseInteractiableActor::OnBeginFocus()
{
	MeshComp->SetRenderCustomDepth(true);
}

void ACSBaseInteractiableActor::OnEndFocus()
{
	MeshComp->SetRenderCustomDepth(false);
}

void ACSBaseInteractiableActor::OnUsed(APawn* InstigatorPawn)
{
	//
}
