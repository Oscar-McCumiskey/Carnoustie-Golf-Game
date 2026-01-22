// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GolfBallPawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UArrowComponent;
struct FInputActionValue;

UCLASS()
class CARNOUSTIEGOLFGAME_API AGolfBallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGolfBallPawn();

protected:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> ArrowComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when mouse clicked
	virtual void StartMouseRotating(const FInputActionValue& Value);

	// Called when mouse unclicked
	virtual void StopMouseRotating(const FInputActionValue& Value);

	// Mouse camera control logic
	virtual void Look(const FInputActionValue& Value);
	
	// Called when touch registered
	virtual void StartTouch(const FInputActionValue& Value);

	// Called when no touch registered
	virtual void StopTouch(const FInputActionValue& Value);
	
	// Touch camera control logic
	virtual void TouchLook(const FInputActionValue& Value);

public:
	// Rotate camera
	virtual void DoLook(float Yaw, float Pitch);

	// Shoot golf ball
	UFUNCTION(BlueprintCallable)
	virtual void DoShot(FVector Direction, float ShotPower);

	UFUNCTION(BlueprintCallable)
	virtual void UpdateShotPowerIndicator(float ShotPower);

	UFUNCTION(BlueprintCallable)
	virtual void UpdateShotTrajectoryIndicator(FRotator ShotDirection);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseToggleLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float RotateRate = 1.f;

	bool bCanRotate = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GolfShot")
	float MinimumShotPower= 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GolfShot")
	float MaximumShotPower = 500.f;

private:
	FVector2D PrevTouchVector;
	FVector2D StartTouchVector;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
