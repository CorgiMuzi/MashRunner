// Fill out your copyright notice in the Description page of Project Settings.


#include "MashRunnerGameModeBase.h"

void AMashRunnerGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetViewMode(EViewModeIndex::VMI_Unlit);
}

void AMashRunnerGameModeBase::SetViewMode(EViewModeIndex ViewMode)
{
	ApplyViewMode(ViewMode, false, GEngine->GameViewport->EngineShowFlags);
	GEngine->GameViewport->ViewModeIndex = ViewMode;
}

void AMashRunnerGameModeBase::AnnounceWinner(const AActor* const Winner)
{
	bIsGameFinished = true;
	
	if (GEngine)
	{
		FString WinnerAnnouncement = FString::Printf(TEXT("Winner is %s"), *Winner->GetName());
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Yellow, *WinnerAnnouncement);
	}

	OnWinnerAnnounced.Broadcast();
}
