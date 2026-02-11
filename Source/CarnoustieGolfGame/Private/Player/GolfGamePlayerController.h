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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DragScreenSpacePos(FVector2D ScreenSpace, AActor* Object);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DropPayload(AActor* ActorPayload);

	void DragScreenSpacePos_Implementation(FVector2D ScreenSpace, AActor* Object);
	void DropPayload_Implementation(AActor* ActorPayload);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Default|Putting")
	float MinimumShotChargeTime = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Default|Putting")
	float MaximumShotChargeTime = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Putting")
	float CurrentShotTime = 0.f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default|Putting")
	TObjectPtr<UCurveFloat> CurrentShotPowerCurve;

	virtual void SetupInputComponent() override;

public:

	UFUNCTION(BlueprintCallable)
	virtual void UpdateShotPower(float DeltaTime, UCurveFloat* Curve);

private:

	bool bReverseShotPower = false;
};
