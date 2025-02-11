// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Runner.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARunner::ARunner() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(FName("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
}

void ARunner::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		AddCharacterMappingContext(PlayerController);
	}

	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	GetSprite()->SetFlipbook(IdleFlipbook);
}

void ARunner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AddMovementInput(GetActorForwardVector());
	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(GetCharacterMovement()->MaxWalkSpeed - (DecelerationRate * DeltaSeconds), 0.f, MaxSpeed);
	if (GetVelocity().Length() > 0 && GetSprite()->GetFlipbook() != RunFlipbook)
	{
		GetSprite()->SetFlipbook(RunFlipbook);
		GetSprite()->SetPlayRate(GetVelocity().Length() / MaxSpeed);
	}else
	{
		GetSprite()->SetFlipbook(IdleFlipbook);
		GetSprite()->SetPlayRate(1.f);
	}
}

void ARunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(LeftButtonAction, ETriggerEvent::Triggered, this, &ThisClass::LeftButtonPress);
		EnhancedInput->BindAction(RightButtonAction, ETriggerEvent::Triggered, this, &ThisClass::RightButtonPress);
	}
}

void ARunner::AddCharacterMappingContext(const APlayerController* PlayerController) const
{
	if (PlayerController == nullptr) return;
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer());
	if (Subsystem && InputMappingContext)
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ARunner::LeftButtonPress()
{
	if (LastPressedButton == ELastPressedButton::LPB_Left) return;
	LastPressedButton = ELastPressedButton::LPB_Left;

	GetCharacterMovement()->MaxWalkSpeed += AccelerationRate;
}

void ARunner::RightButtonPress()
{
	if (LastPressedButton == ELastPressedButton::LPB_Right) return;
	LastPressedButton = ELastPressedButton::LPB_Right;

	GetCharacterMovement()->MaxWalkSpeed += AccelerationRate;
}

