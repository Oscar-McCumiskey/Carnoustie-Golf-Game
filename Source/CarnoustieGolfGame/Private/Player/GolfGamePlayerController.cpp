// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

AGolfGamePlayerController::AGolfGamePlayerController()
{
	CurrentShotTime = 0.f;
}

void AGolfGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->AddMappingContext(DefaultMappingContext.LoadSynchronous(), 0);
	}

}

void AGolfGamePlayerController::DragScreenSpacePos_Implementation(FVector2D ScreenSpace, AActor* Object) {}
void AGolfGamePlayerController::DropPayload_Implementation(AActor* ActorPayload) {}
void AGolfGamePlayerController::UpdateShotPower(float DeltaTime, UCurveFloat* Curve)
{
	// Charge power up or down
	if (!bReverseShotPower)
	{
		CurrentShotTime += (DeltaTime * ShotChargeRate);
		CurrentShotPower = Curve->GetFloatValue(CurrentShotTime);
	}
	else
	{
		CurrentShotTime -= (DeltaTime * ShotChargeRate);
		CurrentShotPower = Curve->GetFloatValue(CurrentShotTime);
	}

	// Check to reverse charge
	if (CurrentShotTime >= MaximumShotChargeTime)
	{
		bReverseShotPower = true;
	}
	else if (CurrentShotTime <= MinimumShotChargeTime)
	{
		bReverseShotPower = false;
	}
}
