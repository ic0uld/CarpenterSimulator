// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CarpenterHUD.generated.h"

UCLASS()
class ACarpenterHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACarpenterHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

