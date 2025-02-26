// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MashRunnerGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBeginRace);
DECLARE_MULTICAST_DELEGATE(FOnWinnerAnnounced);

UCLASS()
class MASHRUNNER_API AMashRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	// ============================
	// AActor
	// ============================
public:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	
	UFUNCTION(BlueprintCallable)
	static void SetViewMode(EViewModeIndex ViewMode);
	
	// ============================
	// AMashRunnerGameMode
	// ============================
public:
	FOnBeginRace OnBeginRace;
	FOnWinnerAnnounced OnWinnerAnnounced;
	void AnnounceWinner(const AActor* const Winner);
	void RestartMashRunner();

	bool bIsGameFinished{false};
};
