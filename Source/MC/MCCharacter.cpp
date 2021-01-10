// Copyright Epic Games, Inc. All Rights Reserved.

#include "MCCharacter.h"

#include "DrawDebugHelpers.h"
#include "MCProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "utils/Utils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMCCharacter

bool AMCCharacter::ChunkExist(int32 X, int32 Y)
{
	if (Chunks.Contains(X) && Chunks.Find(X)->Contains(Y))
	{
		return true;
	}
	return false;
}

void AMCCharacter::SpawnChunk(int32 X, int32 Y)
{
	if (!Chunks.Contains(X))
	{
		Chunks.Add(X, TMap<int32, AChunk*>());
	}
	AChunk* Chunk = Cast<AChunk>(GetWorld()->SpawnActor(ChunkBlueprint.Get(), new FTransform(FVector(X, Y, 0.f))));
	Chunk->GenerateChunk(NoiseGenerator);
	Chunks.Find(X)->Add(Y, Chunk);
}

void AMCCharacter::DestroyChunk(int32 X, int32 Y)
{
	 TMap<int, AChunk*>* XMap = Chunks.Find(X);
	AChunk* Chunk = *XMap->Find(Y);
	Chunk->Destroy();
	XMap->Remove(Y);
	XMap->Shrink();
}

AMCCharacter::AMCCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);


	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	ChunkSize = 100 * 50;

	ChunkGenerationDistance = ChunkSize * 10;

	ChunkDespawnDistance = ChunkSize * 15;

	NoiseGenerator = CreateDefaultSubobject<UFastNoiseWrapper>("NoiseGenerator");
	NoiseType = EFastNoise_NoiseType::Perlin;

	BlockPlaceDestroyDistance = 1500;

}


void AMCCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	NoiseGenerator->SetupFastNoise(NoiseType);
}

void AMCCharacter::Tick(float DeltaSeconds)
{
	// Update chunks
	const auto Location = GetActorLocation();
	for(int StartX = Utils::SnapValue(Location.X - ChunkGenerationDistance, ChunkSize); StartX < Utils::SnapValue(Location.X + ChunkGenerationDistance , ChunkSize); StartX+= ChunkSize)
	{
		for(int StartY = Utils::SnapValue(Location.Y - ChunkGenerationDistance, ChunkSize); StartY < Utils::SnapValue(Location.Y + ChunkGenerationDistance, ChunkSize); StartY+= ChunkSize)
		{
			if (FVector2D::Distance(FVector2D(StartX, StartY), FVector2D(Location.X, Location.Y)) < ChunkDespawnDistance) {
				if (!ChunkExist(StartX, StartY))
				{
					SpawnChunk(StartX, StartY);
				}
			}
		}
	}
	for (const TPair<int32, TMap<int32, AChunk*>> XPair: Chunks)
	{
		for(const TPair<int32, AChunk*> YPair : XPair.Value)
		{
			if (FVector2D::Distance(FVector2D(XPair.Key, YPair.Key), FVector2D(Location.X, Location.Y)) > ChunkDespawnDistance)
			{
				DestroyChunk(XPair.Key, YPair.Key);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind Block events
	PlayerInputComponent->BindAction("PlaceBlock", IE_Pressed, this, &AMCCharacter::OnPlaceBlock);
	PlayerInputComponent->BindAction("TakeBlock", IE_Pressed, this, &AMCCharacter::OnTakeBlock);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMCCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMCCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMCCharacter::LookUpAtRate);
}

void AMCCharacter::OnPlaceBlock()
{
	FHitResult* RayCastHitResult = new FHitResult();
	const FVector StartTrace = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector ForwardVector = GetFirstPersonCameraComponent()->GetForwardVector();
	const FVector EndTrace = (ForwardVector * BlockPlaceDestroyDistance) + StartTrace;
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	if (GetWorld()->LineTraceSingleByChannel(*RayCastHitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		AChunk* Chunk = Cast<AChunk>(RayCastHitResult->Actor);
		if (Chunk != nullptr)
		{
			Chunk->PlaceBlock(RayCastHitResult->Location.X, RayCastHitResult->Location.Y, RayCastHitResult->Location.Z, CurrentBlock, ForwardVector);
		}
	}

	// try and play the sound if specified
	if (PlaceBlockSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PlaceBlockSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (ToolActionAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(ToolActionAnimation, 1.f);
		}
	}
}

void AMCCharacter::OnTakeBlock()
{
	FHitResult* RayCastHitResult = new FHitResult();
	const FVector StartTrace = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector ForwardVector = GetFirstPersonCameraComponent()->GetForwardVector();
	const FVector EndTrace = (ForwardVector * BlockPlaceDestroyDistance) + StartTrace;
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	TraceParams->AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(*RayCastHitResult, StartTrace, EndTrace, ECC_Camera, *TraceParams))
	{
		AChunk* Chunk = Cast<AChunk>(RayCastHitResult->Actor);
		const FVector Location = RayCastHitResult->Location;
		DrawDebugLine(GetWorld(), StartTrace, Location, FColor(255, 0, 0), true);
		if (Chunk != nullptr)
		{
			CurrentBlock = Chunk->DestroyBlock(Location.X, Location.Y, Location.Z, ForwardVector);
		}
	}

	// try and play the sound if specified
	if (PlaceBlockSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PlaceBlockSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (ToolActionAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(ToolActionAnimation, 1.f);
		}
	}
}

void AMCCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMCCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPlaceBlock();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AMCCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AMCCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMCCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMCCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMCCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AMCCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMCCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AMCCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMCCharacter::TouchUpdate);
		return true;
	}

	return false;
}
