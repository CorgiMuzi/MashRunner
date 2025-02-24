// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MashRunnerGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWinnerAnnounced);

UCLASS()
class MASHRUNNER_API AMashRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	static void SetViewMode(EViewModeIndex ViewMode);

	// ============================
	// AMashRunnerGameMode
	// ============================
public:
	FOnWinnerAnnounced OnWinnerAnnounced;
	void AnnounceWinner(const AActor* const Winner);

	bool bIsGameFinished{false};
};
