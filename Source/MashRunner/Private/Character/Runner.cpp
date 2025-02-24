// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Runner.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MashRunner/MashRunnerGameModeBase.h"

ARunner::ARunner() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(FName("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
}

void ARunner::OnWinnerAnnounced()
{
	if (AMashRunnerGameModeBase* GameMode = Cast<AMashRunnerGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		bCanRun = !GameMode->bIsGameFinished;
	}
}

void ARunner::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		AddCharacterMappingContext(PlayerController);
	}

	if (AMashRunnerGameModeBase* GameMode = Cast<AMashRunnerGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnWinnerAnnounced.AddUObject(this, &ARunner::OnWinnerAnnounced);
	}

	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	GetSprite()->SetFlipbook(IdleFlipbook);
	GetSprite()->SetTranslucentSortPriority(2);

	if (AccelerationRate)
	{
		FRichCurve* RichCurve = &AccelerationRate->FloatCurve;
		if (RichCurve->Keys.Num() >= 2)
		{
			RichCurve->Keys[0].Time = 0.f;
			RichCurve->Keys[0].Value = 1.f;
			RichCurve->Keys[1].Time = MaxSpeed;
			RichCurve->Keys[1].Value = 0.1f;
		}
	}

	if (DecelerationRate)
	{
		FRichCurve* RichCurve = &DecelerationRate->FloatCurve;
		if (RichCurve->Keys.Num() >= 2)
		{
			RichCurve->Keys[0].Time = 0.f;
			RichCurve->Keys[0].Value = 0.f;
			RichCurve->Keys[1].Time = MaxSpeed;
			RichCurve->Keys[1].Value = 1.f;
		}
	}
}

void ARunner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(
		GetCharacterMovement()->MaxWalkSpeed - DecelerationUnit * DecelerationRate->GetFloatValue(
			GetCharacterMovement()->MaxWalkSpeed),0.f, MaxSpeed);

	AddMovementInput(GetActorForwardVector());

	if (GetCharacterMovement()->MaxWalkSpeed > 0)
	{
		GetSprite()->SetFlipbook(RunFlipbook);
		GetSprite()->SetPlayRate(GetVelocity().Size() / MaxSpeed);
		if ((GetSprite()->GetPlaybackPositionInFrames() == 1 || GetSprite()->GetPlaybackPositionInFrames() == 5) &&
			!bFootstepSoundPlayed)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), FootstepSound/*, GetActorLocation(), GetActorRotation()*/);
			bFootstepSoundPlayed = true;
		}
		else if (GetSprite()->GetPlaybackPositionInFrames() != 1 && GetSprite()->GetPlaybackPositionInFrames() != 5)
		{
			bFootstepSoundPlayed = false;
		}
	}
	else
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

float ARunner::GetCurrentAccelerationRate() const
{
	return FMath::Clamp(GetCharacterMovement()->MaxWalkSpeed / MaxSpeed, 0.f, 1.f);
}

void ARunner::LeftButtonPress()
{
	if (!bCanRun || LastPressedButton == ELastPressedButton::LPB_Left) return;
	LastPressedButton = ELastPressedButton::LPB_Left;

	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(
		GetCharacterMovement()->MaxWalkSpeed + AccelerationUnit * AccelerationRate->GetFloatValue(
			GetCharacterMovement()->MaxWalkSpeed)
		, 0.f, MaxSpeed);
}

void ARunner::RightButtonPress()
{
	if (!bCanRun || LastPressedButton == ELastPressedButton::LPB_Right) return;
	LastPressedButton = ELastPressedButton::LPB_Right;

	GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(
		GetCharacterMovement()->MaxWalkSpeed + AccelerationUnit * AccelerationRate->GetFloatValue(
			GetCharacterMovement()->MaxWalkSpeed)
		, 0.f, MaxSpeed);
}
