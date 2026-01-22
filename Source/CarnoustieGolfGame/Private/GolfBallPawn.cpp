// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfBallPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ArrowComponent.h"
#include "GolfGamePlayerController.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

// Sets default values
AGolfBallPawn::AGolfBallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("Golf Ball"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->ArrowLength = 10.f;
	ArrowComponent->bHiddenInGame = false;
	ArrowComponent->SetUsingAbsoluteRotation(true);
}

// Called when the game starts or when spawned
void AGolfBallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGolfBallPawn::StartMouseRotating(const FInputActionValue& Value)
{
	bCanRotate = true;
}

void AGolfBallPawn::StopMouseRotating(const FInputActionValue& Value)
{
	bCanRotate = false;
}

void AGolfBallPawn::Look(const FInputActionValue& Value)
{
	// REMOVE TESTING ONLY
	// Ignore mouse input if touch controls active
	if (Cast<AGolfGamePlayerController>(GetController())->bUseTouchInput)
	{
		return;
	}

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (bCanRotate)
	{
		DoLook(LookAxisVector.X, LookAxisVector.Y);
	}
}

void AGolfBallPawn::StartTouch(const FInputActionValue& Value)
{
	PrevTouchVector = StartTouchVector = Value.Get<FVector2D>();
}

void AGolfBallPawn::StopTouch(const FInputActionValue& Value)
{
}

void AGolfBallPawn::TouchLook(const FInputActionValue& Value)
{
	FVector2D CurrentTouchVector = Value.Get<FVector2D>();
	FVector2D LookAxisVector = CurrentTouchVector - PrevTouchVector;
	PrevTouchVector = CurrentTouchVector;

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AGolfBallPawn::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw * RotateRate / 100.f);
		AddControllerPitchInput(Pitch * RotateRate / 100.f);
	}
}

void AGolfBallPawn::DoShot(FVector Direction, float ShotPower)
{
	if (ShotPower >= MinimumShotPower)
	{
		StaticMesh->AddImpulse(Direction * FMath::Clamp(ShotPower, MinimumShotPower, MaximumShotPower));
	}
}

void AGolfBallPawn::UpdateShotPowerIndicator(float ShotPower)
{
	ArrowComponent->SetArrowLength(FMath::Clamp(ShotPower, MinimumShotPower, MaximumShotPower));
}

void AGolfBallPawn::UpdateShotTrajectoryIndicator(FRotator ShotDirection)
{
	ArrowComponent->SetWorldRotation(ShotDirection);
}

// Called every frame
void AGolfBallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateShotTrajectoryIndicator(GetController()->GetDesiredRotation());
}

// Called to bind functionality to input
void AGolfBallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 1);
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Started, this, &AGolfBallPawn::StartTouch);
		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::TouchLook);
		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Completed, this, &AGolfBallPawn::StopTouch);

		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::Look);
		EnhancedInputComponent->BindAction(MouseToggleLookAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::StartMouseRotating);
		EnhancedInputComponent->BindAction(MouseToggleLookAction, ETriggerEvent::Completed, this, &AGolfBallPawn::StopMouseRotating);
	}
	else
	{
		
	}
}

