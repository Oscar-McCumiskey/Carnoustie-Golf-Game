#include "Player/GolfPawn.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "InputActionValue.h"

// Sets default values
AGolfPawn::AGolfPawn()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	GolfMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Golfball"));
	GolfMesh->SetupAttachment(RootComponent);

	GolfMesh->SetSimulatePhysics(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GolfMesh);
	SpringArm->TargetArmLength = 128;
	SpringArm->bDoCollisionTest = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	TurnSensitivity = 1.0f;
	ZoomSensitivity = 25.0f;

	SwingImpulse = 100.0f;
}

// Called when the game starts or when spawned
void AGolfPawn::BeginPlay()
{
	Super::BeginPlay();

	CurZoom = MinZoom;
	OrgLength = SpringArm->TargetArmLength;

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void AGolfPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MouseLeftAction, ETriggerEvent::Started, this, &AGolfPawn::MouseLeftPressed);
		Input->BindAction(MouseRightAction, ETriggerEvent::Started, this, &AGolfPawn::MouseRightPressed);

		Input->BindAction(MouseLeftAction, ETriggerEvent::Completed, this, &AGolfPawn::MouseLeftReleased);
		Input->BindAction(MouseRightAction, ETriggerEvent::Completed, this, &AGolfPawn::MouseRightReleased);

		Input->BindAction(MouseAxisAction, ETriggerEvent::Triggered, this, &AGolfPawn::MouseMove);

		Input->BindAction(MouseScrollAction, ETriggerEvent::Triggered, this, &AGolfPawn::MouseScroll);
	}
}

void AGolfPawn::MouseLeftPressed()
{
	bIsLeftMouseDown = true;
	OnMouseLeft();
}

void AGolfPawn::MouseRightPressed()
{
	bIsRightMouseDown = true;
	OnMouseRight();
}

void AGolfPawn::MouseLeftReleased()
{
	bIsLeftMouseDown = false;

	if (SwingPower > MinSwingPower && GolfMesh && Camera)
	{
		FVector AimVec = Camera->GetForwardVector();
		AimVec.Z = 0.0f;

		//Fallback
		if (AimVec.IsNearlyZero()) { AimVec = FVector::ForwardVector; }
		AimVec.Normalize();

		AimVec = AimVec.GetSafeNormal();
		if (!AimVec.IsNearlyZero())
		{
			const FVector Impulse = AimVec * SwingPower * SwingImpulse;
			GolfMesh->AddImpulse(Impulse, NAME_None, true);
		}

		SwingPower = 0.0f;
	}

	OnMouseLeftRelease();
}

void AGolfPawn::MouseRightReleased()
{
	bIsRightMouseDown = false;
	OnMouseRightRelease();
}

void AGolfPawn::MouseMove(const FInputActionValue& Value)
{
	//Neither left or right mouse is held down
	if (!bIsLeftMouseDown && !bIsRightMouseDown)
	{
		SwingPower = 0.0f;
		return;
	}

	const FVector2D Vector = Value.Get<FVector2D>();
	
	if(bIsLeftMouseDown)
	{
		SwingPower += -Vector.Y;
		SwingPower = FMath::Clamp(SwingPower, 0.0f, MaxSwingPower);

		UE_LOG(LogTemp, Warning, TEXT("Swing Power: %f"), SwingPower);
	}

	if (bIsRightMouseDown)
	{
		float Rotation = Vector.X * TurnSensitivity;
		SpringArm->AddRelativeRotation(FRotator(0.0f, Rotation, 0.0f));
	}
}

void AGolfPawn::MouseScroll(const FInputActionValue& Value)
{
	float ZoomChange = Value.Get<float>() / ZoomSensitivity;
	
	CurZoom += ZoomChange;
	CurZoom = FMath::Clamp(CurZoom, MinZoom, MaxZoom);

	SpringArm->TargetArmLength = OrgLength * CurZoom;
}
