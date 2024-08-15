// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARPENTER_API UCSInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void PrimaryInteract();

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindCloseActor();
	

	UPROPERTY()
	AActor* OnFocuActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UCSInteractionComponent();
	
};