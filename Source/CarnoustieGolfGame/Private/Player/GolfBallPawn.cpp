// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfBallPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GolfGamePlayerController.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

// Sets default values
AGolfBallPawn::AGolfBallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("GolfBall"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 128;
	SpringArm->bDoCollisionTest = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	TurnSensitivity = 1.0f;
	ZoomSensitivity = 25.0f;

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
	
	CurZoom = (MinimumZoom + MaximumZoom) / 2;
	OrgLength = SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = OrgLength * CurZoom;

	SpringArm->SetRelativeRotation(GetController()->GetDesiredRotation() + CameraRotationOffset);
}

void AGolfBallPawn::StartTouch(const FInputActionValue& Value)
{
	if (bInputsLocked) return;

	PrevTouchVector = StartTouchVector = Value.Get<FVector2D>();
}

void AGolfBallPawn::StopTouch(const FInputActionValue& Value)
{
	if (bInputsLocked) return;
}

void AGolfBallPawn::TouchLook(const FInputActionValue& Value)
{
	if (bInputsLocked) return;

	FVector2D CurrentTouchVector = Value.Get<FVector2D>();
	FVector2D LookAxisVector = CurrentTouchVector - PrevTouchVector;
	PrevTouchVector = CurrentTouchVector;

	DoLook(LookAxisVector.X / 10.f, LookAxisVector.Y / 10.f);
}

void AGolfBallPawn::MouseLeftPressed()
{
	if (bInputsLocked) return;

	bIsLeftMouseDown = true;
}

void AGolfBallPawn::MouseRightPressed()
{
	if (bInputsLocked) return;

	bIsRightMouseDown = true;
}

void AGolfBallPawn::MouseLeftReleased()
{
	if (bInputsLocked) return;

	bIsLeftMouseDown = false;
}

void AGolfBallPawn::MouseRightReleased()
{
	if (bInputsLocked) return;

	bIsRightMouseDown = false;
}

void AGolfBallPawn::MouseMove(const FInputActionValue& Value)
{
	if (bInputsLocked) return;

	//Neither left or right mouse is held down
	if (!bIsLeftMouseDown && !bIsRightMouseDown)
	{
		//SwingPower = 0.0f;
		return;
	}

	const FVector2D LookVector = Value.Get<FVector2D>();

	if (bIsLeftMouseDown)
	{

	}

	if (bIsRightMouseDown)
	{
		DoLook(LookVector.X, LookVector.Y);
	}
}

void AGolfBallPawn::MouseScroll(const FInputActionValue& Value)
{
	if (bInputsLocked) return;

	float ZoomChange = Value.Get<float>() / ZoomSensitivity;

	CurZoom += ZoomChange;
	CurZoom = FMath::Clamp(CurZoom, MinimumZoom, MaximumZoom);

	SpringArm->TargetArmLength = OrgLength * CurZoom;
}

void AGolfBallPawn::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw * TurnSensitivity);
		AddControllerPitchInput(Pitch * TurnSensitivity);

		SpringArm->SetRelativeRotation(GetController()->GetDesiredRotation() + CameraRotationOffset);
	}
}

void AGolfBallPawn::DoShot(FVector Direction, float ShotPower)
{
	if (ShotPower >= MinimumShotPower)
	{
		StaticMesh->AddImpulse(Direction * FMath::Clamp(ShotPower, MinimumShotPower, MaximumShotPower));
		OnStroke.Broadcast();
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
			if (bUseTouchControls)
			{
				Subsystem->AddMappingContext(TouchInputMappingContext, 1);
			}
			else
			{
				Subsystem->AddMappingContext(MouseInputMappingContext, 1);
			}
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Started, this, &AGolfBallPawn::StartTouch);
		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::TouchLook);
		EnhancedInputComponent->BindAction(TouchAction, ETriggerEvent::Completed, this, &AGolfBallPawn::StopTouch);

		EnhancedInputComponent->BindAction(MouseLeftAction, ETriggerEvent::Started, this, &AGolfBallPawn::MouseLeftPressed);
		EnhancedInputComponent->BindAction(MouseRightAction, ETriggerEvent::Started, this, &AGolfBallPawn::MouseRightPressed);

		EnhancedInputComponent->BindAction(MouseLeftAction, ETriggerEvent::Completed, this, &AGolfBallPawn::MouseLeftReleased);
		EnhancedInputComponent->BindAction(MouseRightAction, ETriggerEvent::Completed, this, &AGolfBallPawn::MouseRightReleased);

		EnhancedInputComponent->BindAction(MouseAxisAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::MouseMove);

		EnhancedInputComponent->BindAction(MouseScrollAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::MouseScroll);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to setup input bindings"));
	}
}

