#pragma once

#include "CoreMinimal.h"
#include "CSBaseItemActor.h"
#include "Actors/CSBaseInteractiableActor.h"
#include "Carpenter/Carpenter.h"
#include "Components/CSAcitonComponent.h"
#include "CSComputer.generated.h"

class UWidgetComponent;
class ATargetPoint;

UCLASS()
class CARPENTER_API ACSComputer : public ACSBaseInteractiableActor
{
	GENERATED_BODY()
    
public:
	ACSComputer();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void ServerSetItemType(EItemType NewItemType);

	UFUNCTION(BlueprintCallable, Category= "Spawn")
	void OnItemTypeSelected(EItemType SelectedItemType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ItemType)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATargetPoint* TargetPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* ChairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* CabinetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* TableMesh;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* ScreenUI;

	UFUNCTION()
	void OnRep_ItemType();

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ACSBaseItemActor> ItemClass;
};
