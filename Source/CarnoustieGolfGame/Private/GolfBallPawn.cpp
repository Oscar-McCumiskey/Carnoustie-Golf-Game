// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfBallPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/ArrowComponent.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

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
	CanRotate = true;
}

void AGolfBallPawn::StopMouseRotating(const FInputActionValue& Value)
{
	CanRotate = false;
}

void AGolfBallPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (CanRotate)
	{
		DoLook(LookAxisVector.X, LookAxisVector.Y);
	}
}

void AGolfBallPawn::StartTouch(const FInputActionValue& Value)
{
	PrevTouchVector = StartTouchVector = Value.Get<FVector2D>();
}

void AGolfBallPawn::TouchLook(const FInputActionValue& Value)
{
	FVector2D CurrentTouchVector = Value.Get<FVector2D>();
	FVector2D LookAxisVector = CurrentTouchVector - PrevTouchVector;
	PrevTouchVector = CurrentTouchVector;

	DoLook(LookAxisVector.X, LookAxisVector.Y);


	float ShotPower = CurrentTouchVector.Y - StartTouchVector.Y;
	if (ShotPower > 50)
	{
		ArrowComponent->SetArrowLength(FMath::Clamp(ShotPower, 0, 500));
	}
	else
	{
		ArrowComponent->SetArrowLength(10.f);
	}

}

void AGolfBallPawn::TouchShotPower(const FInputActionValue& Value)
{
	FVector2D ShotVector = Value.Get<FVector2D>();
	float ShotPower = ShotVector.Y - StartTouchVector.Y;
	ArrowComponent->SetArrowLength(10.f);

	DoShot(ShotPower);
}

void AGolfBallPawn::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw * RotateRate / 100.f);
	}
}

//void AGolfBallPawn::DoShot(FVector2D Direction, float ShotPower)
//{
//	
//}

// Called every frame
void AGolfBallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ArrowComponent->SetWorldRotation(GetController()->GetDesiredRotation());
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
		EnhancedInputComponent->BindAction(DragClickAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::StartMouseRotating);
		EnhancedInputComponent->BindAction(DragClickAction, ETriggerEvent::Completed, this, &AGolfBallPawn::StopMouseRotating);

		EnhancedInputComponent->BindAction(TouchLookAction, ETriggerEvent::Started, this, &AGolfBallPawn::StartTouch);
		EnhancedInputComponent->BindAction(TouchLookAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::TouchLook);
		EnhancedInputComponent->BindAction(TouchLookAction, ETriggerEvent::Completed, this, &AGolfBallPawn::TouchShotPower);

		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGolfBallPawn::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("Input Failed"), *GetNameSafe(this));
	}
}

