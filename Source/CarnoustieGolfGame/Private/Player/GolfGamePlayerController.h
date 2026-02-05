// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GolfGamePlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class CARNOUSTIEGOLFGAME_API AGolfGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGolfGamePlayerController();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Putting")
	float MinimumShotPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default|Putting")
	float MaximumShotPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Putting")
	float CurrentShotPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Putting")
	float CurrentShotAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Putting")
	float GolfClubShotPower = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Putting")
	float ShotChargeRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Putting")
	float ShotMultiplier = 1.f;

	virtual void SetupInputComponent() override;

public:

	UFUNCTION(BlueprintCallable)
	virtual void UpdateShotPower(float DeltaTime);

private:

	bool bReverseShotPower = false;

};
