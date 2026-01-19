// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfBallPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
AGolfBallPawn::AGolfBallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGolfBallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGolfBallPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AGolfBallPawn::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw * 1);
		AddControllerPitchInput(Pitch * 1);
	}
}

// Called every frame
void AGolfBallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGolfBallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::Look);
	}
}

