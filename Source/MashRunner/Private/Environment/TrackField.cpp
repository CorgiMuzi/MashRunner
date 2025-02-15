// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TrackField.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Components/BoxComponent.h"

ATrackField::ATrackField()
{
	PrimaryActorTick.bCanEverTick = false;

	TrackGroup = CreateDefaultSubobject<USceneComponent>(FName("TrackGroup"));
	SetRootComponent(TrackGroup);
	TrackGroup->SetMobility(EComponentMobility::Static);

	GroundBoxTop = CreateDefaultSubobject<UBoxComponent>(FName("GroundBoxTop"));
	GroundBoxTop->SetupAttachment(RootComponent);
	SetupGroundBoxSettings(GroundBoxTop);
	GroundBoxBottom = CreateDefaultSubobject<UBoxComponent>(FName("GroundBoxBottom"));
	GroundBoxBottom->SetupAttachment(RootComponent);
	SetupGroundBoxSettings(GroundBoxBottom);
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
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, TEXT("No track sprite"));
		return;
	}
	
	float PPU = TrackSprite->GetPixelsPerUnrealUnit();
	float TrackWidth = TrackSprite->GetRenderBounds().GetBox().GetSize().X;
	for (int32 TrackIndex = 0; TrackIndex < TrackNum; ++TrackIndex)
	{
		FString TrackName = FString::Printf(TEXT("Track_%d"), TrackIndex);
		TObjectPtr<UPaperSpriteComponent> NewTrackComp = NewObject<UPaperSpriteComponent>(this, *TrackName);
		FAttachmentTransformRules TransformRules(EAttachmentRule::KeepWorld, false);
		NewTrackComp->AttachToComponent(TrackGroup, TransformRules);
		NewTrackComp->RegisterComponent();
		
		FVector NewLocation(TrackIndex * TrackWidth, 0.f, 0.f);
		NewTrackComp->SetRelativeLocation(NewLocation);
		NewTrackComp->SetSprite(TrackSprite);
		NewTrackComp->SetTranslucentSortPriority(-1.f);
	}

	if (GroundBoxTop)
	{
		
		GroundBoxTop->SetBoxExtent(FVector(TrackWidth * TrackNum / 2.f, GroundWidth, 1.f));
		FVector NewLocation(GroundBoxTop->GetScaledBoxExtent().X - TrackWidth / 2.f, GroundWidth, TopBoxY);
		GroundBoxTop->SetRelativeLocation(NewLocation);
	}

	if (GroundBoxBottom)
	{
		GroundBoxBottom->SetBoxExtent(FVector(TrackWidth * TrackNum / 2.f, GroundWidth, 1.f));
		FVector NewLocation(GroundBoxBottom->GetScaledBoxExtent().X - TrackWidth / 2.f, GroundWidth, BottomBoxY);
		GroundBoxBottom->SetRelativeLocation(NewLocation);
	}
}

void ATrackField::SetupGroundBoxSettings(UBoxComponent* InBox) const
{
	InBox->SetMobility(EComponentMobility::Static);
	InBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	InBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	InBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}
