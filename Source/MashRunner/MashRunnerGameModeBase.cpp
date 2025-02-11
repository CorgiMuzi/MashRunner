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
