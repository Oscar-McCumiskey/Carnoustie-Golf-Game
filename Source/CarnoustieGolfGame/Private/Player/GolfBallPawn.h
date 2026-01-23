// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GolfBallPawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;
struct FInputActionValue;

UCLASS()
class CARNOUSTIEGOLFGAME_API AGolfBallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGolfBallPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInputsLocked;

protected:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bUseTouchControls;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* TouchInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* MouseInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseAxisAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseScrollAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	FRotator CameraRotationOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	float TurnSensitivity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	float ZoomSensitivity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	float MinimumZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	float MaximumZoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Putting")
	float MinimumShotPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Putting")
	float MaximumShotPower;

private:
	FVector2D PrevTouchVector;
	FVector2D StartTouchVector;

	bool bIsRightMouseDown;
	bool bIsLeftMouseDown;
	float CurZoom;
	float OrgLength;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Touch Input

	// Called when touch registered
	virtual void StartTouch(const FInputActionValue& Value);

	// Called when no touch registered
	virtual void StopTouch(const FInputActionValue& Value);

	// Touch camera control logic
	virtual void TouchLook(const FInputActionValue& Value);

#pragma endregion

#pragma region Mouse Input

	virtual void MouseLeftPressed();

	virtual void MouseRightPressed();

	virtual void MouseLeftReleased();

	virtual void MouseRightReleased();

	virtual void MouseMove(const FInputActionValue& Value);

	virtual void MouseScroll(const FInputActionValue& Value);

#pragma endregion

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Rotate camera
	virtual void DoLook(float Yaw, float Pitch);

	// Shoot golf ball
	UFUNCTION(BlueprintCallable)
	virtual void DoShot(FVector Direction, float ShotPower);

	UFUNCTION(BlueprintCallable)
	virtual void UpdateShotPowerIndicator(float ShotPower);

	UFUNCTION(BlueprintCallable)
	virtual void UpdateShotTrajectoryIndicator(FRotator ShotDirection);

};
