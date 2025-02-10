// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Runner.generated.h"

/**
 * 
 */
UCLASS()
class ARunner : public APaperCharacter
{
	GENERATED_BODY()
	// ============================
	// AActor
	// ============================
public:
	virtual void BeginPlay() override;

	// ============================
	// ARunner
	// ============================
public:
	ARunner();

};
