// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TPSHOOTER_API ATPShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void PawnKilled(APawn* PawnKilled);
};
