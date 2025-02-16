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
		UE_LOG(LogTemp, Warning, TEXT("No track sprite assigned"));
		return;
	}

	// GetRenderBounds return Sprite render bounds that scaled by Pixel Per Unit(PPU)
	float TrackWidth = TrackSprite->GetRenderBounds().GetBox().GetSize().X;
	
	GenerateTrackSprites(TrackWidth);
	PlaceGroundBox(GroundBoxTop, TrackWidth);
	PlaceGroundBox(GroundBoxBottom, TrackWidth);
}

void ATrackField::SetupGroundBoxSettings(UBoxComponent* InBox)
{
	InBox->SetMobility(EComponentMobility::Static);
	InBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	InBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	InBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ATrackField::PlaceGroundBox(UBoxComponent* const InBox, const float InTrackWidth) const
{
	if (InBox == nullptr) return;

	InBox->SetBoxExtent(FVector(InTrackWidth * TrackNum / 2.f, GroundWidth, 1.f));
	FVector NewLocation(InBox->GetScaledBoxExtent().X - InTrackWidth / 2.f, GroundWidth, TopBoxY);
	InBox->SetRelativeLocation(NewLocation);
}

void ATrackField::GenerateTrackSprites(const float InTrackWidth)
{
	if (TrackSprite == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No track sprite assigned"));
		return;
	}
	
	for (int32 TrackIndex = 0; TrackIndex < TrackNum; ++TrackIndex)
	{
		FString TrackName = FString::Printf(TEXT("Track_%d"), TrackIndex);
		TObjectPtr<UPaperSpriteComponent> NewTrackComp = NewObject<UPaperSpriteComponent>(this, *TrackName);
		FAttachmentTransformRules TransformRules(EAttachmentRule::KeepWorld, false);
		NewTrackComp->AttachToComponent(TrackGroup, TransformRules);
		NewTrackComp->RegisterComponent();
		
		FVector NewLocation(TrackIndex * InTrackWidth, 0.f, 0.f);
		NewTrackComp->SetRelativeLocation(NewLocation);
		NewTrackComp->SetSprite(TrackSprite);
		NewTrackComp->SetTranslucentSortPriority(-1.f);
	}
}
