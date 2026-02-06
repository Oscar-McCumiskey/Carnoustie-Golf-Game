// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

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