// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CinematicCamera.generated.h"

UCLASS()
class ACinematicCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACinematicCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<AActor*> TargetNodes;

	UPROPERTY(EditInstanceOnly)
	float SpeedMulti = 1.0f;

	//UPROPERTY(BlueprintReadWrite)
	//class UCameraComponent* Camera;

public:	

	UFUNCTION(BlueprintCallable)
	void StartCinematic();

	UFUNCTION(BlueprintCallable)
	void StopCinematic();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartCinematic();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStopCinematic();
};

