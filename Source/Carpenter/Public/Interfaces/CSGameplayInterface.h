
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CSGameplayInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCSGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class CARPENTER_API ICSGameplayInterface 
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DropItem(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PaintItem(APawn* InstigatorPawn);

	
	
};
