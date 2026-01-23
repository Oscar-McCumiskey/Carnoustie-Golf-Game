// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GolfGamePlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class CARNOUSTIEGOLFGAME_API AGolfGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;

	virtual void SetupInputComponent() override;
};
