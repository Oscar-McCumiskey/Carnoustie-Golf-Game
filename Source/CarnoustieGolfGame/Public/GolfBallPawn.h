// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Logging/LogMacros.h"
#include "GolfBallPawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UArrowComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

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

	virtual void StartMouseRotating(const FInputActionValue& Value);

	virtual void StopMouseRotating(const FInputActionValue& Value);

	virtual void Look(const FInputActionValue& Value);
	
	virtual void StartTouch(const FInputActionValue& Value);

	virtual void TouchLook(const FInputActionValue& Value);
	
	virtual void TouchShotPower(const FInputActionValue& Value);

public:
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DoShot(float ShotPower);


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DragClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float RotateRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool CanRotate = false;

private:
	FVector2D PrevTouchVector;
	FVector2D StartTouchVector;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
