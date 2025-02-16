// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Environment/TrackGoal.h"
#include "TrackField.generated.h"

class UPaperSprite;
class UPaperSpriteComponent;
class UBoxComponent;

UCLASS()
class ATrackField : public AActor
{
	GENERATED_BODY()

	// ============================
	// AActor
	// ============================
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// ============================
	// ATrackField
	// ============================
public:
	ATrackField();

private:
	/// Set default settings of UBoxComponent for using it as ground on the track
	/// @param InBox Source box component be used as ground
	static void SetupGroundBoxSettings(UBoxComponent* InBox);
	
	/// Place UBoxComponent on the track based on Track width(and number of them) 
	/// @param InBox UBoxComponent to place
	/// @param InTrackWidth The length of the track
	void PlaceGroundBox(UBoxComponent* const InBox, const float InTrackWidth) const;

	/// Generate track background
	/// @param InTrackWidth The length of the track
	void GenerateTrackSprites(const float InTrackWidth);
	
	UPROPERTY(EditDefaultsOnly, Category="TrackField")
	int32  TrackNum{1};
	UPROPERTY(EditDefaultsOnly, Category="TrackField")
	TObjectPtr<UPaperSprite> TrackSprite;
	UPROPERTY(VisibleDefaultsOnly, Category="TrackField")
	TObjectPtr<USceneComponent> TrackGroup;
	
	UPROPERTY(VisibleDefaultsOnly, Category="TrackField|GroundBox")
	TObjectPtr<UBoxComponent> GroundBoxTop;
	UPROPERTY(EditDefaultsOnly, Category="TrackField|GroundBox")
	float TopBoxY{0.f};
	UPROPERTY(VisibleDefaultsOnly, Category="TrackField|GroundBox")
	TObjectPtr<UBoxComponent> GroundBoxBottom;
	UPROPERTY(EditDefaultsOnly, Category="TrackField|GroundBox")
	float BottomBoxY{0.f};
	UPROPERTY(EditDefaultsOnly, Category="TrackField|GroundBox")
	float GroundWidth{10.f};

	UPROPERTY(EditDefaultsOnly, Category="TrackField|Goal")
	TObjectPtr<ATrackGoal> Goal; 
};
