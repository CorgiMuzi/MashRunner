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

	if (TrackGroup)
	{
		TrackGroup->SetMobility(EComponentMobility::Type::Static);		
	}
	
}

void ATrackField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (TrackSprite == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No track sprite assigned"));
		return;
	}
	
	RefreshTrackSpriteComponents();
	RefreshTrackGoalActors();
	
	// GetRenderBounds return Sprite render bounds that scaled by Pixel Per Unit(PPU)
	float TrackWidth = TrackSprite->GetRenderBounds().GetBox().GetSize().X;
	GenerateTrackSprites(TrackWidth);
	PlaceGroundBox(GroundBoxTop, TrackWidth, TopBoxY);
	PlaceGroundBox(GroundBoxBottom, TrackWidth, BottomBoxY);

	SpawnGoalLine(TrackWidth);
}

void ATrackField::SetupGroundBoxSettings(UBoxComponent* InBox)
{
	InBox->SetMobility(EComponentMobility::Static);
	InBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	InBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	InBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void ATrackField::PlaceGroundBox(UBoxComponent* const InBox, const float InTrackWidth, const float InY) const
{
	if (InBox == nullptr) return;

	InBox->SetBoxExtent(FVector(InTrackWidth * TrackNum / 2.f, GroundWidth, 10.f));
	FVector NewLocation(InBox->GetScaledBoxExtent().X - InTrackWidth / 2.f, GroundWidth, InY);
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
		if (UPaperSpriteComponent* NewTrackComp = NewObject<UPaperSpriteComponent>(this, UPaperSpriteComponent::StaticClass(), *TrackName))
		{
			NewTrackComp->RegisterComponent();
			NewTrackComp->SetSprite(TrackSprite);
			FVector NewLocation(TrackIndex * InTrackWidth, 0.f, 0.f);
			NewTrackComp->SetRelativeLocation(NewLocation);
			NewTrackComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void ATrackField::RefreshTrackSpriteComponents()
{
	for (UPaperSpriteComponent* SpriteComp : TrackSpriteComponents)
	{
		if (SpriteComp) SpriteComp->DestroyComponent();
	}

	TrackSpriteComponents.Empty();
}

void ATrackField::SpawnGoalLine(const float InTrackWidth)
{
	// 300.f is hard coded value, you can change it if you want to adjust new position
	float TrackGoalXPosition = InTrackWidth * (TrackNum - 3) - 300.f;
	TrackGoalInstances.Add(GetWorld()->SpawnActor<ATrackGoal>(TrackGoal, FVector(TrackGoalXPosition, 0.f, TopGoalZPosition), FRotator::ZeroRotator));
	TrackGoalInstances.Add(GetWorld()->SpawnActor<ATrackGoal>(TrackGoal, FVector(TrackGoalXPosition, 0.f, BottomGoalZPosition), FRotator::ZeroRotator));
}

void ATrackField::RefreshTrackGoalActors()
{
	for (UPaperSpriteComponent* SpriteComp : TrackSpriteComponents)
	{
		if (SpriteComp) SpriteComp->DestroyComponent();
	}

	TrackSpriteComponents.Empty();
}
