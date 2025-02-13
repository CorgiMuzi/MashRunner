// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TrackField.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

ATrackField::ATrackField()
{
	PrimaryActorTick.bCanEverTick = false;

	TrackGroup = CreateDefaultSubobject<USceneComponent>(FName("TrackGroup"));
	SetRootComponent(TrackGroup);
}

void ATrackField::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrackField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (TrackSprite == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No sprite image assigned"));
		return;
	} 

	for (int32 TrackIndex = 0; TrackIndex < TrackNum; ++TrackIndex)
	{
		FString TrackName = FString::Printf(TEXT("Track_%d"), TrackIndex);
		TObjectPtr<UPaperSpriteComponent> NewTrackComp = NewObject<UPaperSpriteComponent>(this, *TrackName);
		float TrackWidth = TrackSprite->GetSourceSize().X;
		float PPU = TrackSprite->GetPixelsPerUnrealUnit();
		FVector NewLocation(TrackIndex * (TrackWidth / PPU), 0.f, 0.f);
		NewTrackComp->SetRelativeLocation(NewLocation);
		NewTrackComp->SetSprite(TrackSprite);
		NewTrackComp->RegisterComponent();
		NewTrackComp->SetupAttachment(RootComponent);
	}
}
