#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GolfPawn.generated.h"

class UInputAction;

UCLASS()
class AGolfPawn : public APawn
{
	GENERATED_BODY()

public:
	AGolfPawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GolfMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLeftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseAxisAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseScrollAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	float TurnSensitivity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	float ZoomSensitivity;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	float MinZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Camera")
	float MaxZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Putting")
	float MinSwingPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Putting")
	float MaxSwingPower;

	UPROPERTY(BlueprintReadOnly)
	float SwingPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input|Putting")
	float SwingImpulse;

private:
	UPROPERTY()
	bool bIsRightMouseDown;

	UPROPERTY()
	bool bIsLeftMouseDown;

	UPROPERTY()
	float CurZoom;

	UPROPERTY()
	float OrgLength;

	UPROPERTY()
	FRotator CurRotation;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInputsLocked;

	UFUNCTION()
	void MouseLeftPressed();

	UFUNCTION()
	void MouseRightPressed();

	UFUNCTION()
	void MouseLeftReleased();

	UFUNCTION()
	void MouseRightReleased();

	UFUNCTION()
	void MouseMove(const FInputActionValue& Value);

	UFUNCTION()
	void MouseScroll(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMouseLeft();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMouseRight();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMouseLeftRelease();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMouseRightRelease();
};
