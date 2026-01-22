
#include "Actor/CinematicCamera.h"
#include "CinematicCamera.h"

#include "Camera/CameraComponent.h"
#include "Curves/CurveFloat.h" 
#include "Components/TimelineComponent.h"

// Sets default values
ACinematicCamera::ACinematicCamera()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpeedMulti = 1.0f;
}

// Called when the game starts or when spawned
void ACinematicCamera::BeginPlay()
{
	Super::BeginPlay();
	
	//FOnTimelineFloat TimelineProgressFunc;

	//FOnTimelineEvent TimelineFinish;

	/*UCurveFloat* Curve = NewObject<UCurveFloat>(this);

	Curve->FloatCurve.UpdateOrAddKey(0.0f, 0.0f);
	Curve->FloatCurve.UpdateOrAddKey(1.0f, 1.0f);*/

	//TimelineProgressFunc.BindUFunction(this, TEXT("OnTimelineProgress"));
	//TimelineFinish.BindUFunction(this, TEXT("StopCinematic"));

	//Timeline->SetTimelineFinishedFunc(TimelineFinish);

	//Timeline->AddInterpFloat(Curve, TimelineProgressFunc);
	//Timeline->SetTimelineLengthMode(TL_LastKeyFrame);

	//Timeline->SetPlayRate(SpeedMulti);

	//StartCinematic();
}

void ACinematicCamera::StartCinematic()
{
	SetActorLocation(TargetNodes[0]->GetActorLocation());

	//Timeline->PlayFromStart();

	OnStartCinematic();
}

void ACinematicCamera::StopCinematic()
{
	//Timeline->Stop();

	OnStopCinematic();
}
